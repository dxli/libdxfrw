// SPDX-License-Identifier: GPL-2.0-or-later
#include "lc3_compat_check.cpp"
#include "drw_dxf_extrusion.h"
#include "libdwgr.h"

#include <array>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {
constexpr double tolerance = 1.e-9;
bool near(double a, double b) { return std::abs(a - b) < tolerance; }
bool same(const DRW_Coord& a, const DRW_Coord& b) {
    return near(a.x, b.x) && near(a.y, b.y) && near(a.z, b.z);
}
bool same(const DRW_Circle& a, const DRW_Circle& b) {
    return same(a.basePoint, b.basePoint) && same(a.extPoint, b.extPoint)
        && near(a.radious, b.radious) && near(a.thickness, b.thickness)
        && a.haveExtrusion == b.haveExtrusion && a.layer == b.layer;
}
bool same(const DRW_Arc& a, const DRW_Arc& b) {
    return same(static_cast<const DRW_Circle&>(a), static_cast<const DRW_Circle&>(b))
        && near(a.staangle, b.staangle) && near(a.endangle, b.endangle)
        && a.isccw == b.isccw;
}
struct Adapter : LC3CompatCheck {
    std::vector<DRW_Circle> circles;
    std::vector<DRW_Arc> arcs;
    dxfRW* writer = nullptr;
    bool convert = false;
    bool failed = false;
    void addCircle(const DRW_Circle& value) override { circles.push_back(value); }
    void addArc(const DRW_Arc& value) override { arcs.push_back(value); }
    void writeEntities() override {
        failed = false;
        for (const auto& value : circles) {
            DRW_Circle output;
            if (convert && !DRW_DxfLegacyExtrusion::toNativeOcs(value, output)) {
                failed = true;
                return;
            }
            if (!convert) output = value;
            if (!writer || !writer->writeCircle(&output)) { failed = true; return; }
        }
        for (const auto& value : arcs) {
            DRW_Arc output;
            if (convert && !DRW_DxfLegacyExtrusion::toNativeOcs(value, output)) {
                failed = true;
                return;
            }
            if (!convert) output = value;
            if (!writer || !writer->writeArc(&output)) { failed = true; return; }
        }
    }
    bool finalizeDxfWrite() override { return !failed; }
};
Adapter authored() {
    Adapter result;
    const std::array<DRW_Coord, 6> normals{{
        {0., 0., 1.}, {0., 0., -1.}, {0.6, 0., 0.8},
        {3., 4., -5.}, {0.015625, 0., 1.}, {0.0158, 0., 1.}
    }};
    for (const auto& normal : normals) {
        DRW_Circle circle;
        circle.basePoint = {2., 3., 4.};
        circle.extPoint = normal;
        circle.radious = 2.;
        circle.thickness = 1.5;
        result.circles.push_back(circle);
        DRW_Arc arc;
        arc.basePoint = {2., 3., 4.};
        arc.extPoint = normal;
        arc.radious = 3.;
        arc.thickness = 0.5;
        arc.staangle = 0.25;
        arc.endangle = 1.5;
        result.arcs.push_back(arc);
    }
    return result;
}
bool writeDxf(const std::filesystem::path& path, Adapter& data, bool binary) {
    const std::string name = path.string();
    dxfRW writer(name.c_str());
    data.writer = &writer;
    return writer.write(&data, DRW::AC1027, binary) && !data.failed;
}
struct DwgAdapter final : Adapter, DRW_DwgDefaultMaterialProvider,
                          DRW_DwgDefaultPlotStyleProvider {
    dwgRW* dwgWriter = nullptr;
    bool wantsDwgDefaultMaterialGraph() const override { return true; }
    bool wantsDwgDefaultPlotStyleGraph() const override { return true; }
    bool dwgDefaultMaterialOpaqueReferencesAbsent(std::uint32_t a, std::uint32_t b,
            std::uint32_t c, std::uint32_t d) const override { return a && b && c && d; }
    void dwgDefaultMaterialGraphPlanned(std::uint32_t, std::uint32_t,
                                       std::uint32_t, std::uint32_t) override {}
    void writeHeader(DRW_Header& header) override {
        for (const char* key : {"$CMATERIAL", "CMATERIAL"}) {
            const auto found = header.vars.find(key);
            if (found != header.vars.end()) { delete found->second; header.vars.erase(found); }
        }
    }
    void writeEntities() override {
        failed = false;
        for (const auto& value : circles) {
            DRW_Circle output;
            if (convert && !DRW_DxfLegacyExtrusion::toNativeOcs(value, output)) { failed = true; return; }
            if (!convert) output = value;
            output.handle = 0u;
            if (!dwgWriter || !dwgWriter->writeCircle(&output)) { failed = true; return; }
        }
        for (const auto& value : arcs) {
            DRW_Arc output;
            if (convert && !DRW_DxfLegacyExtrusion::toNativeOcs(value, output)) { failed = true; return; }
            if (!convert) output = value;
            output.handle = 0u;
            if (!dwgWriter || !dwgWriter->writeArc(&output)) { failed = true; return; }
        }
    }
    bool finalizeDwgWrite() override { return !failed; }
};
bool writeDwg(dwgRW& writer, DwgAdapter& data) {
    data.dwgWriter = &writer;
    return writer.write(&data, DRW::AC1024, true) && !data.failed;
}
bool readDwg(const std::filesystem::path& path, Adapter& data, bool apply) {
    const std::string name = path.string();
    dwgR reader(name.c_str());
    return reader.read(&data, apply);
}
bool readDxf(const std::filesystem::path& path, Adapter& data, bool apply) {
    const std::string name = path.string();
    dxfRW reader(name.c_str());
    return reader.read(&data, apply);
}
bool sameGeometry(const Adapter& a, const Adapter& b, bool compareFlag = true) {
    if (a.circles.size() != b.circles.size() || a.arcs.size() != b.arcs.size())
        return false;
    for (std::size_t i = 0; i < a.circles.size(); ++i) {
        DRW_Circle left = a.circles[i], right = b.circles[i];
        if (!compareFlag) left.haveExtrusion = right.haveExtrusion;
        if (!same(left, right)) return false;
    }
    for (std::size_t i = 0; i < a.arcs.size(); ++i) {
        DRW_Arc left = a.arcs[i], right = b.arcs[i];
        if (!compareFlag) left.haveExtrusion = right.haveExtrusion;
        if (!same(left, right)) return false;
    }
    return true;
}
bool roundTrip(const std::filesystem::path& dir, bool binary) {
    const auto original = dir / (binary ? "binary-original.dxf" : "ascii-original.dxf");
    const auto converted = dir / (binary ? "binary-converted.dxf" : "ascii-converted.dxf");
    Adapter source = authored();
    if (!writeDxf(original, source, binary)) return false;
    Adapter raw, transformed;
    if (!readDxf(original, raw, false) || !readDxf(original, transformed, true)) return false;
    Adapter sourceSnapshot = transformed;
    transformed.convert = true;
    if (!writeDxf(converted, transformed, binary)
        || !sameGeometry(sourceSnapshot, transformed)) return false;
    Adapter gotRaw, gotTransformed;
    if (!readDxf(converted, gotRaw, false) || !readDxf(converted, gotTransformed, true))
        return false;
    return sameGeometry(raw, gotRaw) && sameGeometry(sourceSnapshot, gotTransformed);
}
bool dwgRoundTrip(const std::filesystem::path& dir) {
    const auto original = dir / "ac1024-original.dwg";
    const auto converted = dir / "ac1024-converted.dwg";
    DwgAdapter source;
    static_cast<Adapter&>(source) = authored();
    dwgRW first(original.string().c_str());
    if (!writeDwg(first, source)) return false;
    Adapter raw, applied;
    if (!readDwg(original, raw, false) || !readDwg(original, applied, true)) return false;
    const Adapter before = applied;
    DwgAdapter output;
    static_cast<Adapter&>(output) = applied;
    output.convert = true;
    dwgRW second(converted.string().c_str());
    if (!writeDwg(second, output) || !sameGeometry(before, output)) return false;
    Adapter resultRaw, resultApplied;
    if (!readDwg(converted, resultRaw, false) || !readDwg(converted, resultApplied, true)) return false;
    return sameGeometry(raw, resultRaw) && sameGeometry(before, resultApplied);
}
bool dwgRefusalAndRetry(const std::filesystem::path& path) {
    DwgAdapter input;
    static_cast<Adapter&>(input) = authored();
    input.circles[1].extPoint = {0., 0., 0.};
    input.circles[1].haveExtrusion = true;
    input.convert = true;
    { std::ofstream destination(path, std::ios::binary); destination << "unchanged"; }
    dwgRW writer(path.string().c_str());
    if (writeDwg(writer, input)) return false;
    std::ifstream previous(path, std::ios::binary);
    if (std::string((std::istreambuf_iterator<char>(previous)), {}) != "unchanged") return false;
    previous.close();
    input.circles[1].extPoint = {0., 0., -1.};
    return writeDwg(writer, input);
}
bool refusalAndRetry(const std::filesystem::path& path, bool binary) {
    Adapter input = authored();
    // Fail after one record has already been staged.
    input.circles[1].extPoint = {0., 0., 0.};
    input.circles[1].haveExtrusion = true;
    input.convert = true;
    { std::ofstream destination(path, std::ios::binary); destination << "unchanged"; }
    const std::string name = path.string();
    dxfRW writer(name.c_str());
    input.writer = &writer;
    if (writer.write(&input, DRW::AC1027, binary)) return false;
    { std::ifstream destination(path, std::ios::binary);
      const std::string actual((std::istreambuf_iterator<char>(destination)), {});
      if (actual != "unchanged") return false; }
    input.circles[1].extPoint = {0., 0., 1.};
    if (!writer.write(&input, DRW::AC1027, binary) || input.failed) return false;
    Adapter read;
    return readDxf(path, read, false) && sameGeometry(input, read, false);
}
bool directControls() {
    DRW_Circle source;
    source.basePoint = {2., 3., 4.};
    source.extPoint = {0.6, 0., 0.8};
    source.radious = 3.;
    // An authored object can have a nondefault normal and a false flag.
    // applyExtrusion then does nothing; the explicit inverse must also do so.
    source.applyExtrusion();
    DRW_Circle output;
    if (!DRW_DxfLegacyExtrusion::toNativeOcs(source, output) || !same(source, output))
        return false;
    source.haveExtrusion = true;
    const DRW_Circle native = source;
    source.applyExtrusion();
    if (!DRW_DxfLegacyExtrusion::toNativeOcs(source, source) || !same(source, native))
        return false;
    DRW_Circle unchanged;
    unchanged.basePoint = {91., 92., 93.};
    unchanged.radious = 42.;
    const DRW_Circle before = unchanged;
    for (const DRW_Coord normal : {DRW_Coord{0.,0.,0.},
            DRW_Coord{std::numeric_limits<double>::infinity(),0.,1.},
            DRW_Coord{std::numeric_limits<double>::quiet_NaN(),0.,1.}}) {
        DRW_Circle invalid = native;
        invalid.extPoint = normal;
        if (DRW_DxfLegacyExtrusion::toNativeOcs(invalid, unchanged)
            || !same(unchanged, before)) return false;
    }
    // The inverse must use the same stable normalization as applyExtrusion,
    // including extreme finite and subnormal normals.
    const double maximum = std::numeric_limits<double>::max();
    const double tiny = std::numeric_limits<double>::denorm_min();
    for (const DRW_Coord normal : {DRW_Coord{maximum,maximum/4.,-maximum/2.},
                                   DRW_Coord{tiny,tiny,tiny},
                                   DRW_Coord{0.,0.,tiny}, DRW_Coord{0.,0.,-tiny}}) {
        DRW_Circle extreme = native;
        extreme.extPoint = normal;
        const DRW_Circle expected = extreme;
        extreme.applyExtrusion();
        if (!DRW_DxfLegacyExtrusion::toNativeOcs(extreme, extreme)
            || !same(extreme, expected)) return false;
        DRW_Arc extremeArc;
        extremeArc.basePoint = {2.,3.,4.}; extremeArc.extPoint = normal;
        extremeArc.radious = 2.; extremeArc.staangle = 0.25;
        extremeArc.endangle = 1.5; extremeArc.haveExtrusion = true;
        const DRW_Arc expectedArc = extremeArc;
        extremeArc.applyExtrusion();
        if (!DRW_DxfLegacyExtrusion::toNativeOcs(extremeArc, extremeArc)
            || !same(extremeArc, expectedArc)) return false;
    }
    DRW_Circle nonfinite = native;
    nonfinite.basePoint.x = std::numeric_limits<double>::quiet_NaN();
    if (DRW_DxfLegacyExtrusion::toNativeOcs(nonfinite, unchanged)
        || !same(unchanged, before)) return false;
    // Finite inputs can overflow while being projected; refusal must be atomic.
    DRW_Circle overflow = native;
    overflow.haveExtrusion = true;
    overflow.extPoint = {1., 1., 0.};
    overflow.basePoint = {std::numeric_limits<double>::max(),
                          -std::numeric_limits<double>::max(), 0.};
    if (DRW_DxfLegacyExtrusion::toNativeOcs(overflow, unchanged)
        || !same(unchanged, before)) return false;
    struct DerivedCircle : DRW_Circle {} derived;
    const DRW_Circle& cast = derived;
    if (DRW_DxfLegacyExtrusion::toNativeOcs(cast, unchanged)
        || !same(unchanged, before)) return false;
    DerivedCircle derivedOutput;
    derivedOutput.basePoint = {81., 82., 83.};
    const DRW_Circle derivedBefore = derivedOutput;
    if (DRW_DxfLegacyExtrusion::toNativeOcs(native, derivedOutput)
        || !same(derivedOutput, derivedBefore)) return false;
    struct DerivedArc : DRW_Arc {} derivedArcOutput;
    derivedArcOutput.basePoint = {71.,72.,73.};
    const DRW_Arc derivedArcBefore = derivedArcOutput;
    DRW_Arc arcInput;
    if (DRW_DxfLegacyExtrusion::toNativeOcs(arcInput, derivedArcOutput)
        || !same(derivedArcOutput, derivedArcBefore)) return false;
    DerivedArc derivedArcInput;
    DRW_Arc untouchedArc;
    untouchedArc.basePoint = {61.,62.,63.};
    const DRW_Arc untouchedArcBefore = untouchedArc;
    if (DRW_DxfLegacyExtrusion::toNativeOcs(derivedArcInput, untouchedArc)
        || !same(untouchedArc, untouchedArcBefore)) return false;
    DRW_Circle wrong = native;
    wrong.eType = DRW::ARC;
    if (DRW_DxfLegacyExtrusion::toNativeOcs(wrong, unchanged)
        || !same(unchanged, before)) return false;
    DRW_Arc arc;
    arc.basePoint = {2.,3.,4.}; arc.extPoint = {0.,0.,-1.};
    arc.radious = 2.; arc.staangle = 0.25; arc.endangle = 1.5;
    arc.haveExtrusion = true;
    const DRW_Arc nativeArc = arc;
    arc.applyExtrusion();
    if (!DRW_DxfLegacyExtrusion::toNativeOcs(arc, arc) || !same(arc, nativeArc))
        return false;
    DRW_Arc invalid = nativeArc;
    invalid.staangle = std::numeric_limits<double>::infinity();
    if (DRW_DxfLegacyExtrusion::toNativeOcs(invalid, arc) || !same(arc, nativeArc))
        return false;
    const DRW_Circle& arcAsCircle = nativeArc;
    if (DRW_DxfLegacyExtrusion::toNativeOcs(arcAsCircle, unchanged)
        || !same(unchanged, before)) return false;
    return true;
}
} // namespace

int main(int argc, char** argv) {
    const bool keep = argc == 2 && std::string(argv[1]) == "--keep";
    if (argc > 2 || (argc == 2 && !keep)) return 2;
    const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto dir = std::filesystem::temp_directory_path()
        / ("libdxfrw-dxf-legacy-extrusion-" + std::to_string(nonce));
    if (!std::filesystem::create_directory(dir)) return 2;
    bool pass = directControls();
    for (bool binary : {false, true}) {
        pass = roundTrip(dir, binary) && pass;
        pass = refusalAndRetry(dir / (binary ? "retry-binary.dxf" : "retry-ascii.dxf"), binary) && pass;
    }
    pass = dwgRoundTrip(dir) && pass;
    pass = dwgRefusalAndRetry(dir / "ac1024-retry.dwg") && pass;
    if (keep) std::cout << "OUTPUT " << dir.string() << '\n';
    else { std::error_code error; std::filesystem::remove_all(dir, error); }
    std::cout << "DXF explicit legacy extrusion conversion " << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
