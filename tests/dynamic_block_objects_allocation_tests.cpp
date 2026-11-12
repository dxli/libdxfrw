// SPDX-License-Identifier: GPL-2.0-or-later
// Generated dynamic-block OBJECTS record allocation rollback control.
#include "libdxfrw.h"
#include "intern/dxfreader.h"
#include "intern/dxfwriter.h"

#include <cstdlib>
#include <array>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <vector>

class DxfDynamicBlockTestAccess {
public:
    static void attachWriter(dxfRW& codec, DRW::Version version,
                             bool binary, std::ostream& output) {
        codec.version = version;
        codec.binFile = binary;
        codec.writer = binary
            ? std::unique_ptr<dxfWriter>(
                  std::make_unique<dxfWriterBinary>(&output))
            : std::unique_ptr<dxfWriter>(
                  std::make_unique<dxfWriterAscii>(&output));
    }

    static bool writeEnd(dxfRW& codec) {
        return codec.writer != nullptr
            && codec.writer->writeString(0, "WIRE_END");
    }
};

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0u;

struct AllocationFailure {
    explicit AllocationFailure(long count) {
        allocationsBeforeFailure = count;
    }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

DRW_DynamicBlockObject makePurgePreventer() {
    DRW_DynamicBlockObject object(
        "ACDB_DYNAMICBLOCKPURGEPREVENTER_VERSION");
    object.handle = 0xE702u;
    object.parentHandle = 0xE701u;
    object.reactorHandles.push_back(0xE701u);
    object.m_purgeFlag = 1u;
    object.m_purgeBlockHandle = 0xE700u;
    object.m_bodyFullyDecoded = true;
    return object;
}

DRW_DynamicBlockObject makeVisibilityParameter() {
    DRW_DynamicBlockObject object("BLOCKVISIBILITYPARAMETER");
    object.handle = 0xE818u;
    object.parentHandle = 0xE817u;
    object.m_kind = DRW_DynamicBlockObject::Kind::VisibilityParameter;
    object.m_bodyFullyDecoded = true;
    object.m_evalExprParsed = true;
    object.m_parentId = std::numeric_limits<std::uint32_t>::max();
    object.m_major = 29u;
    object.m_minor = 2u;
    object.m_valueCode = -9999;
    object.m_nodeId = 16u;
    object.m_elementParsed = true;
    object.m_elementName = "Visibility State";
    object.m_elementMajor = 29u;
    object.m_elementMinor = 2u;
    object.m_propertyInfoCount = 17;
    object.m_isInitialized = true;
    object.m_visibilityName = "ARCH Title Block";
    object.m_blockCount = 117;
    for (std::uint32_t index = 0u; index < 117u; ++index)
        object.m_blockHandles.push_back(0x1001u + index);
    object.m_stateCount = 4;
    static constexpr const char* stateNames[] = {
        "ARCH D", "ARCH C", "ARCH B", "ARCH A"};
    for (std::size_t state = 0u; state < 4u; ++state) {
        object.m_stateNames.emplace_back(stateNames[state]);
        std::vector<std::uint32_t> stateBlocks;
        for (std::uint32_t member = 0u; member < 29u; ++member)
            stateBlocks.push_back(0x1001u
                + static_cast<std::uint32_t>(state) * 29u + member);
        object.m_stateBlockHandles.push_back(std::move(stateBlocks));
        object.m_stateParameterHandles.emplace_back();
    }
    return object;
}

std::map<std::uint32_t, std::uint32_t> visibilityRemap() {
    std::map<std::uint32_t, std::uint32_t> remap;
    for (std::uint32_t index = 0u; index < 117u; ++index) {
        const std::uint32_t source = 0x1001u + index;
        remap.emplace(source, source + 0x5000u);
    }
    return remap;
}

bool visibilitySourceUnchanged(const DRW_DynamicBlockObject& object) {
    return object.handle == 0xE818u
        && static_cast<std::uint32_t>(object.parentHandle) == 0xE817u
        && object.m_kind
            == DRW_DynamicBlockObject::Kind::VisibilityParameter
        && object.m_bodyFullyDecoded && object.m_evalExprParsed
        && object.m_elementName == "Visibility State"
        && object.m_visibilityName == "ARCH Title Block"
        && object.m_blockCount == 117
        && object.m_blockHandles.size() == 117u
        && object.m_blockHandles.front() == 0x1001u
        && object.m_blockHandles.back() == 0x1075u
        && object.m_stateCount == 4
        && object.m_stateNames.size() == 4u
        && object.m_stateNames.front() == "ARCH D"
        && object.m_stateNames.back() == "ARCH A"
        && object.m_stateBlockHandles.size() == 4u
        && std::all_of(object.m_stateBlockHandles.begin(),
            object.m_stateBlockHandles.end(),
            [](const auto& handles) { return handles.size() == 29u; })
        && object.m_stateBlockHandles.front().front() == 0x1001u
        && object.m_stateBlockHandles.back().back() == 0x1074u
        && object.m_stateParameterHandles.size() == 4u
        && std::all_of(object.m_stateParameterHandles.begin(),
            object.m_stateParameterHandles.end(),
            [](const auto& handles) { return handles.empty(); });
}

bool visibilityWireReadback(const std::string& bytes, bool binary) {
    std::istringstream input(bytes);
    std::unique_ptr<dxfReader> reader = binary
        ? std::unique_ptr<dxfReader>(std::make_unique<dxfReaderBinary>(&input))
        : std::unique_ptr<dxfReader>(std::make_unique<dxfReaderAscii>(&input));
    int code = 0;
    if (!reader->readRec(&code) || code != 0
        || reader->getString() != "BLOCKVISIBILITYPARAMETER")
        return false;
    bool hasHandle = false;
    bool hasOwner = false;
    std::size_t flatReferenceCount = 0u;
    std::size_t stateReferenceCount = 0u;
    bool referencesInRemapRange = true;
    bool ended = false;
    while (reader->readRec(&code)) {
        if (code == 0) {
            ended = reader->getString() == "WIRE_END";
            break;
        }
        switch (code) {
        case 5:
            hasHandle = reader->getHandleString() == 0xE818u;
            break;
        case 330:
            hasOwner = reader->getHandleString() == 0xE817u;
            break;
        case 331:
            {
                const std::uint32_t handle = reader->getHandleString();
                referencesInRemapRange = referencesInRemapRange
                    && handle >= 0x6001u && handle <= 0x6075u;
                ++flatReferenceCount;
            }
            break;
        case 332:
            {
                const std::uint32_t handle = reader->getHandleString();
                referencesInRemapRange = referencesInRemapRange
                    && handle >= 0x6001u && handle <= 0x6075u;
                ++stateReferenceCount;
            }
            break;
        default:
            break;
        }
    }
    return ended && hasHandle && hasOwner && referencesInRemapRange
        && flatReferenceCount == 117u && stateReferenceCount == 116u;
}

template <typename Factory, typename SourceCheck, typename WireCheck>
bool dynamicBlockAllocationSweep(
        const char* profile, bool binary, const Factory& makeObject,
        const SourceCheck& sourceUnchanged, const WireCheck& wireReadback,
        const std::map<std::uint32_t, std::uint32_t>& remap) {
    constexpr long calibrationLimit = 1000000;
    std::ostringstream cleanBytes;
    dxfRW cleanCodec("");
    DxfDynamicBlockTestAccess::attachWriter(
        cleanCodec, DRW::AC1024, binary, cleanBytes);
    cleanCodec.setHandleRemap(remap);
    DRW_DynamicBlockObject cleanObject = makeObject();
    long allocationCount = 0;
    bool cleanWrite = false;
    {
        AllocationFailure failure(calibrationLimit);
        cleanWrite = cleanCodec.writeDynamicBlockObject(&cleanObject);
        allocationCount = calibrationLimit - allocationsBeforeFailure;
    }
    const DRW_WritingContext* cleanContext =
        cleanCodec.getWritingContext();
    if (!cleanWrite || !sourceUnchanged(cleanObject)
        || cleanContext == nullptr
        || cleanContext->sourceHandleToWrittenObjectMap.count(
               cleanObject.handle) != 1u
        || cleanContext->sourceHandleToWrittenObjectMap.at(cleanObject.handle)
            != cleanCodec.remapHandle(cleanObject.handle)
        || !DxfDynamicBlockTestAccess::writeEnd(cleanCodec)
        || !wireReadback(cleanBytes.str(), binary) || allocationCount <= 0) {
        std::cerr << "clean " << profile
                  << " write/readback failed (binary=" << binary
                  << ") allocations=" << allocationCount << '\n';
        return false;
    }

    for (long offset = 0; offset < allocationCount; ++offset) {
        std::ostringstream failedBytes;
        dxfRW failedCodec("");
        DxfDynamicBlockTestAccess::attachWriter(
            failedCodec, DRW::AC1024, binary, failedBytes);
        failedCodec.setHandleRemap(remap);
        DRW_DynamicBlockObject object = makeObject();
        const std::size_t failuresBefore = failedAllocations;
        bool wrote = false;
        try {
            AllocationFailure failure(offset);
            wrote = failedCodec.writeDynamicBlockObject(&object);
        } catch (const std::bad_alloc&) {
            allocationsBeforeFailure = -1;
        }
        allocationsBeforeFailure = -1;
        const DRW_WritingContext* context = failedCodec.getWritingContext();
        if (wrote || failedAllocations == failuresBefore
            || !sourceUnchanged(object) || context == nullptr
            || context->sourceHandleToWrittenObjectMap.count(object.handle)
                != 0u) {
            std::cerr << profile << " allocation rollback failed at "
                      << (binary ? "Binary" : "ASCII")
                      << " offset=" << offset << " wrote=" << wrote
                      << " identityAbsent="
                      << (context != nullptr
                          && context->sourceHandleToWrittenObjectMap.count(
                                 object.handle) == 0u)
                      << '\n';
            return false;
        }

        // The failed low-level writer can retain partial stream bytes and a
        // sticky write error. Prove the source is retryable in a fresh writer.
        std::ostringstream retryBytes;
        dxfRW retryCodec("");
        DxfDynamicBlockTestAccess::attachWriter(
            retryCodec, DRW::AC1024, binary, retryBytes);
        retryCodec.setHandleRemap(remap);
        const bool retryWrote = retryCodec.writeDynamicBlockObject(&object);
        const DRW_WritingContext* retryContext =
            retryCodec.getWritingContext();
        if (!retryWrote || !sourceUnchanged(object) || retryContext == nullptr
            || retryContext->sourceHandleToWrittenObjectMap.count(
                   object.handle) != 1u
            || !DxfDynamicBlockTestAccess::writeEnd(retryCodec)
            || !wireReadback(retryBytes.str(), binary)) {
            std::cerr << profile << " fresh-writer retry failed at "
                      << (binary ? "Binary" : "ASCII")
                      << " offset=" << offset << '\n';
            return false;
        }
    }
    std::cout << (binary ? "Binary" : "ASCII") << ' ' << profile
              << " allocation offsets=" << allocationCount
              << "; source rollback and fresh-writer retries pass\n";
    return true;
}

bool visibilityAllocationSweep(bool binary) {
    return dynamicBlockAllocationSweep(
        "VisibilityParameter", binary, makeVisibilityParameter,
        visibilitySourceUnchanged, visibilityWireReadback,
        visibilityRemap());
}

DRW_DynamicBlockObject makeBasePointParameter() {
    DRW_DynamicBlockObject object("BLOCKBASEPOINTPARAMETER");
    object.handle = 0x1135u;
    object.parentHandle = 0x110Fu;
    object.m_kind = DRW_DynamicBlockObject::Kind::BasePointParameter;
    object.m_bodyFullyDecoded = true;
    object.m_evalExprParsed = true;
    object.m_parentId = std::numeric_limits<std::uint32_t>::max();
    object.m_major = 29u;
    object.m_minor = 2u;
    object.m_valueCode = -9999;
    object.m_nodeId = 79u;
    object.m_elementParsed = true;
    object.m_elementName = "Base Point";
    object.m_elementMajor = 29u;
    object.m_elementMinor = 2u;
    object.m_showProperties = true;
    object.m_defPoint = DRW_Coord(11.25, -12.5, 13.75);
    object.m_parameterValue93 = -4;
    object.m_parameterValue170 = 2;
    object.m_parameterValue171 = 3;
    object.m_point = DRW_Coord(21.25, -22.5, 23.75);
    object.m_basePoint = DRW_Coord(-31.25, 32.5, -33.75);
    return object;
}

bool basePointSourceUnchanged(const DRW_DynamicBlockObject& object) {
    return object.handle == 0x1135u
        && static_cast<std::uint32_t>(object.parentHandle) == 0x110Fu
        && object.m_kind
            == DRW_DynamicBlockObject::Kind::BasePointParameter
        && object.m_bodyFullyDecoded && object.m_evalExprParsed
        && object.m_parentId == std::numeric_limits<std::uint32_t>::max()
        && object.m_nodeId == 79u && object.m_elementParsed
        && object.m_elementName == "Base Point"
        && object.m_showProperties
        && object.m_defPoint.x == 11.25 && object.m_defPoint.y == -12.5
        && object.m_defPoint.z == 13.75
        && object.m_parameterValue93 == -4
        && object.m_parameterValue170 == 2
        && object.m_parameterValue171 == 3
        && object.m_point.x == 21.25 && object.m_point.y == -22.5
        && object.m_point.z == 23.75
        && object.m_basePoint.x == -31.25
        && object.m_basePoint.y == 32.5
        && object.m_basePoint.z == -33.75;
}

bool basePointWireReadback(const std::string& bytes, bool binary) {
    std::istringstream input(bytes);
    std::unique_ptr<dxfReader> reader = binary
        ? std::unique_ptr<dxfReader>(std::make_unique<dxfReaderBinary>(&input))
        : std::unique_ptr<dxfReader>(std::make_unique<dxfReaderAscii>(&input));
    int code = 0;
    if (!reader->readRec(&code) || code != 0
        || reader->getString() != "BLOCKBASEPOINTPARAMETER")
        return false;
    bool hasHandle = false;
    bool hasOwner = false;
    bool hasNode = false;
    bool hasDefPointX = false;
    bool hasParamValue = false;
    bool hasPointX = false;
    bool hasBasePointZ = false;
    bool hasSubclass = false;
    bool ended = false;
    while (reader->readRec(&code)) {
        if (code == 0) {
            ended = reader->getString() == "WIRE_END";
            break;
        }
        switch (code) {
        case 5:
            hasHandle = reader->getHandleString() == 0x1135u;
            break;
        case 330:
            hasOwner = reader->getHandleString() == 0x110Fu;
            break;
        case 90:
            hasNode = reader->getInt32() == 79;
            break;
        case 1010:
            hasDefPointX = reader->getDouble() == 11.25;
            break;
        case 93:
            hasParamValue = reader->getInt32() == -4;
            break;
        case 1011:
            hasPointX = reader->getDouble() == 21.25;
            break;
        case 1032:
            hasBasePointZ = reader->getDouble() == -33.75;
            break;
        case 100:
            hasSubclass = hasSubclass
                || reader->getString() == "AcDbBlockBasepointParameter";
            break;
        default:
            break;
        }
    }
    return ended && hasHandle && hasOwner && hasNode && hasDefPointX
        && hasParamValue && hasPointX && hasBasePointZ && hasSubclass;
}

bool basePointAllocationSweep(bool binary) {
    return dynamicBlockAllocationSweep(
        "BasePointParameter", binary, makeBasePointParameter,
        basePointSourceUnchanged, basePointWireReadback, {});
}

DRW_DynamicBlockObject makeAlignmentParameter() {
    DRW_DynamicBlockObject object("BLOCKALIGNMENTPARAMETER");
    object.handle = 0x1126u;
    object.parentHandle = 0x110Fu;
    object.m_kind = DRW_DynamicBlockObject::Kind::AlignmentParameter;
    object.m_bodyFullyDecoded = true;
    object.m_evalExprParsed = true;
    object.m_parentId = std::numeric_limits<std::uint32_t>::max();
    object.m_major = 29u;
    object.m_minor = 2u;
    object.m_valueCode = -9999;
    object.m_nodeId = 54u;
    object.m_elementParsed = true;
    object.m_elementName = "Alignment";
    object.m_elementMajor = 29u;
    object.m_elementMinor = 2u;
    object.m_showProperties = true;
    object.m_firstPoint = DRW_Coord(1.25, -2.5, 3.75);
    object.m_secondPoint = DRW_Coord(15.0, 4.5, -6.75);
    object.m_twoPointPropertyCodes[0] = {55, -4};
    object.m_twoPointPropertyNames[0] = {"Distance", "Width"};
    object.m_twoPointPropertyCodes[1] = {92};
    object.m_twoPointPropertyNames[1] = {"Angle"};
    object.m_twoPointPropertyCodes[3] = {301, 0};
    object.m_twoPointPropertyNames[3] = {"Position", "Enabled"};
    object.m_propertyStates = {55, 0, 7, -1};
    object.m_parameterBaseLocation = 2;
    object.m_alignPerpendicular = true;
    return object;
}

bool alignmentSourceUnchanged(const DRW_DynamicBlockObject& object) {
    return object.handle == 0x1126u
        && static_cast<std::uint32_t>(object.parentHandle) == 0x110Fu
        && object.m_kind
            == DRW_DynamicBlockObject::Kind::AlignmentParameter
        && object.m_bodyFullyDecoded && object.m_evalExprParsed
        && object.m_parentId == std::numeric_limits<std::uint32_t>::max()
        && object.m_nodeId == 54u && object.m_elementParsed
        && object.m_elementName == "Alignment"
        && object.m_firstPoint.x == 1.25
        && object.m_firstPoint.y == -2.5
        && object.m_firstPoint.z == 3.75
        && object.m_secondPoint.x == 15.0
        && object.m_secondPoint.y == 4.5
        && object.m_secondPoint.z == -6.75
        && object.m_twoPointPropertyCodes.size() == 4u
        && object.m_twoPointPropertyCodes[0]
            == std::vector<std::int32_t>{55, -4}
        && object.m_twoPointPropertyCodes[1]
            == std::vector<std::int32_t>{92}
        && object.m_twoPointPropertyCodes[2].empty()
        && object.m_twoPointPropertyCodes[3]
            == std::vector<std::int32_t>{301, 0}
        && object.m_twoPointPropertyNames.size() == 4u
        && object.m_twoPointPropertyNames[0]
            == std::vector<UTF8STRING>{"Distance", "Width"}
        && object.m_twoPointPropertyNames[1]
            == std::vector<UTF8STRING>{"Angle"}
        && object.m_twoPointPropertyNames[2].empty()
        && object.m_twoPointPropertyNames[3]
            == std::vector<UTF8STRING>{"Position", "Enabled"}
        && object.m_propertyStates
            == std::array<std::int32_t, 4>{{55, 0, 7, -1}}
        && object.m_parameterBaseLocation == 2
        && object.m_alignPerpendicular;
}

bool alignmentWireReadback(const std::string& bytes, bool binary) {
    std::istringstream input(bytes);
    std::unique_ptr<dxfReader> reader = binary
        ? std::unique_ptr<dxfReader>(std::make_unique<dxfReaderBinary>(&input))
        : std::unique_ptr<dxfReader>(std::make_unique<dxfReaderAscii>(&input));
    int code = 0;
    if (!reader->readRec(&code) || code != 0
        || reader->getString() != "BLOCKALIGNMENTPARAMETER")
        return false;
    bool hasHandle = false;
    bool hasOwner = false;
    bool hasNode = false;
    bool hasPropertyStates = false;
    bool hasConnections = false;
    bool hasBaseLocation = false;
    bool hasAlignmentSubclass = false;
    bool hasSubclass = false;
    bool ended = false;
    std::vector<std::int32_t> propertyStates;
    std::vector<std::int32_t> propertyCounts;
    std::vector<std::pair<std::int32_t, std::string>> connections;
    bool awaitingConnectionName = false;
    std::int32_t pendingConnectionCode = 0;
    while (reader->readRec(&code)) {
        if (code == 0) {
            ended = reader->getString() == "WIRE_END";
            break;
        }
        switch (code) {
        case 5:
            hasHandle = reader->getHandleString() == 0x1126u;
            break;
        case 330:
            hasOwner = reader->getHandleString() == 0x110Fu;
            break;
        case 90:
            hasNode = reader->getInt32() == 54;
            break;
        case 91:
            propertyStates.push_back(reader->getInt32());
            break;
        case 171:
        case 172:
        case 173:
        case 174:
            propertyCounts.push_back(reader->getInt32());
            break;
        case 92:
        case 93:
        case 94:
        case 95:
            pendingConnectionCode = reader->getInt32();
            awaitingConnectionName = true;
            break;
        case 301:
        case 302:
        case 303:
        case 304:
            if (awaitingConnectionName) {
                connections.emplace_back(
                    pendingConnectionCode, reader->getString());
                awaitingConnectionName = false;
            }
            break;
        case 177:
            hasBaseLocation = reader->getInt32() == 2;
            break;
        case 100:
            {
                const std::string marker = reader->getString();
                hasSubclass = hasSubclass || marker == "AcDbBlock2PtParameter";
                hasAlignmentSubclass = hasAlignmentSubclass
                    || marker == "AcDbBlockAlignmentParameter";
            }
            break;
        case 280:
            hasAlignmentSubclass = hasAlignmentSubclass || hasSubclass;
            break;
        default:
            break;
        }
    }
    hasPropertyStates = propertyStates
        == std::vector<std::int32_t>{55, 0, 7, -1};
    hasConnections = connections
        == std::vector<std::pair<std::int32_t, std::string>>{
            {55, "Distance"}, {-4, "Width"}, {92, "Angle"},
            {301, "Position"}, {0, "Enabled"}};
    return ended && hasHandle && hasOwner && hasNode && hasPropertyStates
        && propertyCounts == std::vector<std::int32_t>{2, 1, 0, 2}
        && hasConnections && hasBaseLocation && hasSubclass
        && hasAlignmentSubclass;
}

bool alignmentAllocationSweep(bool binary) {
    return dynamicBlockAllocationSweep(
        "AlignmentParameter", binary, makeAlignmentParameter,
        alignmentSourceUnchanged, alignmentWireReadback, {});
}

bool wireReadback(const std::string& bytes, bool binary) {
    std::istringstream input(bytes);
    std::unique_ptr<dxfReader> reader = binary
        ? std::unique_ptr<dxfReader>(std::make_unique<dxfReaderBinary>(&input))
        : std::unique_ptr<dxfReader>(std::make_unique<dxfReaderAscii>(&input));
    int code = 0;
    if (!reader->readRec(&code) || code != 0
        || reader->getString() != "ACDB_DYNAMICBLOCKPURGEPREVENTER_VERSION")
        return false;
    bool ended = false;
    bool hasHandle = false;
    bool hasSubclass = false;
    bool hasFlag = false;
    std::vector<std::uint32_t> owners;
    while (reader->readRec(&code)) {
        if (code == 0) {
            ended = reader->getString() == "WIRE_END";
            break;
        }
        switch (code) {
        case 5:
            hasHandle = reader->getHandleString() == 0xE702u;
            break;
        case 102:
            {
                const std::string marker = reader->getString();
                if (marker != "{ACAD_REACTORS" && marker != "}")
                    return false;
            }
            break;
        case 330:
            owners.push_back(reader->getHandleString());
            break;
        case 100:
            hasSubclass = reader->getString()
                == "AcDbDynamicBlockPurgePreventer";
            break;
        case 70:
            hasFlag = reader->getInt32() == 1;
            break;
        default:
            return false;
        }
    }
    return ended && hasHandle && hasSubclass && hasFlag
        && owners == std::vector<std::uint32_t>{0xE701u, 0xE701u};
}

bool sourceUnchanged(const DRW_DynamicBlockObject& object) {
    return object.handle == 0xE702u
        && static_cast<std::uint32_t>(object.parentHandle) == 0xE701u
        && object.reactorHandles == std::vector<std::uint32_t>{0xE701u}
        && object.m_purgeFlag == 1u
        && object.m_purgeBlockHandle == 0xE700u
        && object.m_bodyFullyDecoded;
}

bool allocationSweep(bool binary) {
    constexpr long calibrationLimit = 1000000;
    const auto writeRecord = [](dxfRW& codec,
                                DRW_DynamicBlockObject& object) {
        return codec.writeDynamicBlockObject(&object);
    };

    std::ostringstream cleanBytes;
    dxfRW cleanCodec("");
    DxfDynamicBlockTestAccess::attachWriter(
        cleanCodec, DRW::AC1024, binary, cleanBytes);
    DRW_DynamicBlockObject cleanObject = makePurgePreventer();
    long allocationCount = 0;
    bool cleanWrite = false;
    {
        AllocationFailure failure(calibrationLimit);
        cleanWrite = writeRecord(cleanCodec, cleanObject);
        allocationCount = calibrationLimit - allocationsBeforeFailure;
    }
    if (!cleanWrite || !sourceUnchanged(cleanObject)
        || cleanCodec.getWritingContext()
            ->sourceHandleToWrittenObjectMap.count(cleanObject.handle) != 1u
        || cleanCodec.getWritingContext()
            ->sourceHandleToWrittenObjectMap.at(cleanObject.handle)
            != cleanCodec.remapHandle(cleanObject.handle)
        || !DxfDynamicBlockTestAccess::writeEnd(cleanCodec)) {
        std::cerr << "clean PurgePreventer record failed (binary="
                  << binary << ") write=" << cleanWrite
                  << " source=" << sourceUnchanged(cleanObject)
                  << " handleBindings="
                  << cleanCodec.getWritingContext()
                         ->sourceHandleToWrittenObjectMap.size()
                  << '\n';
        return false;
    }
    const std::string expectedBytes = cleanBytes.str();
    const bool cleanReadback = wireReadback(expectedBytes, binary);
    if (allocationCount <= 0 || !cleanReadback) {
        std::cerr << "PurgePreventer clean readback/calibration failed binary="
                  << binary << " allocationCount=" << allocationCount
                  << " bytes=" << expectedBytes.size()
                  << " readback=" << cleanReadback << '\n';
        return false;
    }

    for (long offset = 0; offset < allocationCount; ++offset) {
        std::ostringstream failedBytes;
        dxfRW failedCodec("");
        DxfDynamicBlockTestAccess::attachWriter(
            failedCodec, DRW::AC1024, binary, failedBytes);
        DRW_DynamicBlockObject object = makePurgePreventer();
        const std::size_t failuresBefore = failedAllocations;
        bool wrote = false;
        try {
            AllocationFailure failure(offset);
            wrote = writeRecord(failedCodec, object);
        } catch (const std::bad_alloc&) {
            allocationsBeforeFailure = -1;
        }
        allocationsBeforeFailure = -1;
        const DRW_WritingContext* context = failedCodec.getWritingContext();
        if (wrote || failedAllocations == failuresBefore
            || !sourceUnchanged(object) || context == nullptr
            || context->sourceHandleToWrittenObjectMap.count(object.handle)
                != 0u) {
            std::cerr << "PurgePreventer allocation rollback failed at "
                      << (binary ? "Binary" : "ASCII")
                      << " offset=" << offset << " wrote=" << wrote
                      << " failed=" << (failedAllocations != failuresBefore)
                      << " identityAbsent="
                      << (context != nullptr
                          && context->sourceHandleToWrittenObjectMap.count(
                                 object.handle) == 0u)
                      << '\n';
            return false;
        }
    }
    std::cout << (binary ? "Binary" : "ASCII")
              << " PurgePreventer allocation offsets=" << allocationCount
              << "; source identities roll back on every refusal\n";
    return true;
}
} // namespace

void* operator new(std::size_t size) {
    if (allocationsBeforeFailure == 0) {
        ++failedAllocations;
        allocationsBeforeFailure = -1;
        throw std::bad_alloc();
    }
    if (allocationsBeforeFailure > 0)
        --allocationsBeforeFailure;
    if (void* value = std::malloc(size == 0u ? 1u : size))
        return value;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* value) noexcept { std::free(value); }
void operator delete[](void* value) noexcept { std::free(value); }
void operator delete(void* value, std::size_t) noexcept { std::free(value); }
void operator delete[](void* value, std::size_t) noexcept { std::free(value); }

int main() {
    return allocationSweep(false) && allocationSweep(true)
        && visibilityAllocationSweep(false)
        && visibilityAllocationSweep(true)
        && basePointAllocationSweep(false)
        && basePointAllocationSweep(true)
        && alignmentAllocationSweep(false)
        && alignmentAllocationSweep(true) ? 0 : 1;
}
