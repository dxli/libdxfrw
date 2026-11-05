#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "drw_base.h"
#include "drw_header.h"
#define private public
#include "libdxfrw.h"
#undef private
#include "dx_data.h"
#include "dx_iface.h"
#include "intern/dxfreader.h"
#include "intern/dxfwriter.h"
#include "libdwgr.h"

namespace {

class DimensionAssociationProbe final : public DRW_DimensionAssociation {
public:
    using DRW_DimensionAssociation::parseCode;

    std::uint8_t extensionDictionaryFlag() const noexcept {
        return xDictFlag;
    }
    std::int32_t reactorCount() const noexcept { return numReactors; }
};

class DimensionAssociationCapture final : public dx_iface {
public:
    void attachData(dx_data& data) {
        cData = &data;
        currentBlock = data.mBlock;
    }

    void addDimensionAssociation(
            const DRW_DimensionAssociation& association) override {
        DimensionAssociationProbe copy;
        static_cast<DRW_DimensionAssociation&>(copy) = association;
        associations.push_back(std::move(copy));
    }

    std::vector<DimensionAssociationProbe> associations;
};

bool sameVariant(const DRW_Variant& left, const DRW_Variant& right) {
    if (left.type() != right.type() || left.code() != right.code())
        return false;
    switch (left.type()) {
    case DRW_Variant::STRING:
        return std::string(left.c_str()) == std::string(right.c_str());
    case DRW_Variant::INTEGER:
        return left.i_val() == right.i_val();
    case DRW_Variant::INTEGER64:
        return left.i64_val() == right.i64_val();
    case DRW_Variant::DOUBLE:
        return std::abs(left.d_val() - right.d_val())
            <= 1e-12 * std::max(1.0, std::abs(left.d_val()));
    case DRW_Variant::COORD: {
        const DRW_Coord* a = left.coord();
        const DRW_Coord* b = right.coord();
        return a != nullptr && b != nullptr && a->x == b->x
            && a->y == b->y && a->z == b->z;
    }
    case DRW_Variant::BINARY:
        return *left.binary() == *right.binary();
    case DRW_Variant::INVALID:
        return true;
    }
    return false;
}

bool sameExtendedData(const std::vector<DRW_Variant*>& left,
                      const std::vector<DRW_Variant*>& right) {
    if (left.size() != right.size())
        return false;
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i] == nullptr || right[i] == nullptr
            || !sameVariant(*left[i], *right[i]))
            return false;
    }
    return true;
}

bool isReactorEnvelope(const std::list<DRW_Variant>& group) {
    return !group.empty() && group.front().code() == 102
        && group.front().type() == DRW_Variant::STRING
        && std::string(group.front().c_str()) == "ACAD_REACTORS";
}

bool sameAppData(const std::list<std::list<DRW_Variant>>& left,
                 const std::list<std::list<DRW_Variant>>& right) {
    std::vector<const std::list<DRW_Variant>*> leftGroups;
    std::vector<const std::list<DRW_Variant>*> rightGroups;
    for (const auto& group : left) {
        if (!isReactorEnvelope(group))
            leftGroups.push_back(&group);
    }
    for (const auto& group : right) {
        if (!isReactorEnvelope(group))
            rightGroups.push_back(&group);
    }
    if (leftGroups.size() != rightGroups.size())
        return false;
    for (std::size_t groupIndex = 0; groupIndex < leftGroups.size();
         ++groupIndex) {
        const auto& leftGroup = *leftGroups[groupIndex];
        const auto& rightGroup = *rightGroups[groupIndex];
        if (leftGroup.size() != rightGroup.size())
            return false;
        auto leftValue = leftGroup.begin();
        auto rightValue = rightGroup.begin();
        for (; leftValue != leftGroup.end(); ++leftValue, ++rightValue) {
            if (!sameVariant(*leftValue, *rightValue))
                return false;
        }
    }
    return true;
}

std::vector<std::uint32_t> mainObjectHandles(
        const DRW_DimensionAssociationOsnapRef& ref) {
    if (!ref.m_objectHandles.empty())
        return ref.m_objectHandles;
    return ref.m_objectHandle != 0
        ? std::vector<std::uint32_t>{ref.m_objectHandle}
        : std::vector<std::uint32_t>{};
}

