// SPDX-License-Identifier: GPL-2.0-or-later
// Isolated fail-once allocation sweep for the generated TABLE extension facade.
#define LIBDXFRW_TABLE_FACADE_ALLOC_SWEEP 1
#define main libdxfrw_unused_roundtrip_main
#include "dwg_local_roundtrip_tests.cpp"
#undef main

#include <cstdlib>
#include <new>

namespace {
bool allocationProbeEnabled = false;
bool allocationTracking = false;
long allocationFailureOffset = -1;
std::size_t allocationAttempts = 0u;
bool allocationFailed = false;
int allocationProbeStage = 0;
void tableFacadeAllocationBegin(int stage) {
    if (allocationProbeEnabled && allocationProbeStage == stage)
        allocationTracking = true;
}
void tableFacadeAllocationEnd() { allocationTracking = false; }
}

void* operator new(std::size_t size) {
    if (allocationTracking) {
        ++allocationAttempts;
        if (allocationFailureOffset == 0) {
            allocationFailureOffset = -1; // rollback and retry can allocate
            allocationFailed = true;
            throw std::bad_alloc();
        }
        if (allocationFailureOffset > 0)
            --allocationFailureOffset;
    }
    if (void* p = std::malloc(size == 0u ? 1u : size))
        return p;
    throw std::bad_alloc();
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
void operator delete[](void* p, std::size_t) noexcept { std::free(p); }

int main() {
    const auto path = std::filesystem::temp_directory_path()
        / "libdxfrw-table-extension-allocation-sweep.dwg";
    struct Cleanup {
        std::filesystem::path path;
        ~Cleanup() { std::error_code ignored; std::filesystem::remove(path, ignored); }
    } cleanup{path};
    const std::string sentinel = "TABLE extension allocation rollback\n";
    auto seed = [&]() {
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        out << sentinel;
        return static_cast<bool>(out);
    };
    auto unchanged = [&]() {
        std::ifstream in(path, std::ios::binary);
        return std::string((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>()) == sentinel;
    };
    dx_data calibrationData;
    dwgRW calibration(path.string().c_str());
    TableStyleNodWholeFileAdapter baseline(&calibration, &calibrationData,
                                            true, false, false);
    if (!calibration.reserveHandle(baseline.kExtensionHandle)) return 1;
    baseline.setExtensionMode(6);
    allocationProbeEnabled = true;
    allocationAttempts = 0u;
    const bool calibrationOk = calibration.write(&baseline, DRW::AC1018, true);
    allocationProbeEnabled = false;
    const std::size_t count = allocationAttempts;
    if (!calibrationOk || !baseline.ready || count == 0u) {
        std::cerr << "TABLE extension allocation calibration failed\n";
        return 1;
    }
    for (std::size_t offset = 0u; offset < count; ++offset) {
        dx_data data;
        dwgRW writer(path.string().c_str());
        TableStyleNodWholeFileAdapter producer(&writer, &data, true, false, false);
        if (!writer.reserveHandle(producer.kExtensionHandle) || !seed()) return 1;
        producer.setExtensionMode(6);
        allocationAttempts = 0u;
        allocationFailed = false;
        allocationFailureOffset = static_cast<long>(offset);
        allocationProbeEnabled = true;
        const bool wrote = writer.write(&producer, DRW::AC1018, true);
        allocationProbeEnabled = false;
        allocationTracking = false;
        if (!allocationFailed || wrote || !unchanged()
            || producer.table.handle != 0xA720u
            || producer.table.blockRecH.ref != 0u
            || producer.table.getDwgClassNum() != 0u) {
            std::cerr << "TABLE extension allocation rollback failed offset="
                      << offset << " fired=" << allocationFailed
                      << " wrote=" << wrote << '\n';
            return 1;
        }
        allocationFailureOffset = -1;
        if (!writer.write(&producer, DRW::AC1018, true) || !producer.ready) {
            std::cerr << "TABLE extension allocation retry failed offset="
                      << offset << '\n';
            return 1;
        }
        dx_data readback;
        dx_iface reader;
        reader.cData = &readback;
        dwgRW input(path.string().c_str());
        if (!input.read(&reader, false) || readback.tables.size() != 1u
            || readback.tables.front().xDictHandle != producer.kExtensionHandle) {
            std::cerr << "TABLE extension allocation readback failed offset="
                      << offset << '\n';
            return 1;
        }
        const auto priorBlock = std::find_if(readback.blocks.cbegin(),
            readback.blocks.cend(), [](const dx_ifaceBlock* block) {
                return block != nullptr && block->name == "*T1";
            });
        if (priorBlock == readback.blocks.cend()
            || (*priorBlock)->ent.size() != 15u) {
            std::cerr << "TABLE extension prior block readback failed offset="
                      << offset << '\n';
            return 1;
        }
    }
    for (const int mode : {20, 21, 22, 23}) {
        dx_data data;
        dwgRW writer(path.string().c_str());
        TableStyleNodWholeFileAdapter producer(&writer, &data, true, false, false);
        if (!writer.reserveHandle(producer.kExtensionHandle) || !seed()) return 1;
        producer.setExtensionMode(mode);
        allocationProbeStage = 1;
        allocationAttempts = 0u;
        allocationFailed = false;
        allocationFailureOffset = 0;
        allocationProbeEnabled = true;
        const bool wrote = writer.write(&producer, DRW::AC1018, true);
        allocationProbeEnabled = false;
        allocationTracking = false;
        if (!allocationFailed || !producer.caughtException()
            || ((mode == 20 || mode == 22) && (wrote || !unchanged()))
            || ((mode == 21 || mode == 23) && (!wrote || !producer.ready))) {
            std::cerr << "caught duplicate allocation failure mismatch mode="
                      << mode << " fired=" << allocationFailed
                      << " caught=" << producer.caughtException()
                      << " wrote=" << wrote << '\n';
            return 1;
        }
        if (mode == 20 || mode == 22) {
            allocationFailureOffset = -1;
            producer.setExtensionMode(6);
            if (!writer.write(&producer, DRW::AC1018, true) || !producer.ready) {
                std::cerr << "caught duplicate retry failed\n";
                return 1;
            }
        }
    }
    std::cout << "Caught fixed/generic allocation exceptions: "
                 "refusal, explicit rollback and retry PASS\n";
    std::cout << "TABLE extension facade allocation failures=" << count
              << " rollback/retry/readback: PASS\n";
    return 0;
}
