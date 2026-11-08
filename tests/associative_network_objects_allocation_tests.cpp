// SPDX-License-Identifier: GPL-2.0-or-later
// Generated associative-network OBJECTS allocation rollback and retry.
#include "lc3_compat_check.cpp"
#include "dx_iface.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <string>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0;

struct AllocationFailure {
    explicit AllocationFailure(long count) { allocationsBeforeFailure = count; }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

DRW_AssociativeObject makeNetwork() {
    DRW_AssociativeObject network("ACDBASSOCNETWORK");
    network.handle = 0xE901u;
    network.parentHandle = 0xE900u;
    network.reactorHandles = {0xE900u};
    network.m_classVersion = 1u;
    network.m_geometryStatus = 0;
    network.m_owningNetworkHandle = 0u;
    network.m_actionBodyHandle = 0u;
    network.m_actionIndex = 0;
    network.m_maxDependencyIndex = 0;
    network.m_networkVersion = 0;
    network.m_networkActionIndex = 0;
    network.m_networkFieldsParsed = true;

    DRW_AssociativePrefixStatus action;
    action.m_kind = DRW_AssociativePrefixStatus::Kind::AcDbAssocAction;
    action.m_status = DRW_AssociativePrefixStatus::ParseStatus::Complete;
    action.m_classVersion = 1u;
    action.m_decodedHandleCount = 2u;
    DRW_AssociativePrefixStatus networkStatus;
    networkStatus.m_kind =
        DRW_AssociativePrefixStatus::Kind::AcDbAssocNetwork;
    networkStatus.m_status =
        DRW_AssociativePrefixStatus::ParseStatus::Complete;
    networkStatus.m_classVersion = 1u;
    networkStatus.m_decodedHandleCount = 0u;
    networkStatus.m_decodedCountValue = 0;
    network.m_prefixStatuses = {action, networkStatus};
    return network;
}

class AssociativeNetworkExport final : public LC3CompatCheck {
public:
    explicit AssociativeNetworkExport(dxfRW& output) : output(output) {
        dictionary.handle = 0xE900u;
        DRW_Dictionary::Entry entry;
        entry.m_name = "ACAD_ASSOCNETWORK";
        entry.m_handle = 0xE901u;
        dictionary.m_entries.push_back(std::move(entry));
        network = makeNetwork();
    }

    dxfRW& output;
    DRW_Dictionary dictionary;
    DRW_AssociativeObject network;
    long failAt = -1;
    long reached = 0;
    bool emitted = false;