bool sameRef(const DRW_DimensionAssociationOsnapRef& source,
             const DRW_DimensionAssociationOsnapRef& output,
             const std::map<std::uint32_t, std::uint32_t>& remap) {
    const auto mapped = [&remap](std::uint32_t handle) {
        if (handle == 0)
            return std::uint32_t{0};
        const auto found = remap.find(handle);
        return found == remap.end() ? handle : found->second;
    };
    const std::vector<std::uint32_t> sourceHandles =
        mainObjectHandles(source);
    const std::vector<std::uint32_t> outputHandles =
        mainObjectHandles(output);
    if (sourceHandles.size() != outputHandles.size())
        return false;
    for (std::size_t i = 0; i < sourceHandles.size(); ++i) {
        if (mapped(sourceHandles[i]) != outputHandles[i])
            return false;
    }
    return source.m_className == output.m_className
        && source.m_objectOsnapType == output.m_objectOsnapType
        && source.m_hasObjectOsnapType == output.m_hasObjectOsnapType
        && mapped(source.m_objectHandle) == output.m_objectHandle
        && source.m_hasSubentType == output.m_hasSubentType
        && source.m_subentType == output.m_subentType
        && source.m_hasGsMarker == output.m_hasGsMarker
        && source.m_gsMarker == output.m_gsMarker
        && source.m_hasNearParameter == output.m_hasNearParameter
        && (!source.m_hasNearParameter
            || std::abs(source.m_nearParameter - output.m_nearParameter)
                <= 1e-12 * std::max(1.0, std::abs(source.m_nearParameter)))
        && source.m_hasOsnapPoint == output.m_hasOsnapPoint
        && (!source.m_hasOsnapPoint
            || (source.m_osnapPoint.x == output.m_osnapPoint.x
                && source.m_osnapPoint.y == output.m_osnapPoint.y
                && source.m_osnapPoint.z == output.m_osnapPoint.z))
        && source.m_hasLastPointRef == output.m_hasLastPointRef
        && source.m_lastPointRef == output.m_lastPointRef
        && source.m_hasIntersectionObjectHandle
            == output.m_hasIntersectionObjectHandle
        && mapped(source.m_intersectionObjectHandle)
            == output.m_intersectionObjectHandle
        && source.m_hasIntersectionSubentType
            == output.m_hasIntersectionSubentType
        && source.m_intersectionSubentType
            == output.m_intersectionSubentType
        && source.m_hasIntersectionGsMarker
            == output.m_hasIntersectionGsMarker
        && source.m_intersectionGsMarker == output.m_intersectionGsMarker
        && source.m_hasXrefObjectHandleString
            == output.m_hasXrefObjectHandleString
        && source.m_xrefObjectHandleString == output.m_xrefObjectHandleString
        && source.m_hasIntersectionXrefObjectHandleString
            == output.m_hasIntersectionXrefObjectHandleString
        && source.m_intersectionXrefObjectHandleString
            == output.m_intersectionXrefObjectHandleString;
}

bool sameAssociation(const DimensionAssociationProbe& source,
                     const DimensionAssociationProbe& output,
                     const std::map<std::uint32_t, std::uint32_t>& remap) {
    const auto mapped = [&remap](std::uint32_t handle) {
        if (handle == 0)
            return std::uint32_t{0};
        const auto found = remap.find(handle);
        return found == remap.end() ? handle : found->second;
    };
    if (mapped(source.handle) != output.handle
        || mapped(source.parentHandle) != output.parentHandle
        || source.name != output.name
        || source.flags != output.flags
        || mapped(source.xDictHandle) != output.xDictHandle
        || source.reactorHandles.size() != output.reactorHandles.size()
        || !sameExtendedData(source.extData, output.extData)
        || !sameAppData(source.appData, output.appData)
        || source.m_dimensionHandle == 0
        || mapped(source.m_dimensionHandle) != output.m_dimensionHandle
        || source.m_associativityFlags != output.m_associativityFlags
        || source.m_isTransSpace != output.m_isTransSpace
        || source.m_rotatedDimensionType != output.m_rotatedDimensionType
        || source.m_osnapRefs.size() != output.m_osnapRefs.size()
        || output.m_hasUnrepresentableDetail)
        return false;
    for (std::size_t i = 0; i < source.reactorHandles.size(); ++i) {
        if (mapped(source.reactorHandles[i]) != output.reactorHandles[i])
            return false;
    }
    for (std::size_t i = 0; i < source.m_osnapRefs.size(); ++i) {
        if (!sameRef(source.m_osnapRefs[i], output.m_osnapRefs[i], remap))
            return false;
    }
    return true;
}

std::map<std::uint32_t, std::uint32_t> makeHandleRemap(
        const DRW_DimensionAssociation& association) {
    std::map<std::uint32_t, std::uint32_t> result;
    std::uint32_t next = 0xE0000001u;
    const auto add = [&result, &next](std::uint32_t handle) {
        if (handle == 0 || handle == DRW::NoHandle
            || result.count(handle) != 0)
            return;
        result.emplace(handle, next++);
    };
    add(association.handle);
    add(association.parentHandle);
    add(association.xDictHandle);
    add(association.m_dimensionHandle);
    for (const std::uint32_t reactor : association.reactorHandles)
        add(reactor);
    for (const auto& ref : association.m_osnapRefs) {
        for (std::uint32_t handle : mainObjectHandles(ref))
            add(handle);
        if (ref.m_hasIntersectionObjectHandle)
            add(ref.m_intersectionObjectHandle);
    }
    return result;
}

