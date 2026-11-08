// SPDX-License-Identifier: GPL-2.0-or-later
// Generated DATALINK OBJECTS allocation-failure rollback and retry control.
#include "lc3_compat_check.cpp"
#include "dx_iface.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <string>
#include <utility>
#include <vector>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0;

struct AllocationFailure {
    explicit AllocationFailure(long count) { allocationsBeforeFailure = count; }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

class DataLinkExport final : public LC3CompatCheck {
public:
    explicit DataLinkExport(dxfRW& output) : output(output) {
        dictionary.handle = 0xE000u;
        DRW_Dictionary::Entry entry;
        entry.m_name = "MainLink";
        entry.m_handle = 0xE001u;
        dictionary.m_entries.push_back(std::move(entry));

        mainLink.handle = 0xE001u;
        mainLink.parentHandle = dictionary.handle;
        mainLink.m_dataAdapter = "EXCEL";
        mainLink.m_description = "generated DATALINK with custom references";
        mainLink.m_tooltip = "rollback control";
        mainLink.m_connectionString = "inert://generated-control.xlsx";
        mainLink.m_option = 7;
        mainLink.m_updateOption = 511;
        mainLink.m_value92 = -3;
        mainLink.m_year = 2026;
        mainLink.m_month = 11;
        mainLink.m_day = 8;
        mainLink.m_hour = 14;
        mainLink.m_minute = 30;
        mainLink.m_seconds = 12;
        mainLink.m_milliseconds = 345;
        mainLink.m_pathOption = 3;
        mainLink.m_value93 = 19;
        mainLink.m_updateStatus = "ready";
        mainLink.m_customDataCount = 2;
        mainLink.m_customData.push_back({0xE002u, "sheet target alpha"});
        mainLink.m_customData.push_back({0xE003u, "sheet target beta"});

        targetAlpha.handle = 0xE002u;
        targetAlpha.parentHandle = dictionary.handle;
        targetAlpha.m_dataAdapter = "EXCEL";
        targetAlpha.m_description = "alpha target";
        targetBeta.handle = 0xE003u;
        targetBeta.parentHandle = dictionary.handle;
        targetBeta.m_dataAdapter = "EXCEL";
        targetBeta.m_description = "beta target";
    }

    dxfRW& output;
    DRW_Dictionary dictionary;
    DRW_DataLink mainLink;
    DRW_DataLink targetAlpha;
    DRW_DataLink targetBeta;
    long failAt = -1;
    long reached = 0;
    bool emitted = false;

    void writeObjects() override {
        const auto emit = [this]() {
            emitted = output.writeDictionary(&dictionary)
                && output.writeDataLink(&mainLink)
                && output.writeDataLink(&targetAlpha)
                && output.writeDataLink(&targetBeta);
        };
        if (failAt < 0) {
            emit();
            return;
        }
        const auto before = failedAllocations;
        try {
            AllocationFailure failure(failAt);
            emit();
            reached = failedAllocations != before
                ? static_cast<long>(failedAllocations - before)
                : failAt - allocationsBeforeFailure;
        } catch (const std::bad_alloc&) {
            reached = failedAllocations != before
                ? static_cast<long>(failedAllocations - before)
                : failAt - allocationsBeforeFailure;
            throw;
        }
    }

