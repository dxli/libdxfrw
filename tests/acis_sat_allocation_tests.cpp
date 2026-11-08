#include "drw_acis.h"

#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>

namespace {
std::atomic<int> failAfter{-1};

bool allocationShouldFail() noexcept {
    const int remaining = failAfter.load(std::memory_order_relaxed);
    if (remaining < 0) return false;
    if (remaining == 0) {
        failAfter.store(-1, std::memory_order_relaxed);
        return true;
    }
    failAfter.fetch_sub(1, std::memory_order_relaxed);
    return false;
}

void appendU32(std::vector<std::uint8_t>& bytes, std::uint32_t value) {
    for (unsigned shift = 0u; shift < 32u; shift += 8u)
        bytes.push_back(static_cast<std::uint8_t>(value >> shift));
}

void appendString(std::vector<std::uint8_t>& bytes, std::uint8_t tag,
                  const char* value, std::size_t length) {
    bytes.push_back(tag);
    bytes.push_back(static_cast<std::uint8_t>(length));
    bytes.insert(bytes.end(), value, value + length);
}

void appendDouble(std::vector<std::uint8_t>& bytes, double value) {
    bytes.push_back(6u);
    std::uint64_t bits = 0u;
    std::memcpy(&bits, &value, sizeof(bits));
    for (unsigned shift = 0u; shift < 64u; shift += 8u)
        bytes.push_back(static_cast<std::uint8_t>(bits >> shift));
}

void appendRawDouble(std::vector<std::uint8_t>& bytes, double value) {
    std::uint64_t bits = 0u;
    std::memcpy(&bits, &value, sizeof(bits));
    for (unsigned shift = 0u; shift < 64u; shift += 8u)
        bytes.push_back(static_cast<std::uint8_t>(bits >> shift));
}

std::vector<std::uint8_t> makeSab() {
    std::vector<std::uint8_t> bytes{
        'A','C','I','S',' ','B','i','n','a','r','y','F','i','l','e'};
    appendU32(bytes, 21200u);
    appendU32(bytes, 0u);
    appendU32(bytes, 1u);
    appendU32(bytes, 0u);
    appendString(bytes, 7u, "P", 1u);
    appendString(bytes, 7u, "A", 1u);
    appendString(bytes, 7u, "D", 1u);
    appendDouble(bytes, 1.0);
    appendDouble(bytes, 0.1);
    appendDouble(bytes, 0.01);
    appendString(bytes, 13u, "body", 4u);
    bytes.push_back(10u);
    bytes.push_back(17u);
    bytes.insert(bytes.end(), {0x0Eu,0x03u,'E','n','d',
                               0x0Eu,0x02u,'o','f',
                               0x0Eu,0x04u,'A','C','I','S',
                               0x0Du,0x04u,'d','a','t','a'});
    return bytes;
}

std::vector<std::uint8_t> makeModelerSab() {
    std::vector<std::uint8_t> bytes{
        'A','C','I','S',' ','B','i','n','a','r','y','F','i','l','e'};
    appendU32(bytes, 21800u);
    appendU32(bytes, 3u);
    appendU32(bytes, 2u);
    appendU32(bytes, 0u);
    appendString(bytes, 7u, "LOCAL_PRODUCT_IDENTIFIER_WITH_HEAP_STORAGE", 42u);
    appendString(bytes, 7u, "LOCAL_ACIS_VERSION_FOR_ALLOCATION_SWEEP", 39u);
    appendString(bytes, 7u, "LOCAL_CREATION_DATE_FOR_TESTING", 31u);
    appendDouble(bytes, 1.0);
    appendDouble(bytes, 1.0e-6);
    appendDouble(bytes, 1.0e-6);

    appendString(bytes, DRW_SabTag::EntityType, "point", 5u);
    bytes.push_back(DRW_SabTag::Pointer);
    appendU32(bytes, static_cast<std::uint32_t>(-1));
    bytes.push_back(DRW_SabTag::Int);
    appendU32(bytes, 101u);
    bytes.push_back(DRW_SabTag::LocationVec);
    appendRawDouble(bytes, 1.0);
    appendRawDouble(bytes, 2.0);
    appendRawDouble(bytes, 3.0);
    bytes.push_back(DRW_SabTag::RecordEnd);

    appendString(bytes, DRW_SabTag::EntityType, "vertex", 6u);
    bytes.push_back(DRW_SabTag::Pointer);
    appendU32(bytes, static_cast<std::uint32_t>(-1));
    bytes.push_back(DRW_SabTag::Int);
    appendU32(bytes, 202u);
    bytes.push_back(DRW_SabTag::Pointer);
    appendU32(bytes, 0u);
    bytes.push_back(DRW_SabTag::RecordEnd);

    appendString(bytes, DRW_SabTag::EntityType, "End-of-ACIS-data", 16u);
    bytes.push_back(DRW_SabTag::RecordEnd);
    return bytes;
}
} // namespace