bool emitAndReadback(const DimensionAssociationProbe& source,
                     DRW::Version version, bool binary) {
    const auto remap = makeHandleRemap(source);
    std::ostringstream bytes;
    dxfRW writer("");
    writer.version = version;
    writer.binFile = binary;
    writer.setHandleRemap(remap);
    if (binary)
        writer.writer = std::make_unique<dxfWriterBinary>(&bytes);
    else
        writer.writer = std::make_unique<dxfWriterAscii>(&bytes);

    auto mutableSource = source;
    if (!writer.writeDimensionAssociation(&mutableSource)
        || !writer.writer->writeString(0, "WIRE_END")
        || writer.writer->hasWriteError()) {
        std::cerr << "write-failed format=" << (binary ? "binary" : "ascii")
                  << " handle=" << std::hex << source.handle << std::dec
                  << '\n';
        return false;
    }

    std::istringstream input(bytes.str());
    std::unique_ptr<dxfReader> reader;
    if (binary)
        reader = std::make_unique<dxfReaderBinary>(&input);
    else
        reader = std::make_unique<dxfReaderAscii>(&input);
    DimensionAssociationProbe output;
    int code = 0;
    if (!reader->readRec(&code) || code != 0
        || reader->getString() != "DIMASSOC")
        return false;
    bool parsed = true;
    bool ended = false;
    while (reader->readRec(&code)) {
        if (code == 0) {
            ended = reader->getString() == "WIRE_END";
            break;
        }
        if (!output.parseCode(code, reader)) {
            parsed = false;
            break;
        }
    }
    if (!parsed || !ended) {
        std::cerr << "parse-failed format=" << (binary ? "binary" : "ascii")
                  << " handle=" << std::hex << source.handle << std::dec
                  << " ended=" << ended << '\n';
        return false;
    }
    if (!sameAssociation(source, output, remap)) {
        std::cerr << "compare-failed format=" << (binary ? "binary" : "ascii")
                  << " handle=" << std::hex << source.handle << std::dec
                  << " srcParent=" << source.parentHandle
                  << " outParent=" << output.parentHandle
                  << " srcName=" << source.name
                  << " outName=" << output.name
                  << " srcFlags=" << source.flags
                  << " outFlags=" << output.flags
                  << " srcXDict=" << source.xDictHandle
                  << " outXDict=" << output.xDictHandle
                  << " srcXDictFlag=" << static_cast<int>(
                         source.extensionDictionaryFlag())
                  << " outXDictFlag=" << static_cast<int>(
                         output.extensionDictionaryFlag())
                  << " srcNumReactors=" << source.reactorCount()
                  << " outNumReactors=" << output.reactorCount()
                  << " srcDim=" << source.m_dimensionHandle
                  << " outDim=" << output.m_dimensionHandle
                  << " srcReactors=" << source.reactorHandles.size()
                  << " outReactors=" << output.reactorHandles.size()
                  << " srcXData=" << source.extData.size()
                  << " outXData=" << output.extData.size()
                  << " srcAppData=" << source.appData.size()
                  << " outAppData=" << output.appData.size()
                  << " srcRefs=" << source.m_osnapRefs.size()
                  << " outRefs=" << output.m_osnapRefs.size()
                  << " unrepresentable="
                  << output.m_hasUnrepresentableDetail << '\n';
        return false;
    }
    return true;
}

bool hasResolvedEntityReferences(
        const DRW_DimensionAssociation& association,
        const std::map<std::uint32_t, DRW::ETYPE>& entities) {
    if (association.m_dimensionHandle == 0
        || entities.count(association.m_dimensionHandle) == 0)
        return false;
    for (const auto& ref : association.m_osnapRefs) {
        const std::vector<std::uint32_t> handles = mainObjectHandles(ref);
        if (handles.empty() || ref.m_hasXrefObjectHandleString)
            return false;
        for (std::uint32_t handle : handles) {
            if (handle == 0 || entities.count(handle) == 0)
                return false;
        }
        if (ref.m_hasIntersectionObjectHandle
            && (entities.count(ref.m_intersectionObjectHandle) == 0
                || ref.m_hasIntersectionXrefObjectHandleString))
            return false;
    }
    return true;
}

