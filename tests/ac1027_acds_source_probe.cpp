// Opt-in physical AC1027 AcDs source probe. No drawing fixture is bundled.
#include "dx_iface.h"
#include "drw_datastorage.h"
#include "intern/dwgreader27.h"
#include "libdwgr.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace {
constexpr std::size_t maxSourceBytes = 20u * 1024u * 1024u;
constexpr std::size_t maxSectionBytes = 4u * 1024u * 1024u;

void hexBytes(const std::vector<std::uint8_t>& bytes) {
    for (std::uint8_t value : bytes)
        std::cout << std::hex << std::setfill('0') << std::setw(2)
                  << static_cast<unsigned>(value);
    std::cout << std::dec;
}

class AcdsSectionProbe final : public dwgReader27 {
public:
    using dwgReader27::dwgReader27;
    bool extract(std::vector<std::uint8_t>& bytes) {
        if (!readMetaData() || !readFileHeader()) return false;
        const auto it = sections.find(secEnum::PROTOTYPE);
        if (it == sections.end()) return false;
        std::unique_ptr<std::uint8_t[]> decoded;
        std::uint64_t length = 0;
        if (!parseDataPage(it->second, decoded, length) || !decoded
            || length == 0 || length > maxSectionBytes) return false;
        bytes.assign(decoded.get(), decoded.get() + length);
        return true;
    }
};

const char* modelerName(DRW::ETYPE type) {
    switch (type) {
    case DRW::REGION: return "REGION";
    case DRW::E3DSOLID: return "3DSOLID";
    case DRW::BODY: return "BODY";
    default: return "UNKNOWN";
    }
}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: ac1027_acds_source_probe input.dwg\n";
        return 2;
    }
    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if (!file) return 1;
    const auto length = file.tellg();
    if (length <= 0 || static_cast<std::uint64_t>(length) > maxSourceBytes) return 1;
    file.seekg(0);
    std::vector<std::uint8_t> source(static_cast<std::size_t>(length));
    if (!file.read(reinterpret_cast<char*>(source.data()), length)) return 1;

    dwgRW owner(argv[1]);
    (void)owner.getPreview();
    if (owner.getVersion() != DRW::AC1027) return 1;
    AcdsSectionProbe sourceProbe(
        std::unique_ptr<dwgBuffer>(new dwgBuffer(source.data(), source.size())), &owner);
    std::vector<std::uint8_t> bytes;
    if (!sourceProbe.extract(bytes)) return 1;
    const auto acds = DRW_parseDataStorage(bytes, DRW::AC1027, true);
    if (acds.parseFailed || !acds.structurallyValid || !acds.payloadsRetained
        || acds.records.size() > 10000u || acds.dataIndexEntries.size() > 10000u)
        return 1;
    std::cout << "SECTION " << bytes.size() << ' '; hexBytes(bytes); std::cout << '\n';
    for (const auto& schema : acds.schemas) {
        std::cout << "SCHEMA " << schema.index << ' ' << schema.segmentIndex
                  << ' ' << schema.localOffset;
        for (const auto& p : schema.properties) std::cout << ' ' << p.name << ':' << p.type;
        std::cout << '\n';
    }
    for (const auto& entry : acds.dataIndexEntries)
        std::cout << "INDEX " << entry.schemaIndex << ' ' << entry.segmentIndex
                  << ' ' << entry.localOffset << '\n';
    for (const auto& record : acds.records) {
        if (record.payload.size() > maxSectionBytes) return 1;
        std::cout << "RECORD " << std::hex << record.handle << std::dec << ' '
                  << record.schemaIndex << ' ' << record.segmentIndex << ' '
                  << record.localOffset << ' ' << record.dataOffset << ' '
                  << record.isBlobReference << ' ' << record.payload.size() << ' ';
        hexBytes(record.payload); std::cout << '\n';
    }

    dx_data drawing;
    dx_iface adapter;
    if (!adapter.fileImport(argv[1], &drawing, false)
        || drawing.sourceFormat != dx_source_format::DWG
        || drawing.sourceVersion != DRW::AC1027 || !drawing.mBlock
        || drawing.dataStorageSections.size() != 1u) return 1;
    for (const auto& block : drawing.blockRecords) {
        std::string upperName = block.name;
        std::transform(upperName.begin(), upperName.end(), upperName.begin(),
                       [](unsigned char value) { return static_cast<char>(std::toupper(value)); });
        if (upperName == "*MODEL_SPACE")
            std::cout << "MODEL_BLOCK_RECORD " << std::hex << block.handle << std::dec << '\n';
    }
    std::size_t count = 0;
    const auto emit = [&](const dx_ifaceBlock* block, const char* scope) {
        if (!block) return false;
        for (const auto* entity : block->ent) {
            const auto* modeler = dynamic_cast<const DRW_ModelerGeometry*>(entity);
            if (!modeler) continue;
            if (++count > 10000u || modeler->m_rawBytes.size() > maxSectionBytes) return false;
            const auto* record = acds.findRecordByHandle(modeler->handle);
            const bool exact = record && record->payload == modeler->dataStorageData;
            std::cout << "ENTITY " << scope << ' ' << modelerName(modeler->eType) << ' '
                      << std::hex << modeler->handle << ' ' << modeler->parentHandle
                      << ' ' << modeler->dataStorageHandle << ' ' << modeler->m_historyHandle
                      << std::dec << ' ' << modeler->dataStorageHandleKey << ' '
                      << modeler->hasDataStorageRecord << ' ' << modeler->m_modelerVersion
                      << ' ' << exact << ' ' << modeler->m_rawBytes.size() << ' ';
            hexBytes(modeler->m_rawBytes); std::cout << '\n';
            std::cout << "REVISION_FRAME " << std::hex << modeler->handle
                      << std::dec << ' ' << modeler->m_bodyBitSize << ' ';
            hexBytes(modeler->m_rawBytes); std::cout << '\n';
        }
        return true;
    };
    if (!emit(drawing.mBlock, "MODEL")) return 1;
    for (const auto* block : drawing.blocks) if (!emit(block, "BLOCK")) return 1;
    return std::cout.good() ? 0 : 1;
}
