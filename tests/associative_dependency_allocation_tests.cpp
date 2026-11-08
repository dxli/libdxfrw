// SPDX-License-Identifier: GPL-2.0-or-later
// Generated associative-dependency OBJECTS allocation rollback and retry.
#include "lc3_compat_check.cpp"
#include "dx_iface.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <new>
#include <string>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0;

struct AllocationFailure {
    explicit AllocationFailure(long count) { allocationsBeforeFailure = count; }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

DRW_AssociativeObject makeDependency(bool valueDependency) {
    DRW_AssociativeObject dependency(valueDependency
        ? "ACDBASSOCVALUEDEPENDENCY" : "ACDBASSOCDEPENDENCY");
    dependency.handle = 0xE901u;
    dependency.m_classVersion = 3u;
    dependency.m_status = -7;
    dependency.m_dependencyFieldsParsed = true;
    dependency.m_dependencyWireComplete = true;
    dependency.m_dependencyIsRead = true;
    dependency.m_dependencyIsAttachedToObject = true;
    dependency.m_dependencyOrder = -2;
    dependency.m_dependencyNamePresent = true;
    dependency.m_dependencyName = "allocation-retry";
    dependency.m_dependencyBodyId = -9;
    if (valueDependency) {
        dependency.m_valueDependencyFieldsParsed = true;
        dependency.m_valueDependencyClassVersion = 0;
        dependency.m_valueDependencyName = "VALUE_DEPENDENCY";
        dependency.m_valueDependencyValueCode = 40;
        dependency.m_valueDependencyValueKind =
            DRW_AssociativeObject::EvalVariantKind::Real;
        dependency.m_valueDependencyValueReal = 12.5;
    }
    return dependency;
}

class AssociativeDependencyExport final : public LC3CompatCheck {
public:
    explicit AssociativeDependencyExport(dxfRW& output,
                                         bool valueDependency = false)
        : output(output) {
        dependency = makeDependency(valueDependency);
    }

    dxfRW& output;
    DRW_AssociativeObject dependency;
    long failAt = -1;
    long reached = 0;
    bool emitted = false;

    void writeObjects() override {
        const auto emit = [this]() {
            emitted = output.writeAssociativeDependency(&dependency);
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

bool write(AssociativeDependencyExport& exporter, dxfRW& codec, bool binary) {
    return codec.write(&exporter, DRW::AC1024, binary);
}

bool verify(const std::filesystem::path& path, bool valueDependency) {
    dx_data data;
    dx_iface reader;
    if (!reader.fileImport(path.string(), &data, false)
        || data.associativeObjects.size() != 1u)
        return false;
    const DRW_AssociativeObject& actual = data.associativeObjects.front();
    return actual.handle == 0xE901u
        && actual.m_recordName == (valueDependency
            ? "ACDBASSOCVALUEDEPENDENCY" : "ACDBASSOCDEPENDENCY")
        && actual.m_dependencyFieldsParsed
        && actual.m_dependencyWireComplete
        && actual.m_classVersion == 3u && actual.m_status == -7
        && actual.m_dependencyIsRead
        && actual.m_dependencyIsAttachedToObject
        && actual.m_dependencyOrder == -2
        && actual.m_dependencyNamePresent
        && actual.m_dependencyName == "allocation-retry"
        && actual.m_dependencyBodyId == -9
        && (!valueDependency
            || (actual.m_valueDependencyFieldsParsed
                && actual.m_valueDependencyClassVersion == 0
                && actual.m_valueDependencyName == "VALUE_DEPENDENCY"
                && actual.m_valueDependencyValueCode == 40
                && actual.m_valueDependencyValueKind
                    == DRW_AssociativeObject::EvalVariantKind::Real
                && actual.m_valueDependencyValueReal == 12.5));
}

bool sourceUnchanged(const DRW_AssociativeObject& actual,
                     const DRW_AssociativeObject& expected) {
    return actual.handle == expected.handle
        && actual.parentHandle == expected.parentHandle
        && actual.reactorHandles == expected.reactorHandles
        && actual.m_recordName == expected.m_recordName
        && actual.m_dependencyFieldsParsed
            == expected.m_dependencyFieldsParsed
        && actual.m_dependencyWireComplete
            == expected.m_dependencyWireComplete
        && actual.m_dependencyName == expected.m_dependencyName
        && actual.m_dependencyBodyId == expected.m_dependencyBodyId
        && actual.m_valueDependencyFieldsParsed
            == expected.m_valueDependencyFieldsParsed
        && actual.m_valueDependencyClassVersion
            == expected.m_valueDependencyClassVersion
        && actual.m_valueDependencyName == expected.m_valueDependencyName
        && actual.m_valueDependencyValueCode
            == expected.m_valueDependencyValueCode
        && actual.m_valueDependencyValueKind
            == expected.m_valueDependencyValueKind
        && actual.m_valueDependencyValueReal
            == expected.m_valueDependencyValueReal;
}

bool run(const std::filesystem::path& directory) {
    const auto fail = [](const char* message) {
        std::cerr << "associative dependency OBJECTS allocation retry: "
                  << message << '\n';
        return false;
    };
    if (!std::filesystem::create_directories(directory)
        && !std::filesystem::is_directory(directory))
        return fail("could not create scratch directory");
    const auto output = directory / "associative-dependency-objects.dxf";
    const std::string sentinel = "prior associative dependency bytes\n";
    dxfRW codec(output.string().c_str());
    for (const bool binary : {false, true}) {
        for (const bool valueDependency : {false, true}) {
            AssociativeDependencyExport exporter(codec, valueDependency);
            AssociativeDependencyExport expected(codec, valueDependency);
            if (!write(exporter, codec, binary)
                || !verify(output, valueDependency))
                return fail("generated baseline did not read back typed dependency fields");
            exporter.failAt = 1000000;
            if (!write(exporter, codec, binary)
                || !verify(output, valueDependency))
                return fail("allocation calibration write/readback failed");
            const long allocationCount = exporter.reached;
            if (allocationCount <= 0)
                return fail("calibration observed no callback allocations");
            for (long offset = 0; offset < allocationCount; ++offset) {
                {
                    std::ofstream prior(output,
                                        std::ios::binary | std::ios::trunc);
                    prior << sentinel;
                    if (!prior)
                        return fail("could not initialize destination sentinel");
                }
                exporter.failAt = offset;
                exporter.reached = 0;
                exporter.emitted = false;
                const DRW_AssociativeObject sourceBefore = exporter.dependency;
                const bool wrote = write(exporter, codec, binary);
                exporter.failAt = -1;
                std::string current;
                if (wrote || exporter.reached < 1
                    || !readAll(output, current) || current != sentinel)
                    return fail("injected allocation published bytes or was missed");
                if (!sourceUnchanged(exporter.dependency, sourceBefore)
                    || !sourceUnchanged(exporter.dependency,
                                        expected.dependency))
                    return fail("injected failure changed the caller object");
                if (!write(exporter, codec, binary)
                    || !verify(output, valueDependency))
                    return fail("same-codec retry failed typed readback");
                if (!sourceUnchanged(exporter.dependency,
                                     expected.dependency))
                    return fail("retry changed the caller object");
            }
            std::cout << "AC1024 "
                      << (valueDependency ? "value" : "base")
                      << " associative dependency OBJECTS "
                      << (binary ? "Binary" : "ASCII")
                      << ": allocation offsets=" << allocationCount
                      << ", rollback/retry/readback=PASS\n";
        }
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
        std::cerr << "usage: associative_dependency_allocation_tests <scratch-dir>\n";
        return 2;
    }
    return run(argv[1]) ? 0 : 1;
}
