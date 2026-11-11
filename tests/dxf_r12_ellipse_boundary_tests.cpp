// SPDX-License-Identifier: GPL-2.0-or-later
#include "libdxfrw.h"
#include "lc3_compat_check.cpp"
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>

namespace {
constexpr double turn = M_PIx2;
struct Writer : LC3CompatCheck {
    explicit Writer(dxfRW& w) : output(w) {}
    void writeEntities() override { accepted = output.writeEllipse(ellipse); }
    bool finalizeDxfWrite() override { return accepted; }
    dxfRW& output; DRW_Ellipse* ellipse = nullptr; bool accepted = false;
};
struct CcwWriter : Writer, DRW_DxfR12CcwEllipseOptIn { using Writer::Writer; };
struct Reader : LC3CompatCheck {
    void addPolyline(const DRW_Polyline& value) override { polyline = value; ++count; }
    DRW_Polyline polyline; int count = 0;
};
std::string contents(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
DRW_Coord point(const DRW_Ellipse& e, double angle, bool threeD) {
    const double x = e.basePoint.x + e.secPoint.x * std::cos(angle);
    const double minor = e.secPoint.x * e.ratio * std::sin(angle);
    return threeD ? DRW_Coord(x, e.basePoint.y, e.basePoint.z - minor)
                  : DRW_Coord(x, e.basePoint.y + minor, 0.);
}
bool near(const DRW_Coord& a, const DRW_Coord& b) {
    return std::abs(a.x-b.x) < 1.e-7 && std::abs(a.y-b.y) < 1.e-7
        && std::abs(a.z-b.z) < 1.e-7;
}
bool checkPolyline(const DRW_Polyline& p, const DRW_Ellipse& e, bool threeD,
                   bool closed, std::size_t count) {
    if (p.vertlist.size() != count || ((p.flags & 1) != 0) != closed
        || ((p.flags & 8) != 0) != threeD || !p.vertlist.front()) return false;
    if (!closed && (!p.vertlist.back()
        || !near(p.vertlist.front()->basePoint, point(e, e.staparam, threeD))
        || !near(p.vertlist.back()->basePoint, point(e, e.endparam, threeD)))) return false;
    return true;
}
bool run(const std::filesystem::path& path, bool binary, bool threeD, bool opted,
         double start, double end, bool closed, std::size_t count) {
    DRW_Ellipse ellipse; ellipse.basePoint = threeD ? DRW_Coord(2.,3.,4.) : DRW_Coord(2.,3.,0.);
    ellipse.secPoint = {1.e8,0.,0.}; ellipse.extPoint = threeD ? DRW_Coord(0.,1.,0.) : DRW_Coord(0.,0.,1.);
    ellipse.ratio = .5; ellipse.staparam = start; ellipse.endparam = end; ellipse.isccw = 1;
    // Direct API uses its legacy signed sweep; the opt-in is a writer policy.
    DRW_Polyline direct; ellipse.toPolyline(&direct,128);
    const bool directClosed = start == end || end == start + turn || end == start - turn;
    const std::size_t directCount = directClosed ? 128u :
        static_cast<std::size_t>(std::max(1., std::ceil(128. * std::abs(end-start) / turn))) + 1u;
    if (!checkPolyline(direct,ellipse,threeD,directClosed,directCount)) return false;
    dxfRW output(path.string().c_str()); Writer legacy(output); CcwWriter ccw(output);
    Writer* writer = opted ? static_cast<Writer*>(&ccw) : &legacy;
    writer->ellipse = &ellipse;
    if (!output.write(writer,DRW::AC1009,binary) || !writer->accepted
        || ellipse.staparam != start || ellipse.endparam != end) return false;
    dxfRW input(path.string().c_str()); Reader reader;
    if (!input.read(&reader,false) || reader.count != 1
        || !checkPolyline(reader.polyline,ellipse,threeD,closed,count)) return false;
    dxfRW extrusionInput(path.string().c_str()); Reader extrusionReader;
    return extrusionInput.read(&extrusionReader,true) && extrusionReader.count == 1
        && checkPolyline(extrusionReader.polyline,ellipse,threeD,closed,count);
}
bool invalidRetry(const std::filesystem::path& path, bool binary) {
    {std::ofstream existing(path,std::ios::binary|std::ios::trunc); existing << "sentinel";}
    DRW_Ellipse ellipse; ellipse.secPoint={2.,0.,0.}; ellipse.ratio=.5;
    ellipse.staparam=.5; ellipse.endparam=.5+5.e-11;
    ellipse.extPoint={0.,0.,0.};
    dxfRW output(path.string().c_str()); Writer writer(output); writer.ellipse=&ellipse;
    if (output.write(&writer,DRW::AC1009,binary) || writer.accepted || contents(path)!="sentinel") return false;
    ellipse.extPoint={0.,0.,1.};
    if (!output.write(&writer,DRW::AC1009,binary) || !writer.accepted) return false;
    dxfRW input(path.string().c_str()); Reader reader;
    if (!input.read(&reader,false) || reader.count != 1 || reader.polyline.vertlist.size() != 2
        || (reader.polyline.flags & 1) != 0) return false;
    {std::ofstream existing(path,std::ios::binary|std::ios::trunc); existing << "sentinel";}
    ellipse.staparam = 0.; ellipse.endparam = turn + 5.e-11;
    if (output.write(&writer,DRW::AC1009,binary) || writer.accepted || contents(path)!="sentinel") return false;
    ellipse.endparam = turn - 5.e-11;
    return output.write(&writer,DRW::AC1009,binary) && writer.accepted;
}
}
int main() {
    const auto path=std::filesystem::temp_directory_path() /
        ("ellipse-boundary-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count())+".dxf");
    struct Cleanup { std::filesystem::path path; ~Cleanup(){std::error_code e;std::filesystem::remove(path,e);} } cleanup{path};
    for (bool binary : {false,true}) for (bool threeD : {false,true}) for (bool opted : {false,true}) {
        struct Case {double start,end;bool closed;std::size_t legacy,opt;};
        const Case cases[] = {
            {.5,.5+5.e-11,false,2u,2u},
            {.5,.5-5.e-11,false,2u,129u},
            {turn-5.e-11,0.,false,129u,2u},
            {0.,turn-5.e-11,false,129u,129u},
            {.5,.5,true,128u,128u},
            {0.,turn,true,128u,128u},
            {turn,0.,true,128u,128u},
            {.5,.5+turn,true,128u,128u},
            {.5,.5-turn,true,128u,128u}
        };
        for (const Case& c : cases) if (!run(path,binary,threeD,opted,c.start,c.end,c.closed,opted?c.opt:c.legacy)) {
            std::cerr << "ellipse boundary failed binary="<<binary<<" 3d="<<threeD<<" opted="<<opted
                      <<" start="<<c.start<<" end="<<c.end<<'\n'; return 1;
        }
    }
    for (bool binary : {false,true}) if (!invalidRetry(path,binary)) return 1;
    return 0;
}
