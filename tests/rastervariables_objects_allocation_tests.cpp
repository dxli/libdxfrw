#include "dx_iface.h"
#include "libdwgr.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <string>
#include <vector>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0;

struct AllocationFailure {
    explicit AllocationFailure(long count) { allocationsBeforeFailure = count; }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

class RasterExport final : public dx_iface {
public:
    long failAt = -1;
    long reached = 0;

    void writeObjects() override {
        if (failAt < 0) {
            dx_iface::writeObjects();
            return;
        }
        const auto failuresBefore = failedAllocations;
        try {
            AllocationFailure failure(failAt);
            dx_iface::writeObjects();
            reached = failedAllocations != failuresBefore
                ? static_cast<long>(failedAllocations - failuresBefore)
                : failAt - allocationsBeforeFailure;
        } catch (const std::bad_alloc&) {
            reached = failedAllocations != failuresBefore
                ? static_cast<long>(failedAllocations - failuresBefore)
                : failAt - allocationsBeforeFailure;
            throw;
        }
    }
};

class RasterAuthor final : public dx_iface {
public:
    explicit RasterAuthor(dwgRW& output) : output(output) {
        cData = &storage;
        currentBlock = storage.mBlock;
        variables.handle = 0xB500u;
        variables.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        variables.m_classVersion = 0;
        variables.m_imageFrame = 0;
        variables.m_imageQuality = 1;
        variables.m_units = 1;
        variables.reactorHandles = {DRW::DwgNamedObjectsDictionaryHandle};
        variables.setDwgCommonObjectState(1, 1, false);
    }

    void writeHeader(DRW_Header& header) override { header.vars.clear(); }
    void writeDwgClasses() override {
        DRW_RasterVariables registration;
        registration.handle = variables.handle;
        rootRegistered = output.registerDwgNamedObjectDictionaryEntry(
            "ACAD_IMAGE_VARS", variables.handle);
        classRegistered = output.registerRasterVariablesObjectClass(
            &registration);
    }
    void writeObjects() override {
        variablesWritten = output.writeRasterVariables(&variables);
    }
    bool finalizeDwgWrite() override {
        return rootRegistered && classRegistered && variablesWritten;
    }
    bool finalizeDwgWriteStructure() override { return true; }