void* operator new(std::size_t size) {
    if (allocationShouldFail()) throw std::bad_alloc();
    if (void* memory = std::malloc(size == 0u ? 1u : size)) return memory;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) {
    if (allocationShouldFail()) throw std::bad_alloc();
    if (void* memory = std::malloc(size == 0u ? 1u : size)) return memory;
    throw std::bad_alloc();
}

void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory, std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory, std::size_t) noexcept { std::free(memory); }

int main() {
    const std::vector<std::uint8_t> sab = makeSab();
    std::vector<std::uint8_t> expected;
    if (!drw_convertSabToSatV1(sab.data(), sab.size(), DRW::AC1024, expected)
        || expected.empty()) {
        std::fprintf(stderr, "baseline SAT conversion failed\n");
        return 1;
    }

    int firstSuccess = -1;
    for (int failIndex = 0; failIndex < 512; ++failIndex) {
        std::vector<std::uint8_t> output{0xA5u, 0x5Au};
        failAfter.store(failIndex, std::memory_order_relaxed);
        bool converted = false;
        bool threw = false;
        try {
            converted = drw_convertSabToSatV1(
                sab.data(), sab.size(), DRW::AC1024, output);
        } catch (...) {
            threw = true;
        }
        failAfter.store(-1, std::memory_order_relaxed);

        if (threw) {
            std::fprintf(stderr, "allocation %d escaped as an exception\n",
                         failIndex);
            return 2;
        }
        if (converted) {
            if (output != expected) {
                std::fprintf(stderr, "allocation sweep output mismatch\n");
                return 3;
            }
            firstSuccess = failIndex;
            break;
        }
        if (output != std::vector<std::uint8_t>({0xA5u, 0x5Au})) {
            std::fprintf(stderr, "allocation %d changed output on failure\n",
                         failIndex);
            return 4;
        }
        if (!drw_convertSabToSatV1(sab.data(), sab.size(), DRW::AC1024,
                                   output)
            || output != expected) {
            std::fprintf(stderr, "allocation %d same-buffer retry failed\n",
                         failIndex);
            return 5;
        }
    }

    if (firstSuccess < 8) {
        std::fprintf(stderr, "allocation sweep too short: %d\n", firstSuccess);
        return 6;
    }

    const std::vector<std::uint8_t> modelerSab = makeModelerSab();
    DRW_AcisBrep expectedBrep;
    DRW_SabParseOutcome expectedOutcome;
    if (!drw_decodeAcisWireframeWithOutcome(modelerSab, expectedBrep,
                                            expectedOutcome)
        || !expectedOutcome.complete || expectedBrep.vertices.size() != 1u
        || !expectedBrep.vertices.front().valid
        || expectedBrep.vertices.front().point.x != 1.0
        || expectedBrep.vertices.front().point.y != 2.0
        || expectedBrep.vertices.front().point.z != 3.0
        || !expectedBrep.hasBBox || expectedBrep.bboxMin.x != 1.0
        || expectedBrep.bboxMin.y != 2.0 || expectedBrep.bboxMin.z != 3.0
        || expectedBrep.bboxMax.x != 1.0 || expectedBrep.bboxMax.y != 2.0
        || expectedBrep.bboxMax.z != 3.0) {
        std::fprintf(stderr,
                     "baseline ACIS decode failed: complete=%d vertices=%zu "
                     "bbox=%d diagnostics=%zu\n",
                     expectedOutcome.complete ? 1 : 0,
                     expectedBrep.vertices.size(), expectedBrep.hasBBox ? 1 : 0,
                     expectedBrep.diagnostics.size());
        return 7;
    }
    const auto matchesExpectedBrep = [&expectedBrep](const DRW_AcisBrep& value) {
        return value.vertices.size() == expectedBrep.vertices.size()
            && value.vertices.front().point.x
                == expectedBrep.vertices.front().point.x
            && value.vertices.front().point.y
                == expectedBrep.vertices.front().point.y
            && value.vertices.front().point.z
                == expectedBrep.vertices.front().point.z
            && value.edges.size() == expectedBrep.edges.size()
            && value.faces.size() == expectedBrep.faces.size()
            && value.lumps.size() == expectedBrep.lumps.size()
            && value.shells.size() == expectedBrep.shells.size()
            && value.hasBBox == expectedBrep.hasBBox
            && value.bboxMin.x == expectedBrep.bboxMin.x
            && value.bboxMin.y == expectedBrep.bboxMin.y
            && value.bboxMin.z == expectedBrep.bboxMin.z
            && value.bboxMax.x == expectedBrep.bboxMax.x
            && value.bboxMax.y == expectedBrep.bboxMax.y
            && value.bboxMax.z == expectedBrep.bboxMax.z
            && value.diagnostics == expectedBrep.diagnostics;
    };
    int decodeFirstSuccess = -1;
    for (int failIndex = 0; failIndex < 512; ++failIndex) {
        DRW_AcisBrep output;
        DRW_AcisVertex sentinel;
        sentinel.nodeIndex = 99;
        sentinel.valid = true;
        sentinel.point.x = 99.0;
        output.vertices.push_back(sentinel);
        output.hasBBox = true;
        output.diagnostics.push_back("sentinel");
        DRW_SabParseOutcome outcome;
        outcome.complete = true;
        outcome.sourceByteCount = 99u;

        failAfter.store(failIndex, std::memory_order_relaxed);
        bool decoded = false;
        bool threw = false;
        try {
            decoded = drw_decodeAcisWireframeWithOutcome(
                modelerSab, output, outcome);
        } catch (...) {
            threw = true;
        }
        failAfter.store(-1, std::memory_order_relaxed);

        if (threw) {
            std::fprintf(stderr, "ACIS decode allocation %d escaped\n",
                         failIndex);
            return 8;
        }
        if (decoded) {
            if (!matchesExpectedBrep(output) || !outcome.complete) {
                std::fprintf(stderr, "ACIS decode output mismatch\n");
                return 9;
            }
            decodeFirstSuccess = failIndex;
            break;
        }
        if (!output.empty() || output.hasBBox || !output.diagnostics.empty()
            || !output.vertices.empty() || !output.edges.empty()
            || !output.faces.empty() || !output.lumps.empty()
            || !output.shells.empty()
            || !outcome.allocationFailure
            || outcome.sourceByteCount != modelerSab.size()) {
            std::fprintf(stderr,
                         "ACIS decode allocation %d rollback mismatch: "
                         "empty=%d bbox=%d diag=%zu vertices=%zu edges=%zu "
                         "faces=%zu lumps=%zu shells=%zu complete=%d "
                         "alloc=%d bytes=%zu expected=%zu\n",
                         failIndex, output.empty() ? 1 : 0,
                         output.hasBBox ? 1 : 0, output.diagnostics.size(),
                         output.vertices.size(), output.edges.size(),
                         output.faces.size(), output.lumps.size(),
                         output.shells.size(), outcome.complete ? 1 : 0,
                         outcome.allocationFailure ? 1 : 0,
                         outcome.sourceByteCount, modelerSab.size());
            return 10;
        }
        if (!drw_decodeAcisWireframeWithOutcome(modelerSab, output, outcome)
            || !outcome.complete || !matchesExpectedBrep(output)) {
            std::fprintf(stderr, "ACIS decode allocation %d retry failed\n",
                         failIndex);
            return 11;
        }
    }
    if (decodeFirstSuccess < 8) {
        std::fprintf(stderr, "ACIS decode allocation sweep too short: %d\n",
                     decodeFirstSuccess);
        return 12;
    }

    DRW_AcisBrep headerBrep;
    DRW_SabHeader expectedHeader;
    DRW_SabParseOutcome headerOutcome;
    if (!drw_decodeAcisWireframeWithHeader(
            modelerSab, headerBrep, expectedHeader, headerOutcome,
            DRW_SabParseLimits())
        || !headerOutcome.complete
        || expectedHeader.signature != "ACIS BinaryFile"
        || expectedHeader.version != 21800
        || expectedHeader.numRecords != 3
        || expectedHeader.numEntities != 2
        || expectedHeader.productId
            != "LOCAL_PRODUCT_IDENTIFIER_WITH_HEAP_STORAGE"
        || expectedHeader.acisVersion
            != "LOCAL_ACIS_VERSION_FOR_ALLOCATION_SWEEP"
        || expectedHeader.creationDate != "LOCAL_CREATION_DATE_FOR_TESTING") {
        std::fprintf(stderr, "baseline ACIS header decode failed\n");
        return 13;
    }
    const auto matchesExpectedHeader = [&expectedHeader](
            const DRW_SabHeader& value) {
        return value.signature == expectedHeader.signature
            && value.version == expectedHeader.version
            && value.numRecords == expectedHeader.numRecords
            && value.numEntities == expectedHeader.numEntities
            && value.flags == expectedHeader.flags
            && value.productId == expectedHeader.productId
            && value.acisVersion == expectedHeader.acisVersion
            && value.creationDate == expectedHeader.creationDate
            && value.unitsInMm == expectedHeader.unitsInMm
            && value.resTol == expectedHeader.resTol
            && value.norTol == expectedHeader.norTol;
    };
    const auto isResetHeader = [](const DRW_SabHeader& value) {
        return value.signature.empty() && value.version == 0
            && value.numRecords == 0 && value.numEntities == 0
            && value.flags == 0 && value.productId.empty()
            && value.acisVersion.empty() && value.creationDate.empty()
            && value.unitsInMm == 1.0 && value.resTol == 0.0
            && value.norTol == 0.0;
    };
    int headerFirstSuccess = -1;
    for (int failIndex = 0; failIndex < 512; ++failIndex) {
        DRW_AcisBrep output;
        DRW_AcisVertex sentinel;
        sentinel.nodeIndex = 99;
        sentinel.valid = true;
        sentinel.point.x = 99.0;
        output.vertices.push_back(sentinel);
        output.hasBBox = true;
        output.diagnostics.push_back("sentinel");
        DRW_SabHeader header;
        header.signature = "caller-signature";
        header.version = 99;
        header.productId = "caller-product-id";
        header.acisVersion = "caller-acis-version";
        header.creationDate = "caller-date";
        DRW_SabParseOutcome outcome;
        outcome.complete = true;
        outcome.sourceByteCount = 99u;

        failAfter.store(failIndex, std::memory_order_relaxed);
        bool decoded = false;
        bool threw = false;
        try {
            decoded = drw_decodeAcisWireframeWithHeader(
                modelerSab, output, header, outcome, DRW_SabParseLimits());
        } catch (...) {
            threw = true;
        }
        failAfter.store(-1, std::memory_order_relaxed);

        if (threw) {
            std::fprintf(stderr, "ACIS header allocation %d escaped\n",
                         failIndex);
            return 14;
        }
        if (decoded) {
            if (!matchesExpectedBrep(output) || !outcome.complete
                || !matchesExpectedHeader(header)) {
                std::fprintf(stderr, "ACIS header decode output mismatch\n");
                return 15;
            }
            headerFirstSuccess = failIndex;
            break;
        }
        if (!output.empty() || !output.vertices.empty() || output.hasBBox
            || !output.diagnostics.empty() || !isResetHeader(header)
            || !outcome.allocationFailure
            || outcome.sourceByteCount != modelerSab.size()) {
            std::fprintf(stderr,
                         "ACIS header allocation %d did not roll back\n",
                         failIndex);
            return 16;
        }
        if (!drw_decodeAcisWireframeWithHeader(
                modelerSab, output, header, outcome, DRW_SabParseLimits())
            || !outcome.complete || !matchesExpectedBrep(output)
            || !matchesExpectedHeader(header)) {
            std::fprintf(stderr, "ACIS header allocation %d retry failed\n",
                         failIndex);
            return 17;
        }
    }
    if (headerFirstSuccess < 8) {
        std::fprintf(stderr, "ACIS header allocation sweep too short: %d\n",
                     headerFirstSuccess);
        return 18;
    }
    std::printf("SAT allocation rollback/retry passed at %d points; "
                "ACIS decode rollback/retry passed at %d points; "
                "ACIS header decode rollback/retry passed at %d points\n",
                firstSuccess, decodeFirstSuccess, headerFirstSuccess);
    return 0;
}