bool runPhysicalSource(const std::string& path) {
    dx_data data;
    DimensionAssociationCapture capture;
    capture.attachData(data);
    dwgRW reader(path.c_str());
    const bool read = reader.read(&capture, false);
    if (!read) {
        std::cerr << "DWG read failed: error=" << reader.getError() << '\n';
        return false;
    }

    std::map<std::uint32_t, DRW::ETYPE> entities;
    const auto collectBlock = [&entities](const dx_ifaceBlock* block) {
        if (block == nullptr)
            return;
        for (const DRW_Entity* entity : block->ent) {
            if (entity != nullptr && entity->handle != 0
                && entity->handle != DRW::NoHandle)
                entities.emplace(entity->handle, entity->eType);
        }
    };
    collectBlock(data.mBlock);
    for (const dx_ifaceBlock* block : data.blocks)
        collectBlock(block);

    std::size_t closed = 0;
    std::size_t ascii = 0;
    std::size_t binary = 0;
    std::size_t refused = 0;
    std::map<int, std::size_t> dimensionTargetTypes;
    for (const DimensionAssociationProbe& association
         : capture.associations) {
        const auto dimension = entities.find(association.m_dimensionHandle);
        if (dimension != entities.end())
            ++dimensionTargetTypes[static_cast<int>(dimension->second)];
        const bool targetsResolved =
            hasResolvedEntityReferences(association, entities);
        if (association.m_hasUnrepresentableDetail || !targetsResolved) {
            ++refused;
            std::cerr << "excluded handle=" << std::hex << association.handle
                      << " dimension=" << association.m_dimensionHandle
                      << " dimensionTargetFound="
                      << (dimension != entities.end())
                      << " dimensionTargetType="
                      << (dimension == entities.end()
                              ? -1 : static_cast<int>(dimension->second))
                      << " refs=" << std::dec
                      << association.m_osnapRefs.size()
                      << " unrepresentable="
                      << association.m_hasUnrepresentableDetail << '\n';
            for (const auto& ref : association.m_osnapRefs) {
                const std::vector<std::uint32_t> handles =
                    mainObjectHandles(ref);
                std::cerr << "  main=" << std::hex;
                for (std::uint32_t handle : handles)
                    std::cerr << handle << ',';
                std::cerr << " mainFound=";
                for (std::uint32_t handle : handles)
                    std::cerr << entities.count(handle) << ',';
                std::cerr
                          << " xref=" << ref.m_hasXrefObjectHandleString
                          << " intersection="
                          << ref.m_intersectionObjectHandle
                          << " intersectionFound="
                          << entities.count(ref.m_intersectionObjectHandle)
                          << " intersectionXref="
                          << ref.m_hasIntersectionXrefObjectHandleString
                          << std::dec << " class=" << ref.m_className
                          << " osnapType="
                          << static_cast<unsigned>(ref.m_objectOsnapType)
                          << " hasSubent=" << ref.m_hasSubentType
                          << " subent=" << ref.m_subentType
                          << " hasGs=" << ref.m_hasGsMarker
                          << " gs=" << ref.m_gsMarker
                          << " hasNear=" << ref.m_hasNearParameter
                          << " near=" << ref.m_nearParameter
                          << " hasPoint=" << ref.m_hasOsnapPoint
                          << " hasLast=" << ref.m_hasLastPointRef
                          << " last=" << ref.m_lastPointRef
                          << " hasInterSubent="
                          << ref.m_hasIntersectionSubentType
                          << " interSubent=" << ref.m_intersectionSubentType
                          << " hasInterGs="
                          << ref.m_hasIntersectionGsMarker
                          << " interGs=" << ref.m_intersectionGsMarker
                          << " hasMainXref="
                          << ref.m_hasXrefObjectHandleString
                          << " mainXrefPath="
                          << ref.m_xrefObjectHandleString
                          << " hasInterXref="
                          << ref.m_hasIntersectionXrefObjectHandleString
                          << " interXrefPath="
                          << ref.m_intersectionXrefObjectHandleString
                          << std::dec << '\n';
            }
            continue;
        }
        ++closed;
        if (emitAndReadback(association, reader.getVersion(), false))
            ++ascii;
        if (emitAndReadback(association, reader.getVersion(), true))
            ++binary;
    }

    std::cout << "sourceVersion=" << reader.getVersion()
              << " entityParseFailures="
              << reader.getReadObservations().entityParseFailures
              << " objectParseFailures="
              << reader.getReadObservations().objectParseFailures
              << " callbacks=" << capture.associations.size()
              << " typedEntityHandles=" << entities.size()
              << " resolvedRepresentable=" << closed
              << " refusedOrUnresolved=" << refused
              << " asciiReadback=" << ascii
              << " binaryReadback=" << binary
              << " dimensionTargetTypes=";
    for (const auto& [type, count] : dimensionTargetTypes)
        std::cout << type << ':' << count << ',';
    std::cout << '\n';
    return closed != 0 && ascii == closed && binary == closed;
}

} // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: libdxfrw_dimassoc_physical_transport_tests <source.dwg>\n";
        return 2;
    }
    return runPhysicalSource(argv[1]) ? 0 : 1;
}