    bool finalizeDxfWrite() override { return emitted; }
};

bool readAll(const std::filesystem::path& path, std::string& value) {
    std::ifstream input(path, std::ios::binary);
    if (!input)
        return false;
    value.assign(std::istreambuf_iterator<char>(input),
                 std::istreambuf_iterator<char>());
    return !input.bad();
}

bool write(DataLinkExport& exporter, dxfRW& codec, bool binary) {
    codec.setRootDictEntries({{"ACAD_DATALINK", "E000"}});
    return codec.write(&exporter, DRW::AC1027, binary);
}

bool verifyGraph(const std::filesystem::path& path) {
    dx_data data;
    dx_iface reader;
    if (!reader.fileImport(path.string(), &data, false)
        || !data.hasNamedObjectDictionary || data.dataLinks.size() != 3u)
        return false;
    bool rooted = false;
    for (const auto& entry : data.namedObjectDictionary.m_entries)
        rooted = rooted || (entry.m_name == "ACAD_DATALINK"
                            && entry.m_handle == 0xE000u);
    const auto dictionary = std::find_if(data.dictionaries.begin(),
        data.dictionaries.end(), [](const DRW_Dictionary& value) {
            return value.handle == 0xE000u;
        });
    if (!rooted || dictionary == data.dictionaries.end()
        || dictionary->parentHandle != data.namedObjectDictionary.handle
        || dictionary->m_entries.size() != 1u
        || dictionary->m_entries.front().m_name != "MainLink"
        || dictionary->m_entries.front().m_handle != 0xE001u)
        return false;
    const auto link = [&data](std::uint32_t handle) -> const DRW_DataLink* {
        const auto found = std::find_if(data.dataLinks.begin(), data.dataLinks.end(),
            [handle](const DRW_DataLink& value) {
                return value.handle == handle;
            });
        return found == data.dataLinks.end() ? nullptr : &*found;
    };
    const DRW_DataLink* main = link(0xE001u);
    const DRW_DataLink* alpha = link(0xE002u);
    const DRW_DataLink* beta = link(0xE003u);
    return main != nullptr && alpha != nullptr && beta != nullptr
        && main->parentHandle == 0xE000u
        && main->m_dataAdapter == "EXCEL"
        && main->m_description == "generated DATALINK with custom references"
        && main->m_tooltip == "rollback control"
        && main->m_connectionString == "inert://generated-control.xlsx"
        && main->m_option == 7 && main->m_updateOption == 511
        && main->m_value92 == -3 && main->m_year == 2026
        && main->m_month == 11 && main->m_day == 8
        && main->m_hour == 14 && main->m_minute == 30
        && main->m_seconds == 12 && main->m_milliseconds == 345
        && main->m_pathOption == 3 && main->m_value93 == 19
        && main->m_updateStatus == "ready"
        && main->m_customDataCount == 2
        && main->m_customData.size() == 2u
        && main->m_customData[0].m_targetHandle == 0xE002u
        && main->m_customData[0].m_text == "sheet target alpha"
        && main->m_customData[1].m_targetHandle == 0xE003u
        && main->m_customData[1].m_text == "sheet target beta"
        && alpha->parentHandle == 0xE000u
        && alpha->m_description == "alpha target"
        && beta->parentHandle == 0xE000u
        && beta->m_description == "beta target";
}

bool sourceUnchanged(const DataLinkExport& actual,
                     const DataLinkExport& expected) {
    const auto sameLink = [](const DRW_DataLink& a, const DRW_DataLink& b) {
        if (a.handle != b.handle || a.parentHandle != b.parentHandle
            || a.m_dataAdapter != b.m_dataAdapter
            || a.m_description != b.m_description || a.m_tooltip != b.m_tooltip
            || a.m_connectionString != b.m_connectionString
            || a.m_option != b.m_option || a.m_updateOption != b.m_updateOption
            || a.m_value92 != b.m_value92 || a.m_year != b.m_year
            || a.m_month != b.m_month || a.m_day != b.m_day
            || a.m_hour != b.m_hour || a.m_minute != b.m_minute
            || a.m_seconds != b.m_seconds || a.m_milliseconds != b.m_milliseconds
            || a.m_pathOption != b.m_pathOption || a.m_value93 != b.m_value93
            || a.m_updateStatus != b.m_updateStatus
            || a.m_customDataCount != b.m_customDataCount
            || a.m_customData.size() != b.m_customData.size())
            return false;
        for (std::size_t i = 0; i < a.m_customData.size(); ++i)
            if (a.m_customData[i].m_targetHandle
                    != b.m_customData[i].m_targetHandle
                || a.m_customData[i].m_text != b.m_customData[i].m_text)
                return false;
        return true;
    };
    return actual.dictionary.handle == expected.dictionary.handle
        && actual.dictionary.m_entries.size() == expected.dictionary.m_entries.size()
        && actual.dictionary.m_entries.front().m_name
            == expected.dictionary.m_entries.front().m_name
        && actual.dictionary.m_entries.front().m_handle
            == expected.dictionary.m_entries.front().m_handle
        && sameLink(actual.mainLink, expected.mainLink)
        && sameLink(actual.targetAlpha, expected.targetAlpha)
        && sameLink(actual.targetBeta, expected.targetBeta);
}

bool run(const std::filesystem::path& directory) {
    const auto fail = [](const char* message) {
        std::cerr << "DATALINK OBJECTS allocation retry: " << message << '\n';
        return false;
    };
    if (!std::filesystem::create_directories(directory)
        && !std::filesystem::is_directory(directory))
        return fail("could not create scratch directory");
    const auto output = directory / "datalink-objects.dxf";
    const std::string sentinel = "prior DATALINK destination bytes\n";
    dxfRW codec(output.string().c_str());
    DataLinkExport exporter(codec);
    DataLinkExport expected(codec);

    for (const bool binary : {false, true}) {
        if (!write(exporter, codec, binary))
            return fail("warm-up export was refused");
        exporter.failAt = 1000000;
        if (!write(exporter, codec, binary))
            return fail("allocation calibration export was refused");
        if (!verifyGraph(output))
            return fail("allocation calibration output did not read back graph");
        const long allocationCount = exporter.reached;
        if (allocationCount <= 0)
            return fail("calibration observed no callback allocations");
        for (long offset = 0; offset < allocationCount; ++offset) {
            {
                std::ofstream prior(output, std::ios::binary | std::ios::trunc);
                prior << sentinel;
                if (!prior)
                    return fail("could not initialize destination sentinel");
            }
            exporter.failAt = offset;
            exporter.reached = 0;
            exporter.emitted = false;
            const bool wrote = write(exporter, codec, binary);
            exporter.failAt = -1;
            std::string current;
            if (wrote || exporter.reached < 1
                || !readAll(output, current) || current != sentinel)
                return fail("injected failure changed destination or was missed");
            if (!sourceUnchanged(exporter, expected))
                return fail("injected failure changed the caller graph");
            if (!write(exporter, codec, binary) || !verifyGraph(output))
                return fail("same-writer retry failed typed graph readback");
            if (!sourceUnchanged(exporter, expected))
                return fail("same-writer retry changed the caller graph");
        }
        std::cout << "DATALINK OBJECTS " << (binary ? "Binary" : "ASCII")
                  << ": allocation offsets=" << allocationCount
                  << ", rollback/retry/readback=PASS\n";
    }
    std::error_code ignored;
    std::filesystem::remove(output, ignored);
    return true;
}
} // namespace

void* operator new(std::size_t size) {
    if (allocationsBeforeFailure == 0) {
        ++failedAllocations;
        throw std::bad_alloc();
    }
    if (allocationsBeforeFailure > 0)
        --allocationsBeforeFailure;
    if (void* value = std::malloc(size == 0 ? 1 : size))
        return value;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* value) noexcept { std::free(value); }
void operator delete[](void* value) noexcept { std::free(value); }
void operator delete(void* value, std::size_t) noexcept { std::free(value); }
void operator delete[](void* value, std::size_t) noexcept { std::free(value); }

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: datalink_objects_allocation_tests SCRATCH_DIR\n";
        return 2;
    }
    const auto before = failedAllocations;
    if (!run(argv[1]))
        return 1;
    if (failedAllocations == before) {
        std::cerr << "allocation fault injector did not reach operator new\n";
        return 1;
    }
    return 0;
}
