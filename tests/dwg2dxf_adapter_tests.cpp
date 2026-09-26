#include "dx_iface.h"

#include <iostream>
#include <string>

namespace {

constexpr const char* kImagePath = "assets/adapter-image-control.png";

bool fail(const std::string& message) {
    std::cerr << message << '\n';
    return false;
}

bool generate(const std::string& path) {
    dx_data data;
    dx_iface iface;
    iface.cData = &data;
    iface.currentBlock = data.mBlock;

    DRW_Image image;
    image.ref = 0xD700u;
    image.basePoint = DRW_Coord(1.0, 2.0, 3.0);
    image.secPoint = DRW_Coord(5.0, 2.0, 3.0);
    image.vVector = DRW_Coord(0.0, 3.0, 0.0);
    image.sizeu = 640.0;
    image.sizev = 480.0;
    image.clip = 1;
    image.brightness = 60;
    image.contrast = 70;
    image.fade = 10;
    image.m_classVersion = 2;
    iface.addImage(&image);

    DRW_ImageDef definition;
    definition.handle = image.ref;
    definition.name = kImagePath;
    iface.linkImage(&definition);

    if (data.mBlock->ent.size() != 1)
        return fail("generated image entity count is not one");
    auto* stored = dynamic_cast<dx_ifaceImg*>(data.mBlock->ent.front());
    if (stored == nullptr || stored->path != kImagePath)
        return fail("image-definition callback did not update stored entity");
    if (!iface.fileExport(path, DRW::AC1027, false, &data, false))
        return fail("could not export generated image-path DXF control");
    return true;
}

bool verify(const std::string& path) {
    dx_data data;
    dx_iface iface;
    if (!iface.fileImport(path, &data, false))
        return fail("could not read image-path DXF control");
    if (data.mBlock->ent.size() != 1)
        return fail("read-back image entity count is not one");
    auto* stored = dynamic_cast<dx_ifaceImg*>(data.mBlock->ent.front());
    if (stored == nullptr)
        return fail("read-back image entity lost its adapter type");
    if (stored->path != kImagePath) {
        std::cerr << "image path mismatch: expected '" << kImagePath
                  << "', got '" << stored->path << "'\n";
        return false;
    }
    return true;
}

bool generate3DLine(const std::string& path) {
    dx_data data;
    dx_iface iface;
    iface.cData = &data;
    iface.currentBlock = data.mBlock;

    DRW_3DLine line;
    line.basePoint = DRW_Coord(1.25, -2.5, 3.75);
    line.secPoint = DRW_Coord(-4.5, 5.25, -6.75);
    line.extPoint = DRW_Coord(0.0, 1.0, 0.0);
    line.thickness = 2.5;
    iface.add3DLine(line);

    if (data.mBlock->ent.size() != 1
        || dynamic_cast<DRW_3DLine*>(data.mBlock->ent.front()) == nullptr)
        return fail("generated 3DLINE lost its adapter subtype");
    if (!iface.fileExport(path, DRW::AC1027, false, &data, false))
        return fail("could not export generated 3DLINE control");
    return true;
}

bool verify3DLine(const std::string& path) {
    dx_data data;
    dx_iface iface;
    if (!iface.fileImport(path, &data, false))
        return fail("could not read 3DLINE control");
    if (data.mBlock->ent.size() != 1)
        return fail("read-back 3DLINE entity count is not one");
    auto* line = dynamic_cast<DRW_3DLine*>(data.mBlock->ent.front());
    if (line == nullptr || line->eType != DRW::THREEDLINE)
        return fail("read-back entity did not retain its 3DLINE subtype");
    if (line->basePoint.x != 1.25 || line->basePoint.y != -2.5
        || line->basePoint.z != 3.75 || line->secPoint.x != -4.5
        || line->secPoint.y != 5.25 || line->secPoint.z != -6.75
        || line->thickness != 2.5 || line->extPoint.x != 0.0
        || line->extPoint.y != 1.0 || line->extPoint.z != 0.0)
        return fail("read-back 3DLINE fields changed");
    return true;
}

} // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: dwg2dxf_adapter_tests --generate|--verify <file>\n";
        return 2;
    }

    const std::string mode = argv[1];
    const std::string path = argv[2];
    if (mode == "--generate")
        return generate(path) ? 0 : 1;
    if (mode == "--verify")
        return verify(path) ? 0 : 1;
    if (mode == "--generate-3dline")
        return generate3DLine(path) ? 0 : 1;
    if (mode == "--verify-3dline")
        return verify3DLine(path) ? 0 : 1;
    std::cerr << "unknown mode: " << mode << '\n';
    return 2;
}
