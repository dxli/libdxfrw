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
    std::printf("SAT allocation rollback/retry passed at %d allocation points\n",
                firstSuccess);
    return 0;
}
