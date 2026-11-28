// SPDX-License-Identifier: GPL-2.0-or-later
// Optional scratch-only source/target transaction probe. The diagnostic mode
// requires a separately built, explicitly guard-bypassed adapter; it does not
// authorize or change the production export policy. Never commit its output.
#include "dx_iface.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

static std::string bytes(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}
static bool write(dx_iface& adapter, dx_data& data, const std::string& output,
                  bool binary, bool debug = false) {
    return adapter.fileExport(output, DRW::AC1015, binary, &data, debug, false,
        dx_dxfSourceTextPolicy::Compatible, DRW::DxfTextWritePolicy::Compatible,
        dx_dxfProxyGraphicsPolicy::Compatible, dx_dwgReadCompletenessPolicy::AllowPartial);
}
static bool setFault(const char* name, bool enabled) {
#ifdef _WIN32
    return _putenv_s(name, enabled ? "1" : "") == 0;
#else
    return enabled ? setenv(name, "1", 1) == 0 : unsetenv(name) == 0;
#endif
}
int main(int argc, char** argv) {
    if (argc != 4 || (std::string(argv[3]) != "production-refusal"
                      && std::string(argv[3]) != "diagnostic-only"
                      && std::string(argv[3]) != "diagnostic-table-fault"
                      && std::string(argv[3]) != "diagnostic-object-fault"
                      && std::string(argv[3]) != "diagnostic-body-fault"
                      && std::string(argv[3]) != "diagnostic-role-fault"
                      && std::string(argv[3]) != "diagnostic-ucs-fault")) {
        std::cerr << "usage: probe INPUT NEW_SCRATCH_DIRECTORY production-refusal|diagnostic-only|diagnostic-table-fault|diagnostic-object-fault|diagnostic-body-fault|diagnostic-role-fault|diagnostic-ucs-fault\n";
        return 2;
    }
    const bool diagnostic = std::string(argv[3]) != "production-refusal";
    const bool tableFault = std::string(argv[3]) == "diagnostic-table-fault";
    const bool objectFault = std::string(argv[3]) == "diagnostic-object-fault";
    const bool bodyFault = std::string(argv[3]) == "diagnostic-body-fault";
    const bool roleFault = std::string(argv[3]) == "diagnostic-role-fault";
    const bool ucsFault = std::string(argv[3]) == "diagnostic-ucs-fault";
    const char* faultName = ucsFault ? "LIBDXFRW_DIAGNOSTIC_BAD_UCS"
        : roleFault ? "LIBDXFRW_DIAGNOSTIC_BAD_ROLE"
        : tableFault ? "LIBDXFRW_DIAGNOSTIC_BAD_LAYOUT"
        : objectFault ? "LIBDXFRW_DIAGNOSTIC_BAD_OBJECT" : "LIBDXFRW_DIAGNOSTIC_BAD_BODY";
    const auto outputDir = std::filesystem::absolute(argv[2]);
    if (std::filesystem::exists(outputDir) || !std::filesystem::create_directories(outputDir)) return 2;
    dx_iface adapter;
    dx_data source;
    if (!adapter.fileImport(argv[1], &source, false) || source.layouts.empty()
        || (source.sourceVersion != DRW::AC1012 && source.sourceVersion != DRW::AC1014)) return 3;
    const auto originalName = source.layouts.front().name;
    for (const bool binary : {false, true}) {
        const auto out = outputDir / (binary ? "binary.dxf" : "ascii.dxf");
        const std::string sentinel = "diagnostic destination sentinel\n";
        { std::ofstream file(out, std::ios::binary | std::ios::trunc); file << sentinel; }
        if (!diagnostic) {
            if (write(adapter, source, out.string(), binary) || bytes(out) != sentinel) return 4;
            continue;
        }
        if (tableFault || objectFault || bodyFault || roleFault || ucsFault) {
            if (!setFault(faultName, true)) return 2;
        } else {
            source.layouts.front().name += "_changed";
        }
        const bool refused = !write(adapter, source, out.string(), binary, tableFault || objectFault || bodyFault || roleFault || ucsFault)
            && bytes(out) == sentinel;
        if (tableFault || objectFault || bodyFault || roleFault || ucsFault) {
            if (!setFault(faultName, false)) return 2;
        } else {
            source.layouts.front().name = originalName;
        }
        if (!refused || !write(adapter, source, out.string(), binary)) return 4;
        dx_iface reader;
        dx_data output;
        if (!reader.fileImport(out.string(), &output, false) || output.layouts.size() != source.layouts.size()) return 5;
        for (const DRW_Layout& layout : output.layouts) {
            const auto record = std::find_if(output.blockRecords.begin(), output.blockRecords.end(),
                [&layout](const DRW_Block_Record& value) { return value.handle == layout.paperSpaceBlockRecordHandle.ref; });
            if (record == output.blockRecords.end() || record->layoutHandle != layout.handle) return 6;
        }
        if (std::any_of(source.blockRecords.begin(), source.blockRecords.end(),
                [](const DRW_Block_Record& record) { return record.layoutHandle != 0u; })) return 7;
        const auto count = std::count_if(output.dxfClasses.begin(), output.dxfClasses.end(),
            [](const DRW_Class& value) { return value.recName == "LAYOUT" && value.className == "AcDbLayout"; });
        if (count != 1) return 8;
    }
    std::cout << (diagnostic ? "diagnostic output and retry" : "production refusal")
              << ": ASCII and binary PASS\n";
}