    dwgRW& output;
    dx_data storage;
    DRW_RasterVariables variables;
    bool rootRegistered = false;
    bool classRegistered = false;
    bool variablesWritten = false;
};

class RasterReadback final : public dx_iface {
public:
    void addRasterVariables(const DRW_RasterVariables& value) override {
        records.push_back(value);
    }
    std::vector<DRW_RasterVariables> records;
};

bool readAll(const std::filesystem::path& path, std::string& value) {
    std::ifstream input(path, std::ios::binary);
    if (!input)
        return false;
    value.assign(std::istreambuf_iterator<char>(input),
                 std::istreambuf_iterator<char>());
    return !input.bad();
}

bool verifyGraph(const std::filesystem::path& path) {
    dx_data data;
    RasterReadback reader;
    if (!reader.fileImport(path.string(), &data, false)
        || reader.records.size() != 1u || !data.hasNamedObjectDictionary)
        return false;
    const auto& variables = reader.records.front();
    const auto rootEntry = std::find_if(data.namedObjectDictionary.m_entries.begin(),
        data.namedObjectDictionary.m_entries.end(), [&](const DRW_Dictionary::Entry& entry) {
            return entry.m_name == "ACAD_IMAGE_VARS"
                && entry.m_handle == variables.handle;
        });
    return rootEntry != data.namedObjectDictionary.m_entries.end()
        && variables.handle == 0xB500u
        && variables.parentHandle == data.namedObjectDictionary.handle
        && variables.m_classVersion == 0
        && variables.m_imageFrame == 0
        && variables.m_imageQuality == 1
        && variables.m_units == 1
        && variables.reactorHandles
            == std::vector<std::uint32_t>{data.namedObjectDictionary.handle};
}

bool sourceRecordUnchanged(const dx_data& data,
                           const DRW_RasterVariables& expected) {
    if (data.dwgRasterVariables.size() != 1u)
        return false;
    const auto& actual = data.dwgRasterVariables.begin()->second;
    return actual.handle == expected.handle
        && actual.parentHandle == expected.parentHandle
        && actual.reactorHandles == expected.reactorHandles
        && actual.xDictHandle == expected.xDictHandle
        && actual.m_classVersion == expected.m_classVersion
        && actual.m_imageFrame == expected.m_imageFrame
        && actual.m_imageQuality == expected.m_imageQuality
        && actual.m_units == expected.m_units;
}

bool run(const std::filesystem::path& directory) {
    const auto fail = [](const char* message) {
        std::cerr << "RASTERVARIABLES OBJECTS allocation retry: "
                  << message << '\n';
        return false;
    };
    const auto source = directory / "rastervariables-source.dwg";
    const std::string sentinel = "prior destination bytes\n";
    std::array<std::filesystem::path, 2> destinations{{
        directory / "rastervariables-ascii.dxf",
        directory / "rastervariables-binary.dxf"}};
    struct Cleanup {
        std::filesystem::path source;
        std::array<std::filesystem::path, 2> destinations;
        ~Cleanup() {
            std::error_code ignored;
            std::filesystem::remove(source, ignored);
            for (const auto& path : destinations)
                std::filesystem::remove(path, ignored);
        }
    } cleanup{source, destinations};
    if (std::filesystem::exists(source)
        || std::filesystem::exists(destinations[0])
        || std::filesystem::exists(destinations[1]))
        return fail("scratch input/output path already exists");

    {
        dwgRW sourceWriter(source.string().c_str());
        RasterAuthor author(sourceWriter);
        if (!sourceWriter.write(&author, DRW::AC1027, false))
            return fail("from-scratch AC1027 source generation failed");
    }

    dx_data data;
    dx_iface importer;
    if (!importer.fileImport(source.string(), &data, false)
        || data.sourceFormat != dx_source_format::DWG
        || data.sourceVersion != DRW::AC1027
        || data.rasterVariablesCaptureFailed
        || data.dwgRasterVariables.size() != 1u
        || data.rasterVariablesRetainedRecords != 1u)
        return fail("generated source did not import one typed raster graph");
    const DRW_RasterVariables sourceRecord =
        data.dwgRasterVariables.begin()->second;

    RasterExport exporter;
    exporter.cData = &data;
    const auto write = [&](std::size_t index, bool binary) {
        return exporter.fileExport(destinations[index].string(), DRW::AC1027,
            binary, &data, false, false, dx_dxfSourceTextPolicy::Compatible,
            DRW::DxfTextWritePolicy::Compatible,
            dx_dxfProxyGraphicsPolicy::Compatible,
            dx_dwgReadCompletenessPolicy::Legacy);
    };

    for (std::size_t index = 0; index < destinations.size(); ++index) {
        const bool binary = index == 1u;
        {
            std::ofstream output(destinations[index],
                std::ios::binary | std::ios::trunc);
            output.write(sentinel.data(),
                         static_cast<std::streamsize>(sentinel.size()));
            if (!output)
                return fail("could not initialize destination sentinel");
        }
        exporter.failAt = 1000000;
        if (!write(index, binary))
            return fail("allocation calibration export failed");
        const long allocationCount = exporter.reached;
        if (allocationCount <= 0 || !verifyGraph(destinations[index]))
            return fail("calibration did not emit the typed raster graph");

        unsigned offsets = 0u;
        for (long offset = 0; offset < allocationCount; ++offset) {
            {
                std::ofstream output(destinations[index],
                    std::ios::binary | std::ios::trunc);
                output.write(sentinel.data(),
                             static_cast<std::streamsize>(sentinel.size()));
                if (!output)
                    return fail("could not reset destination sentinel");
            }
            exporter.failAt = offset;
            exporter.reached = 0;
            const bool wrote = write(index, binary);
            exporter.failAt = -1;
            std::string current;
            if (wrote || exporter.reached < 1
                || !readAll(destinations[index], current)
                || current != sentinel
                || !sourceRecordUnchanged(data, sourceRecord))
                return fail("injected failure changed destination or caller record");
            if (!write(index, binary) || !verifyGraph(destinations[index])
                || !sourceRecordUnchanged(data, sourceRecord))
                return fail("same-adapter retry failed typed graph readback");
            ++offsets;
        }
        if (offsets != static_cast<unsigned>(allocationCount))
            return fail("not every calibrated callback allocation was injected");
        std::cout << "RASTERVARIABLES OBJECTS "
                  << (binary ? "Binary" : "ASCII")
                  << ": allocation offsets=" << allocationCount
                  << ", rollback/retry/readback=" << offsets << '\n';
    }
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
        std::cerr << "usage: rastervariables_objects_allocation_tests SCRATCH_DIR\n";
        return 2;
    }
    const std::filesystem::path directory(argv[1]);
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    if (error) {
        std::cerr << "could not create scratch directory: " << error.message() << '\n';
        return 2;
    }
    const auto before = failedAllocations;
    if (!run(directory))
        return 1;
    if (failedAllocations == before) {
        std::cerr << "allocation fault injector did not reach operator new\n";
        return 1;
    }
    return 0;
}
