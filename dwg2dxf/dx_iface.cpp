/******************************************************************************
**  dwg2dxf - Program to convert dwg/dxf to dxf(ascii & binary)              **
**                                                                           **
**  Copyright (C) 2015 José F. Soriano, rallazz@gmail.com                    **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include <iostream>
#include <algorithm>
#include <charconv>
#include <array>
#include <cctype>
#include <functional>
#include <limits>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "dx_iface.h"
#include "libdwgr.h"
#include "libdxfrw.h"
#include "intern/dxfparserlimits.h"

namespace {

DRW::Version sourceVersionFromDxfHeader(
    const DRW_Header& header, DRW::Version fallback) {
    const auto acadVersion = header.vars.find("$ACADVER");
    if (acadVersion == header.vars.end() || acadVersion->second == nullptr
            || acadVersion->second->type() != DRW_Variant::STRING) {
        return fallback;
    }

    const std::string value = acadVersion->second->c_str();
    for (const auto& versionEntry : DRW::dwgVersionStrings) {
        if (versionEntry.first != nullptr && value == versionEntry.first)
            return versionEntry.second;
    }
    return fallback;
}

void addAcdsSchemaHeader(DRW_RawDxfSection& section, int index,
                         const char* schemaName, const char* payloadName) {
    section.m_groups.emplace_back(0, "ACDSSCHEMA");
    section.m_groups.emplace_back(90, index);
    section.m_groups.emplace_back(1, schemaName);
    section.m_groups.emplace_back(2, "AcDbDs::ID");
    section.m_groups.emplace_back(280, 10);
    section.m_groups.emplace_back(91, 8);
    section.m_groups.emplace_back(2, payloadName);
    section.m_groups.emplace_back(280, 15);
    section.m_groups.emplace_back(91, 0);
}

void addAcdsSchemaAttributes(DRW_RawDxfSection& section, int index) {
    section.m_groups.emplace_back(101, "ACDSRECORD");
    section.m_groups.emplace_back(95, index);
    section.m_groups.emplace_back(90, 2);
    section.m_groups.emplace_back(2, "AcDbDs::TreatedAsObjectData");
    section.m_groups.emplace_back(280, 1);
    section.m_groups.emplace_back(291, 1);

    section.m_groups.emplace_back(101, "ACDSRECORD");
    section.m_groups.emplace_back(95, index);
    section.m_groups.emplace_back(90, 3);
    section.m_groups.emplace_back(2, "AcDbDs::Legacy");
    section.m_groups.emplace_back(280, 1);
    section.m_groups.emplace_back(291, 1);

    section.m_groups.emplace_back(101, "ACDSRECORD");
    section.m_groups.emplace_back(1, "AcDbDs::ID");
    section.m_groups.emplace_back(90, 4);
    section.m_groups.emplace_back(2, "AcDs:Indexable");
    section.m_groups.emplace_back(280, 1);
    section.m_groups.emplace_back(291, 1);

    section.m_groups.emplace_back(101, "ACDSRECORD");
    section.m_groups.emplace_back(1, "AcDbDs::ID");
    section.m_groups.emplace_back(90, 5);
    section.m_groups.emplace_back(2, "AcDbDs::HandleAttribute");
    section.m_groups.emplace_back(280, 7);
    section.m_groups.emplace_back(282, 1);
}

void addAcdsMarkerSchema(DRW_RawDxfSection& section, int index,
                         const char* name, const char* member) {
    section.m_groups.emplace_back(0, "ACDSSCHEMA");
    section.m_groups.emplace_back(90, index);
    section.m_groups.emplace_back(1, name);
    section.m_groups.emplace_back(2, member);
    section.m_groups.emplace_back(280, 1);
    section.m_groups.emplace_back(91, 0);
}

bool hasSabSignature(const std::vector<std::uint8_t>& payload) {
    static constexpr char signature[] = "ACIS BinaryFile";
    return payload.size() >= sizeof(signature) - 1u
        && std::equal(std::begin(signature), std::end(signature) - 1,
                      payload.begin(), [](char expected, std::uint8_t actual) {
                          return static_cast<std::uint8_t>(expected) == actual;
                      });
}

bool matchesDataStorageSchemaProperty(
    const DRW_DataStorageSchemaProperty& property, std::uint32_t nameIndex,
    const char* name, std::uint32_t type, std::uint32_t flags,
    std::uint16_t valueCount,
    const std::vector<std::vector<std::uint8_t>>& values) {
    return name != nullptr && property.nameIndex == nameIndex
        && property.name == name && property.type == type
        && property.flags == flags && property.valueCount == valueCount
        && property.values == values;
}

// The DXF ACDSSCHEMA writer below reconstructs one observed AC1027 table.
// Restrict this bridge to the matching DWG DataStorage schema fingerprint;
// a six-schema count alone must never relabel a different property layout.
// This is a sample-qualified guard, not a general DataStorage schema codec.
bool matchesQualifiedAc1027AcdsSchemas(
    const DRW_DataStorageSection& storage) {
    try {
    static const std::array<const char*, 7> expectedPropertyNames = {
        "AcDbDs::ID", "Thumbnail_Data", "ASM_Data",
        "AcDbDs::TreatedAsObjectData", "AcDbDs::Legacy",
        "AcDs:Indexable", "AcDbDs::HandleAttribute"};
    if (storage.schemaCount != 6u || storage.schemas.size() != 6u
        || storage.schemaPropertyNameCount != expectedPropertyNames.size()
        || storage.schemaPropertyNames.size() != expectedPropertyNames.size()) {
        return false;
    }
    for (std::size_t index = 0; index < expectedPropertyNames.size(); ++index) {
        if (storage.schemaPropertyNames[index] != expectedPropertyNames[index])
            return false;
    }

    std::array<const DRW_DataStorageSchema*, 6> schemasByIndex{};
    for (const DRW_DataStorageSchema& schema : storage.schemas) {
        if (schema.index >= schemasByIndex.size()
            || schemasByIndex[schema.index] != nullptr) {
            return false;
        }
        schemasByIndex[schema.index] = &schema;
    }
    if (!std::all_of(schemasByIndex.begin(), schemasByIndex.end(),
                     [](const DRW_DataStorageSchema* schema) {
                         return schema != nullptr;
                     })) {
        return false;
    }
    const auto propertyMatches = [](const DRW_DataStorageSchema& schema,
                                    std::size_t propertyIndex,
                                    std::uint32_t nameIndex,
                                    const char* name,
                                    std::uint32_t type,
                                    std::uint32_t flags,
                                    std::uint16_t valueCount,
                                    const std::vector<std::vector<std::uint8_t>>&
                                        values) {
        return propertyIndex < schema.properties.size()
            && matchesDataStorageSchemaProperty(
                schema.properties[propertyIndex], nameIndex, name, type, flags,
                valueCount, values);
    };
    const std::vector<std::vector<std::uint8_t>> idValues0 = {
        {6u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
        {7u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}};
    const std::vector<std::vector<std::uint8_t>> idValues1 = {
        {2u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
        {3u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}};
    const std::vector<std::vector<std::uint8_t>> handleValue = {{0u}};
    const auto& thumbnail = *schemasByIndex[0];
    const auto& asmData = *schemasByIndex[1];
    const auto& treatedAsObjectData = *schemasByIndex[2];
    const auto& legacy = *schemasByIndex[3];
    const auto& indexable = *schemasByIndex[4];
    const auto& handleAttribute = *schemasByIndex[5];
    return thumbnail.indexes == std::vector<std::uint64_t>{4u, 5u}
        && thumbnail.properties.size() == 2u
        && propertyMatches(thumbnail, 0u, 0u, "AcDbDs::ID", 10u, 0u, 2u,
                           idValues0)
        && propertyMatches(thumbnail, 1u, 1u, "Thumbnail_Data", 15u, 0u, 0u,
                           {})
        && asmData.indexes == std::vector<std::uint64_t>{0u, 1u}
        && asmData.properties.size() == 2u
        && propertyMatches(asmData, 0u, 0u, "AcDbDs::ID", 10u, 0u, 2u,
                           idValues1)
        && propertyMatches(asmData, 1u, 2u, "ASM_Data", 15u, 0u, 0u, {})
        && treatedAsObjectData.indexes.empty()
        && treatedAsObjectData.properties.size() == 1u
        && propertyMatches(treatedAsObjectData, 0u, 3u,
                           "AcDbDs::TreatedAsObjectData", 1u, 0u, 0u, {})
        && legacy.indexes.empty() && legacy.properties.size() == 1u
        && propertyMatches(legacy, 0u, 4u, "AcDbDs::Legacy", 1u, 0u, 0u, {})
        && indexable.indexes.empty() && indexable.properties.size() == 1u
        && propertyMatches(indexable, 0u, 5u, "AcDs:Indexable", 1u, 0u, 0u,
                           {})
        && handleAttribute.indexes.empty()
        && handleAttribute.properties.size() == 1u
        && propertyMatches(handleAttribute, 0u, 6u,
                           "AcDbDs::HandleAttribute", 7u, 8u, 1u,
                           handleValue);
    } catch (...) {
        return false;
    }
}

std::string proxyHandleString(std::uint64_t handle) {
    std::array<char, 17> digits{};
    const auto converted = std::to_chars(digits.data(), digits.data() + 16,
                                         handle, 16);
    if (converted.ec != std::errc())
        return {};
    std::string value(digits.data(), converted.ptr);
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char ch) {
                       return static_cast<char>(std::toupper(ch));
                   });
    return value;
}

bool appendProxyBinaryGroups(DRW_RawDxfObject& raw, int code,
                             const std::vector<std::uint8_t>& bytes) {
    constexpr std::size_t bytesPerChunk = 127u;
    for (std::size_t offset = 0; offset < bytes.size();
         offset += bytesPerChunk) {
        const std::size_t count = std::min(bytesPerChunk,
                                           bytes.size() - offset);
        static constexpr char hex[] = "0123456789ABCDEF";
        std::string chunk;
        chunk.reserve(count * 2u);
        for (std::size_t index = 0; index < count; ++index) {
            const std::uint8_t byte = bytes[offset + index];
            chunk.push_back(hex[byte >> 4u]);
            chunk.push_back(hex[byte & 0x0Fu]);
        }
        raw.groups.emplace_back(code, std::move(chunk));
    }
    return true;
}

bool makeRawProxyObject(const DRW_ProxyObject& proxy,
                        DRW_RawDxfObject& raw) {
    const auto expectedBytes = [](std::uint64_t bits) {
        return bits / 8u + ((bits & 7u) != 0 ? 1u : 0u);
    };
    if (proxy.handle == 0 || proxy.parentHandle == 0
        || !proxy.m_hasProxyCarrierId || !proxy.m_hasProxyClassId
        || !proxy.m_hasProxyDrawingFormat
        || !proxy.m_hasObjectDataBitSize
        || proxy.m_objectDataBitSize
               > DRW::kMaxDxfBinaryPayloadBytes * 8u
        || expectedBytes(proxy.m_objectDataBitSize)
               != proxy.m_objectData.size()
        || (proxy.m_hasProxyGraphicsByteSize
            && (proxy.m_proxyGraphicsByteSize
                    != proxy.m_binaryData.size()
                || proxy.m_proxyGraphicsByteSize
                       > DRW::kMaxDxfBinaryPayloadBytes))
        || (!proxy.m_hasProxyGraphicsByteSize
            && !proxy.m_binaryData.empty())
        || (proxy.m_hasUnknownDataByteSize
            && (proxy.m_unknownDataByteSize
                    != proxy.m_unknownData.size()
                || proxy.m_unknownDataByteSize
                       > DRW::kMaxDxfBinaryPayloadBytes))
        || (!proxy.m_hasUnknownDataByteSize
            && !proxy.m_unknownData.empty())) {
        return false;
    }

    raw = DRW_RawDxfObject{};
    raw.name = "ACAD_PROXY_OBJECT";
    raw.handle = proxy.handle;
    raw.parentHandle = proxy.parentHandle;
    raw.groups.emplace_back(5, proxyHandleString(proxy.handle));
    raw.groups.emplace_back(330, proxyHandleString(proxy.parentHandle));
    if (!proxy.reactorHandles.empty()) {
        raw.groups.emplace_back(102, "{ACAD_REACTORS");
        for (std::uint32_t reactor : proxy.reactorHandles) {
            if (reactor == 0)
                return false;
            raw.groups.emplace_back(330, proxyHandleString(reactor));
        }
        raw.groups.emplace_back(102, "}");
    }
    if (proxy.xDictHandle != 0) {
        raw.groups.emplace_back(102, "{ACAD_XDICTIONARY");
        raw.groups.emplace_back(360, proxyHandleString(proxy.xDictHandle));
        raw.groups.emplace_back(102, "}");
    }
    raw.groups.emplace_back(100, "AcDbProxyObject");
    raw.groups.emplace_back(90, proxy.m_proxyCarrierId);
    raw.groups.emplace_back(91, proxy.m_proxyClassId);
    if (proxy.m_hasProxyGraphicsByteSize) {
        if (proxy.m_proxyGraphicsByteSize
            > static_cast<std::uint64_t>(
                  std::numeric_limits<std::int32_t>::max())) {
            return false;
        }
        raw.groups.emplace_back(92, static_cast<std::int32_t>(
            proxy.m_proxyGraphicsByteSize));
        if (!appendProxyBinaryGroups(raw, 310, proxy.m_binaryData))
            return false;
    }
    if (proxy.m_proxyDrawingFormat
        > static_cast<std::uint32_t>(
              std::numeric_limits<std::int32_t>::max())) {
        return false;
    }
    raw.groups.emplace_back(95, static_cast<std::int32_t>(
        proxy.m_proxyDrawingFormat));
    raw.groups.emplace_back(70,
        proxy.m_hasFromDxf && proxy.m_fromDxf ? 1 : 0);
    raw.groups.emplace_back(161,
        static_cast<std::int64_t>(proxy.m_objectDataBitSize));
    if (!appendProxyBinaryGroups(raw, 310, proxy.m_objectData))
        return false;
    if (proxy.m_hasUnknownDataByteSize) {
        raw.groups.emplace_back(162, static_cast<std::int64_t>(
            proxy.m_unknownDataByteSize));
        if (!appendProxyBinaryGroups(raw, 310, proxy.m_unknownData))
            return false;
    }
    for (const DRW_ProxyObjectIdRef& reference : proxy.m_objectIdRefs) {
        if ((reference.m_dxfCode != 330 && reference.m_dxfCode != 340
             && reference.m_dxfCode != 350 && reference.m_dxfCode != 360)
            || reference.m_handle == 0) {
            return false;
        }
        raw.groups.emplace_back(reference.m_dxfCode,
                                proxyHandleString(reference.m_handle));
    }
    return true;
}

bool collectAcdsHistoryProxyGraph(
    const dx_data& data, const DRW_ModelerGeometry& modeler,
    std::vector<DRW_RawDxfObject>& output,
    std::vector<std::uint32_t>& materialHandles) {
    if (modeler.m_historyHandle == 0 || modeler.handle == 0)
        return false;

    std::unordered_map<std::uint32_t, std::vector<const DRW_ProxyObject*>> byHandle;
    for (const DRW_ProxyObject& proxy : data.proxyObjects)
        byHandle[proxy.handle].push_back(&proxy);

    std::vector<std::pair<std::uint32_t, std::uint32_t>> pending;
    std::unordered_map<std::uint32_t, std::uint32_t> expectedOwner;
    std::unordered_set<std::uint32_t> visited;
    pending.emplace_back(modeler.m_historyHandle, modeler.handle);
    try {
        while (!pending.empty()) {
            if (output.size() >= 3u)
                return false;
            const auto current = pending.back();
            pending.pop_back();
            const std::uint32_t handle = current.first;
            const auto priorOwner = expectedOwner.emplace(handle, current.second);
            if (!priorOwner.second && priorOwner.first->second != current.second)
                return false;
            if (!visited.insert(handle).second)
                continue;

            const auto found = byHandle.find(handle);
            if (found == byHandle.end() || found->second.size() != 1u
                || found->second.front()->parentHandle != current.second) {
                return false;
            }
            const DRW_ProxyObject& proxy = *found->second.front();
            const std::int32_t expectedClassIds[] = {521, 520, 519};
            const char* expectedSubclasses[] = {
                "cn:AcDbShHistory", "cn:AcDbEvalGraph", "cn:AcDbShCone"};
            if (!proxy.m_hasProxyClassId
                || proxy.m_proxyClassId != expectedClassIds[output.size()]
                || proxy.m_proxySubclass != expectedSubclasses[output.size()]
                || proxy.m_objectIdRefs.size() != 1u) {
                return false;
            }

            DRW_RawDxfObject raw;
            if (!makeRawProxyObject(proxy, raw)) {
                return false;
            }
            output.push_back(std::move(raw));

            const DRW_ProxyObjectIdRef& reference =
                proxy.m_objectIdRefs.front();
            if (reference.m_handle == 0
                || reference.m_handle
                       > std::numeric_limits<std::uint32_t>::max()) {
                return false;
            }
            const std::uint32_t target =
                static_cast<std::uint32_t>(reference.m_handle);
            if (output.size() < 3u) {
                if (reference.m_dxfCode != 340
                    || byHandle.find(target) == byHandle.end()) {
                    return false;
                }
                pending.emplace_back(target, handle);
                continue;
            }

            if (reference.m_dxfCode != 360
                || byHandle.find(target) != byHandle.end())
                return false;
            std::size_t materialMatches = 0;
            for (const DRW_Material& material : data.materials) {
                if (material.handle == target)
                    ++materialMatches;
            }
            if (materialMatches != 1u)
                return false;
            materialHandles.push_back(target);
        }
    } catch (...) {
        return false;
    }
    return output.size() == 3u && materialHandles.size() == 1u;
}

bool configureAcdsMaterialDictionary(
    const dx_data& data, const std::vector<std::uint32_t>& referencedMaterials,
    std::vector<DRW_Dictionary>& namedDictionaries,
    std::vector<std::pair<std::string, std::string>>& rootEntries,
    std::vector<std::uint32_t>& materialHandles) {
    if (referencedMaterials.empty())
        return true;
    std::unordered_set<std::uint32_t> required(referencedMaterials.begin(),
                                                referencedMaterials.end());
    const DRW_Dictionary* materialDictionary = nullptr;
    for (const DRW_Dictionary& dictionary : data.dictionaries) {
        // R2010+ DWG readers do not publish the implicit NamedObjectsDictionary
        // body (handle C). Reconstruct only ACAD_MATERIAL when one complete
        // root-owned dictionary has entries that all resolve uniquely to
        // typed MATERIAL objects and includes every history reference.
        if (dictionary.handle == 0 || dictionary.handle == 0xCu
            || dictionary.handle == 0xDu || dictionary.parentHandle != 0xCu
            || !dictionary.hasCompleteDwgEntries()
            || dictionary.m_entries.empty()) {
            continue;
        }

        std::unordered_set<std::uint32_t> entries;
        bool matchesMaterials = true;
        for (const DRW_Dictionary::Entry& entry : dictionary.m_entries) {
            if (entry.m_handle == 0 || !entries.insert(entry.m_handle).second) {
                matchesMaterials = false;
                break;
            }
            std::size_t matches = 0;
            for (const DRW_Material& material : data.materials) {
                if (material.handle == entry.m_handle)
                    ++matches;
            }
            if (matches != 1u) {
                matchesMaterials = false;
                break;
            }
        }
        if (!matchesMaterials
            || !std::all_of(required.begin(), required.end(),
                            [&entries](std::uint32_t handle) {
                                return entries.count(handle) != 0;
                            })) {
            continue;
        }
        if (materialDictionary != nullptr)
            return false;
        materialDictionary = &dictionary;
    }
    if (materialDictionary == nullptr)
        return false;

    for (const DRW_Dictionary::Entry& entry : materialDictionary->m_entries)
        materialHandles.push_back(entry.m_handle);
    namedDictionaries.push_back(*materialDictionary);
    rootEntries.emplace_back("ACAD_MATERIAL",
                             proxyHandleString(materialDictionary->handle));
    return true;
}

bool appendAcdsDataSection(
    const dx_data& data, DRW::Version outputVersion,
    std::vector<DRW_RawDxfSection>& sections) {
    bool hasExistingAcdsData = false;
    for (const DRW_RawDxfSection& section : sections) {
        if (section.m_name == "ACDSDATA"
            || section.m_name == "acdsdata") {
            hasExistingAcdsData = true;
            break;
        }
    }

    const DRW_ModelerGeometry* modeler = nullptr;
    std::size_t linkedEntityCount = 0;
    const auto inspectBlock = [&](const dx_ifaceBlock* block) {
        if (block == nullptr)
            return true;
        for (const DRW_Entity* entity : block->ent) {
            if (entity == nullptr)
                continue;
            const bool carriesDataStorage = entity->hasDataStorageRecord
                || entity->hasDataStorageBinaryData()
                || !entity->dataStorageData.empty();
            if (!carriesDataStorage)
                continue;
            ++linkedEntityCount;
            if (entity->eType != DRW::E3DSOLID)
                return false;
            modeler = static_cast<const DRW_ModelerGeometry*>(entity);
        }
        return true;
    };

    if (!inspectBlock(data.mBlock))
        return false;
    for (const dx_ifaceBlock* block : data.blocks) {
        if (block == data.mBlock)
            continue;
        if (!inspectBlock(block))
            return false;
    }

    if (data.dataStorageSections.empty())
        return linkedEntityCount == 0;
    if (data.dataStorageSections.size() != 1u)
        return false;
    const DRW_DataStorageSection& storage = data.dataStorageSections.front();
    if (storage.records.empty() && linkedEntityCount == 0)
        return true;
    if (hasExistingAcdsData || outputVersion != DRW::AC1027
        || storage.m_name != "AcDb:AcDsPrototype_1b"
        || storage.m_version != DRW::AC1027 || storage.parseFailed
        || !storage.structurallyValid || !storage.replayAllowed
        || !storage.payloadsRetained
        || !matchesQualifiedAc1027AcdsSchemas(storage)
        || storage.records.size() != 1u
        || !storage.duplicateRecordHandleKeys.empty()
        || storage.orphanRecordCount != 0u || linkedEntityCount != 1u
        || modeler == nullptr || modeler->eType != DRW::E3DSOLID
        || !modeler->hasDataStorageBinaryData()
        || !modeler->hasDataStorageRecord || modeler->m_isEmpty
        || !modeler->m_hasModelerData || modeler->m_dwgAcisPayload.size() != 0u
        || modeler->dataStorageSchemaIndex != 1u
        || modeler->dataStorageData.empty()
        || modeler->dataStorageData.size() > 8u * 1024u * 1024u
        || !hasSabSignature(modeler->dataStorageData)) {
        return false;
    }

    std::vector<DRW_RawDxfObject> historyObjects;
    std::vector<std::uint32_t> referencedMaterials;
    if (!collectAcdsHistoryProxyGraph(data, *modeler, historyObjects,
                                      referencedMaterials)) {
        return false;
    }
    std::vector<DRW_Dictionary> materialDictionaries;
    std::vector<std::pair<std::string, std::string>> rootEntries;
    std::vector<std::uint32_t> materialHandles;
    if (!configureAcdsMaterialDictionary(data, referencedMaterials,
                                         materialDictionaries, rootEntries,
                                         materialHandles)) {
        return false;
    }

    const DRW_DataStorageRecord& record = storage.records.front();
    if (!record.isHandleSafe || record.isBlobReference
        || record.schemaIndex != 1u || record.handle != modeler->handle
        || record.handle != modeler->dataStorageHandle
        || record.handleKey.empty()
        || record.handleKey != modeler->dataStorageHandleKey
        || record.payload != modeler->dataStorageData
        || record.dataByteLength != record.payload.size()
        || !record.hasPayloadMarker || record.payloadMarkerLength == 0u
        || record.payloadMarkerOffset > record.payload.size()
        || record.payloadMarkerLength
               > record.payload.size() - record.payloadMarkerOffset) {
        return false;
    }

    DRW_RawDxfSection section;
    section.m_name = "ACDSDATA";
    section.m_version = DRW::AC1027;
    section.m_groups.emplace_back(70, 2);
    section.m_groups.emplace_back(71, 2);

    addAcdsSchemaHeader(section, 0, "AcDb_Thumbnail_Schema",
                        "Thumbnail_Data");
    addAcdsSchemaAttributes(section, 0);
    addAcdsSchemaHeader(section, 1, "AcDb3DSolid_ASM_Data", "ASM_Data");
    addAcdsSchemaAttributes(section, 1);
    addAcdsMarkerSchema(section, 2,
                        "AcDbDs::TreatedAsObjectDataSchema",
                        "AcDbDs::TreatedAsObjectData");
    addAcdsMarkerSchema(section, 3, "AcDbDs::LegacySchema",
                        "AcDbDs::Legacy");
    addAcdsMarkerSchema(section, 4, "AcDbDs::IndexedPropertySchema",
                        "AcDs:Indexable");
    section.m_groups.emplace_back(0, "ACDSSCHEMA");
    section.m_groups.emplace_back(90, 5);
    section.m_groups.emplace_back(1, "AcDbDs::HandleAttributeSchema");
    section.m_groups.emplace_back(2, "AcDbDs::HandleAttribute");
    section.m_groups.emplace_back(280, 7);
    section.m_groups.emplace_back(91, 1);
    section.m_groups.emplace_back(284, 1);

    section.m_groups.emplace_back(0, "ACDSRECORD");
    section.m_groups.emplace_back(90, 1);
    section.m_groups.emplace_back(2, "AcDbDs::ID");
    section.m_groups.emplace_back(280, 10);
    section.m_groups.emplace_back(320, record.handleKey);
    section.m_groups.emplace_back(2, "ASM_Data");
    section.m_groups.emplace_back(280, 15);
    section.m_groups.emplace_back(94,
        static_cast<std::int32_t>(record.payload.size()));
    static constexpr char hexDigits[] = "0123456789ABCDEF";
    constexpr std::size_t bytesPerChunk = 127u;
    for (std::size_t offset = 0; offset < record.payload.size();
         offset += bytesPerChunk) {
        const std::size_t count = std::min(
            bytesPerChunk, record.payload.size() - offset);
        std::string hex;
        hex.reserve(count * 2u);
        for (std::size_t index = 0; index < count; ++index) {
            const std::uint8_t value = record.payload[offset + index];
            hex.push_back(hexDigits[value >> 4u]);
            hex.push_back(hexDigits[value & 0x0fu]);
        }
        section.m_groups.emplace_back(310, std::move(hex));
    }
    sections.push_back(std::move(section));
    return true;
}

} // namespace

bool dx_iface::collectAcdsHistoryProxyObjects(
    const dx_data& data, std::vector<DRW_RawDxfObject>& objects,
    std::vector<std::uint32_t>& materialHandles) {
    if (data.dataStorageSections.empty())
        return true;
    const DRW_ModelerGeometry* linkedModeler = nullptr;
    std::size_t linkedCount = 0;
    const auto inspect = [&](const dx_ifaceBlock* block) {
        if (block == nullptr)
            return true;
        for (const DRW_Entity* entity : block->ent) {
            if (entity == nullptr
                || (!entity->hasDataStorageRecord
                    && !entity->hasDataStorageBinaryData()
                    && entity->dataStorageData.empty())) {
                continue;
            }
            if (entity->eType != DRW::E3DSOLID)
                return false;
            ++linkedCount;
            linkedModeler = static_cast<const DRW_ModelerGeometry*>(entity);
        }
        return true;
    };
    if (!inspect(data.mBlock))
        return false;
    for (const dx_ifaceBlock* block : data.blocks) {
        if (block != data.mBlock && !inspect(block))
            return false;
    }
    if (linkedCount != 1u || linkedModeler == nullptr)
        return false;
    if (!collectAcdsHistoryProxyGraph(data, *linkedModeler, objects,
                                      materialHandles)) {
        return false;
    }
    return true;
}

bool dx_iface::prepareExtensionObjectGraph(dx_data* data) {
    m_extensionDictionaryHandles.clear();
    m_extensionXRecordHandles.clear();
    if (data == nullptr)
        return false;

    try {
        std::unordered_map<std::uint32_t, const DRW_Dictionary*> dictionaries;
        std::unordered_map<std::uint32_t, const DRW_XRecord*> xrecords;
        std::unordered_set<std::uint32_t> sourceHandles;
        std::unordered_set<std::uint32_t> linetypeHandles;
        for (const DRW_LType& lineType : data->lineTypes) {
            if (lineType.handle == 0)
                continue;
            if (!sourceHandles.insert(lineType.handle).second)
                return false;
            linetypeHandles.insert(lineType.handle);
        }
        for (const DRW_Dictionary& dictionary : data->dictionaries) {
            if (dictionary.handle == 0)
                continue;
            if (!sourceHandles.insert(dictionary.handle).second
                || !dictionaries.emplace(dictionary.handle, &dictionary).second)
                return false;
        }
        for (const DRW_XRecord& record : data->xRecords) {
            if (record.handle == 0)
                continue;
            if (!sourceHandles.insert(record.handle).second
                || !xrecords.emplace(record.handle, &record).second)
                return false;
        }

        std::unordered_set<std::uint32_t> visiting;
        std::unordered_set<std::uint32_t> visitedDictionaries;
        std::unordered_set<std::uint32_t> visitedXRecords;
        std::function<bool(std::uint32_t)> visitDictionary;
        std::function<bool(std::uint32_t)> visitXRecord;
        visitXRecord = [&](std::uint32_t handle) {
            const auto found = xrecords.find(handle);
            if (found == xrecords.end())
                return false;
            if (visiting.count(handle) != 0)
                return false;
            if (visitedXRecords.count(handle) != 0)
                return true;
            if (found->second->m_rawDataValid
                || (!found->second->m_handleValues.empty()
                    && std::any_of(found->second->m_handleValues.cbegin(),
                                   found->second->m_handleValues.cend(),
                                   [](const auto& value) {
                                       return value.first == 0;
                                   })))
                return false;
            visiting.insert(handle);
            m_extensionXRecordHandles.push_back(handle);
            if (found->second->xDictHandle != 0
                && !visitDictionary(found->second->xDictHandle)) {
                visiting.erase(handle);
                return false;
            }
            for (const std::uint32_t reactor : found->second->reactorHandles) {
                if (reactor == 0) {
                    visiting.erase(handle);
                    return false;
                }
                if (linetypeHandles.count(reactor) != 0)
                    continue;
                // Reactors are non-owning back-references; an object already
                // on the containment walk is present in the output closure.
                if (visiting.count(reactor) != 0)
                    continue;
                if (dictionaries.count(reactor) != 0) {
                    if (!visitDictionary(reactor)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else if (xrecords.count(reactor) == 0
                           || !visitXRecord(reactor)) {
                    visiting.erase(handle);
                    return false;
                }
            }
            visiting.erase(handle);
            visitedXRecords.insert(handle);
            return true;
        };
        visitDictionary = [&](std::uint32_t handle) {
            const auto found = dictionaries.find(handle);
            if (found == dictionaries.end() || handle == 0xCu || handle == 0xDu)
                return false;
            if (visiting.count(handle) != 0)
                return false;
            if (visitedDictionaries.count(handle) != 0)
                return true;
            if (!visiting.insert(handle).second)
                return false;
            if (!found->second->hasCompleteDwgEntries()
                && !found->second->hasCompleteDxfEntries()) {
                visiting.erase(handle);
                return false;
            }
            m_extensionDictionaryHandles.push_back(handle);
            if (found->second->xDictHandle != 0
                && !visitDictionary(found->second->xDictHandle)) {
                visiting.erase(handle);
                return false;
            }
            for (const std::uint32_t reactor : found->second->reactorHandles) {
                if (reactor == 0) {
                    visiting.erase(handle);
                    return false;
                }
                if (linetypeHandles.count(reactor) != 0
                    || visiting.count(reactor) != 0)
                    continue;
                if (dictionaries.count(reactor) != 0) {
                    if (!visitDictionary(reactor)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else if (xrecords.count(reactor) == 0
                           || !visitXRecord(reactor)) {
                    visiting.erase(handle);
                    return false;
                }
            }
            for (const DRW_Dictionary::Entry& entry : found->second->m_entries) {
                if (entry.m_handle == 0)
                    return false;
                if (dictionaries.count(entry.m_handle) != 0) {
                    if (!visitDictionary(entry.m_handle)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else if (xrecords.count(entry.m_handle) != 0) {
                    if (!visitXRecord(entry.m_handle)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else {
                    visiting.erase(handle);
                    return false;
                }
            }
            visiting.erase(handle);
            visitedDictionaries.insert(handle);
            return true;
        };

        for (const DRW_LType& lineType : data->lineTypes) {
            if (lineType.xDictHandle != 0
                && !visitDictionary(lineType.xDictHandle))
                return false;
            for (const std::uint32_t reactor : lineType.reactorHandles) {
                if (reactor == 0)
                    return false;
                if (linetypeHandles.count(reactor) != 0
                    || visiting.count(reactor) != 0)
                    continue;
                if (dictionaries.count(reactor) != 0) {
                    if (!visitDictionary(reactor))
                        return false;
                } else if (xrecords.count(reactor) == 0
                           || !visitXRecord(reactor)) {
                    return false;
                }
            }
        }

        const auto hasParent = [&dictionaries, &visitedDictionaries,
                                &visitedXRecords, &linetypeHandles](
                                   std::uint32_t parent) {
            return parent == 0 || parent == 0xCu || parent == 0xDu
                || linetypeHandles.count(parent) != 0
                || (dictionaries.count(parent) != 0
                    && visitedDictionaries.count(parent) != 0)
                || visitedXRecords.count(parent) != 0;
        };
        for (std::uint32_t handle : m_extensionDictionaryHandles) {
            const DRW_Dictionary* dictionary = dictionaries.at(handle);
            if (!hasParent(dictionary->parentHandle))
                return false;
        }
        for (std::uint32_t handle : m_extensionXRecordHandles) {
            const DRW_XRecord* record = xrecords.at(handle);
            if (!hasParent(record->parentHandle))
                return false;
            const auto isHandleCode = [](int code) {
                return (code >= 320 && code <= 369)
                    || (code >= 390 && code <= 399)
                    || (code >= 480 && code <= 481)
                    || code == 5 || code == 105
                    || (code >= 1005 && code <= 1009);
            };
            const auto validateHandleTarget =
                [&isHandleCode, &linetypeHandles, &visitedDictionaries,
                 &visitedXRecords](int code, std::uint64_t target) {
                    if (!isHandleCode(code))
                        return true;
                    if (target > std::numeric_limits<std::uint32_t>::max())
                        return false;
                    const std::uint32_t targetHandle =
                        static_cast<std::uint32_t>(target);
                    return targetHandle == 0
                        || linetypeHandles.count(targetHandle) != 0
                        || visitedDictionaries.count(targetHandle) != 0
                        || visitedXRecords.count(targetHandle) != 0;
                };
            const auto validateHandleValue =
                [&isHandleCode, &validateHandleTarget](
                    const DRW_Variant& value) {
                    const int code = value.code();
                    if (!isHandleCode(code))
                        return true;
                    std::uint64_t target = 0;
                    if (value.type() == DRW_Variant::INTEGER) {
                        if (value.i_val() < 0)
                            return false;
                        target = static_cast<std::uint32_t>(value.i_val());
                    } else if (value.type() == DRW_Variant::INTEGER64) {
                        if (value.i64_val() < 0)
                            return false;
                        target = static_cast<std::uint64_t>(value.i64_val());
                    } else if (value.type() == DRW_Variant::STRING) {
                        const char* text = value.c_str();
                        if (text == nullptr || *text == '\0')
                            return false;
                        const char* end = text
                            + std::char_traits<char>::length(text);
                        const auto parsed = std::from_chars(text, end, target, 16);
                        if (parsed.ec != std::errc{} || parsed.ptr != end)
                            return false;
                    } else {
                        return false;
                    }
                    return validateHandleTarget(code, target);
                };
            if (!record->m_dataEntries.empty()) {
                for (const DRW_Variant& value : record->m_dataEntries) {
                    if (!validateHandleValue(value))
                        return false;
                }
            } else {
                for (const DRW_Variant& value : record->m_values) {
                    if (!validateHandleValue(value))
                        return false;
                }
                for (const auto& value : record->m_handleValues) {
                    if (!validateHandleTarget(value.first, value.second))
                        return false;
                }
            }
        }
        return true;
    } catch (...) {
        m_extensionDictionaryHandles.clear();
        m_extensionXRecordHandles.clear();
        return false;
    }
}


bool dx_iface::fileImport(const std::string& fileI, dx_data *fData, bool debug){
    unsigned int found = fileI.find_last_of(".");
    std::string fileExt = fileI.substr(found+1);
    std::transform(fileExt.begin(), fileExt.end(),fileExt.begin(), ::toupper);
    cData = fData;
    currentBlock = cData->mBlock;

    if (fileExt == "DXF"){
        //loads dxf
        dxfRW* dxf = new dxfRW(fileI.c_str());
        if (debug) {
            dxf->setDebug(DRW::DebugLevel::Debug);
        }
        bool success = dxf->read(this, false);
        if (success)
            fData->sourceVersion = sourceVersionFromDxfHeader(
                fData->headerC, dxf->getVersion());
        if (!success) {
            std::cout << "DXF file error: format " << dxf->getVersion() << " error " << dxf->getError() << std::endl;
        }
        delete dxf;
        return success;
    } else if (fileExt == "DWG"){
        //loads dwg
        dwgR* dwg = new dwgR(fileI.c_str());
        if (debug) {
            dwg->setDebug(DRW::DebugLevel::Debug);
        }
        bool success = dwg->read(this, false);
        if (success)
            fData->sourceVersion = dwg->getVersion();
        if (!success) {
            std::cout << "DWG file error: format " << dwg->getVersion() << " error " << dwg->getError() << std::endl;
        }
        delete dwg;
        return success;
    }
    std::cout << "file extension can be dxf or dwg" << std::endl;
    return false;
}

bool dx_iface::fileExport(const std::string& file, DRW::Version v, bool binary, dx_data *fData, bool debug){
    cData = fData;
    m_acdsHistoryObjects.clear();
    m_acdsMaterialHandles.clear();
    if (!prepareExtensionObjectGraph(cData))
        return false;
    dxfW = new dxfRW(file.c_str());
    std::vector<DRW_LType> lineTypes(cData->lineTypes.begin(),
                                     cData->lineTypes.end());
    dxfW->setCanonicalLineTypeMetadata(lineTypes);
    std::vector<DRW_RawDxfSection> rawDxfSections = cData->rawDxfSections;
    if (!appendAcdsDataSection(*cData, v, rawDxfSections)) {
        delete dxfW;
        dxfW = nullptr;
        return false;
    }
    std::vector<std::uint32_t> referencedMaterials;
    std::vector<DRW_Dictionary> materialDictionaries;
    std::vector<std::pair<std::string, std::string>> materialRootEntries;
    if (!cData->dataStorageSections.empty()) {
        if (!collectAcdsHistoryProxyObjects(*cData, m_acdsHistoryObjects,
                                            referencedMaterials)
            || !configureAcdsMaterialDictionary(
                *cData, referencedMaterials, materialDictionaries,
                materialRootEntries, m_acdsMaterialHandles)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
        dxfW->setNamedDictObjects(materialDictionaries);
        dxfW->setRootDictEntries(materialRootEntries);
    }
    dxfW->setRawDxfSections(rawDxfSections);
    if (debug) {
        dxfW->setDebug(DRW::DebugLevel::Debug);
    }
    const std::set<std::uint32_t> fixedHandles = {
        0x1u, 0x2u, 0x3u, 0x5u, 0x6u, 0x7u, 0x8u, 0x9u, 0xAu,
        0xCu, 0xDu, 0x10u, 0x12u, 0x14u, 0x15u, 0x16u,
        0x1Cu, 0x1Du, 0x1Eu, 0x1Fu, 0x20u, 0x21u};
    for (const DRW_Dictionary& dictionary : cData->dictionaries) {
        if (dictionary.handle != 0 && !dxfW->reserveHandle(dictionary.handle)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
    }
    for (const DRW_XRecord& record : cData->xRecords) {
        if (record.handle != 0 && !dxfW->reserveHandle(record.handle)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
    }
    std::unordered_set<std::uint32_t> proxyHandles;
    for (const DRW_RawDxfObject& object : cData->rawProxyObjects) {
        if (object.handle == 0 || !proxyHandles.insert(object.handle).second
            || !dxfW->reserveHandle(object.handle)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
    }
    if (!cData->dataStorageSections.empty()) {
        for (const DRW_RawDxfObject& object : m_acdsHistoryObjects) {
            if (!proxyHandles.insert(object.handle).second
                || !dxfW->reserveHandle(object.handle)) {
                delete dxfW;
                dxfW = nullptr;
                return false;
            }
        }
        for (const std::uint32_t handle : m_acdsMaterialHandles) {
            if (!dxfW->reserveHandle(handle)) {
                delete dxfW;
                dxfW = nullptr;
                return false;
            }
        }
    }
    std::map<std::uint32_t, std::uint32_t> handleRemap;
    for (std::uint32_t handle : m_extensionDictionaryHandles) {
        if (fixedHandles.count(handle) == 0)
            continue;
        const std::uint32_t replacement = dxfW->allocHandle();
        if (replacement == 0 || !dxfW->reserveHandle(replacement)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
        handleRemap.emplace(handle, replacement);
    }
    for (std::uint32_t handle : m_extensionXRecordHandles) {
        if (fixedHandles.count(handle) == 0)
            continue;
        const std::uint32_t replacement = dxfW->allocHandle();
        if (replacement == 0 || !dxfW->reserveHandle(replacement)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
        handleRemap.emplace(handle, replacement);
    }
    dxfW->setHandleRemap(handleRemap);
    bool success = dxfW->write(this, v, binary);
    if (!success && debug) {
        const DRW_OperationDiagnostic diagnostic = dxfW->getLastDiagnostic();
        std::cerr << "DXF export failed: " << diagnostic.code << ": "
                  << diagnostic.message << '\n';
    }
    delete dxfW;
    dxfW = nullptr;
    return success;
}

void dx_iface::writeEntity(DRW_Entity* e){
    switch (e->eType) {
    case DRW::POINT:
        dxfW->writePoint(static_cast<DRW_Point*>(e));
        break;
    case DRW::LINE:
        dxfW->writeLine(static_cast<DRW_Line*>(e));
        break;
    case DRW::THREEDLINE:
        dxfW->write3DLine(static_cast<DRW_3DLine*>(e));
        break;
    case DRW::RAY:
        dxfW->writeRay(static_cast<DRW_Ray*>(e));
        break;
    case DRW::XLINE:
        dxfW->writeXline(static_cast<DRW_Xline*>(e));
        break;
    case DRW::CIRCLE:
        dxfW->writeCircle(static_cast<DRW_Circle*>(e));
        break;
    case DRW::ARC:
        dxfW->writeArc(static_cast<DRW_Arc*>(e));
        break;
    case DRW::SOLID:
        dxfW->writeSolid(static_cast<DRW_Solid*>(e));
        break;
    case DRW::DXF_TRACE:
        dxfW->writeTrace(static_cast<DRW_Trace*>(e));
        break;
    case DRW::E3DFACE:
        dxfW->write3dface(static_cast<DRW_3Dface*>(e));
        break;
    case DRW::ELLIPSE:
        dxfW->writeEllipse(static_cast<DRW_Ellipse*>(e));
        break;
    case DRW::LWPOLYLINE:
        dxfW->writeLWPolyline(static_cast<DRW_LWPolyline*>(e));
        break;
    case DRW::POLYLINE:
        dxfW->writePolyline(static_cast<DRW_Polyline*>(e));
        break;
    case DRW::MESH:
        dxfW->writeMesh(static_cast<DRW_Mesh*>(e));
        break;
    case DRW::HELIX:
        dxfW->writeHelix(static_cast<DRW_Helix*>(e));
        break;
    case DRW::SPLINE:
        dxfW->writeSpline(static_cast<DRW_Spline*>(e));
        break;
//    case RS2::EntitySplinePoints:
//        writeSplinePoints(static_cast<DRW_Point*>(e));
//        break;
//    case RS2::EntityVertex:
//        break;
    case DRW::INSERT:
        dxfW->writeInsert(static_cast<DRW_Insert*>(e));
        break;
    case DRW::MTEXT:
        dxfW->writeMText(static_cast<DRW_MText*>(e));
        break;
    case DRW::TEXT:
        if (auto* rtext = dynamic_cast<DRW_RText*>(e))
            dxfW->writeRText(rtext);
        else if (auto* arcText = dynamic_cast<DRW_ArcAlignedText*>(e))
            dxfW->writeArcAlignedText(arcText);
        else
            dxfW->writeText(static_cast<DRW_Text*>(e));
        break;
    case DRW::DIMLINEAR:
    case DRW::DIMALIGNED:
    case DRW::DIMANGULAR:
    case DRW::DIMANGULAR3P:
    case DRW::DIMRADIAL:
    case DRW::DIMDIAMETRIC:
    case DRW::DIMORDINATE:
        dxfW->writeDimension(static_cast<DRW_Dimension*>(e));
        break;
    case DRW::LEADER:
        dxfW->writeLeader(static_cast<DRW_Leader*>(e));
        break;
    case DRW::HATCH:
        dxfW->writeHatch(static_cast<DRW_Hatch*>(e));
        break;
    case DRW::MPOLYGON:
        dxfW->writeMPolygon(static_cast<DRW_MPolygon*>(e));
        break;
    case DRW::VIEWPORT:
        dxfW->writeViewport(static_cast<DRW_Viewport*>(e));
        break;
    case DRW::IMAGE:
        dxfW->writeImage(static_cast<DRW_Image*>(e), static_cast<dx_ifaceImg*>(e)->path);
        break;
    case DRW::PLANESURFACE:
    case DRW::EXTRUDEDSURFACE:
    case DRW::REVOLVEDSURFACE:
    case DRW::SWEPTSURFACE:
    case DRW::LOFTEDSURFACE:
    case DRW::NURBSURFACE:
        dxfW->writeSurface(static_cast<DRW_Surface*>(e));
        break;
    case DRW::E3DSOLID:
    case DRW::REGION:
    case DRW::BODY:
        dxfW->writeModelerGeometry(static_cast<DRW_ModelerGeometry*>(e));
        break;
    default:
        break;
    }
}