    void writeObjects() override {
        const auto emit = [this]() {
            emitted = output.writeDictionary(&dictionary)
                && output.writeAssociativeNetwork(&network);
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

bool write(AssociativeNetworkExport& exporter, dxfRW& codec, bool binary) {
    codec.setRootDictEntries({{"ACAD_ASSOCNETWORK", "E900"}});
    return codec.write(&exporter, DRW::AC1024, binary);
}

bool verify(const std::filesystem::path& path) {
    dx_data data;
    dx_iface reader;
    if (!reader.fileImport(path.string(), &data, false)
        || !data.hasNamedObjectDictionary
        || data.associativeObjects.size() != 1u)
        return false;
    const bool rooted = std::any_of(
        data.namedObjectDictionary.m_entries.begin(),
        data.namedObjectDictionary.m_entries.end(),
        [](const DRW_Dictionary::Entry& entry) {
            return entry.m_name == "ACAD_ASSOCNETWORK"
                && entry.m_handle == 0xE900u;
        });
    const auto dictionary = std::find_if(data.dictionaries.begin(),
        data.dictionaries.end(), [](const DRW_Dictionary& value) {
            return value.handle == 0xE900u;
        });
    if (!rooted || dictionary == data.dictionaries.end()
        || dictionary->parentHandle != data.namedObjectDictionary.handle
        || dictionary->m_entries.size() != 1u
        || dictionary->m_entries.front().m_name != "ACAD_ASSOCNETWORK"
        || dictionary->m_entries.front().m_handle != 0xE901u)
        return false;
    const DRW_AssociativeObject& network = data.associativeObjects.front();
    return network.handle == 0xE901u && network.parentHandle == 0xE900u
        && network.reactorHandles == std::vector<std::uint32_t>{0xE900u}
        && network.m_recordName == "ACDBASSOCNETWORK"
        && network.m_classVersion == 1u && network.m_networkVersion == 0
        && network.m_networkFieldsParsed && network.m_actions.empty()
        && network.m_owningNetworkHandle == 0u;
}

bool sourceUnchanged(const AssociativeNetworkExport& actual,
                     const AssociativeNetworkExport& expected) {
    const auto& a = actual.network;
    const auto& b = expected.network;
    if (actual.dictionary.handle != expected.dictionary.handle
        || actual.dictionary.m_entries.size() != 1u
        || actual.dictionary.m_entries.front().m_name
            != expected.dictionary.m_entries.front().m_name
        || actual.dictionary.m_entries.front().m_handle
            != expected.dictionary.m_entries.front().m_handle
        || a.handle != b.handle || a.parentHandle != b.parentHandle
        || a.reactorHandles != b.reactorHandles
        || a.m_owningNetworkHandle != b.m_owningNetworkHandle
        || a.m_classVersion != b.m_classVersion
        || a.m_networkVersion != b.m_networkVersion
        || a.m_actions.size() != b.m_actions.size()
        || a.m_prefixStatuses.size() != b.m_prefixStatuses.size())
        return false;
    for (std::size_t index = 0u; index < a.m_actions.size(); ++index)
        if (a.m_actions[index].m_isOwned != b.m_actions[index].m_isOwned
            || a.m_actions[index].m_handle != b.m_actions[index].m_handle)
            return false;
    for (std::size_t index = 0u; index < a.m_prefixStatuses.size(); ++index) {
        const auto& left = a.m_prefixStatuses[index];
        const auto& right = b.m_prefixStatuses[index];
        if (left.m_kind != right.m_kind || left.m_status != right.m_status
            || left.m_classVersion != right.m_classVersion
            || left.m_decodedHandleCount != right.m_decodedHandleCount
            || left.m_decodedCountValue != right.m_decodedCountValue)
            return false;
    }
    return true;
}

bool run(const std::filesystem::path& directory) {
    const auto fail = [](const char* message) {
        std::cerr << "associative-network OBJECTS allocation retry: "
                  << message << '\n';
        return false;
    };
    if (!std::filesystem::create_directories(directory)
        && !std::filesystem::is_directory(directory))
        return fail("could not create scratch directory");
    const auto output = directory / "associative-network-objects.dxf";
    const std::string sentinel = "prior associative-network destination\n";
    dxfRW codec(output.string().c_str());
    AssociativeNetworkExport exporter(codec);
    AssociativeNetworkExport expected(codec);

    for (const bool binary : {false, true}) {
        if (!write(exporter, codec, binary))
            return fail("warm-up export was refused");
        exporter.failAt = 1000000;
        if (!write(exporter, codec, binary))
            return fail("allocation calibration export was refused");
        if (!verify(output))
            return fail("calibration output did not read back typed network");
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
            if (wrote || exporter.reached < 1 || !readAll(output, current)
                || current != sentinel)
                return fail("injected failure changed destination or was missed");
            if (!sourceUnchanged(exporter, expected))
                return fail("injected failure changed caller graph");
            if (!write(exporter, codec, binary) || !verify(output))
                return fail("same-writer retry failed typed network readback");
            if (!sourceUnchanged(exporter, expected))
                return fail("same-writer retry changed caller graph");
        }
        std::cout << "associative-network OBJECTS "
                  << (binary ? "Binary" : "ASCII")
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
        std::cerr << "usage: associative_network_objects_allocation_tests <scratch-dir>\n";
        return 2;
    }
    return run(argv[1]) ? 0 : 1;
}
