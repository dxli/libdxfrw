// SPDX-License-Identifier: GPL-2.0-or-later
#include "lc3_compat_check.cpp"
#include "libdwgr.h"
#include "intern/dwgwriter24.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <sstream>

class DrwHeaderEncodeTestAccess {
public:
    static bool encode(DRW_Header& h, DRW::Version v, dwgBufferW& data,
                       dwgBufferW& handles, dwgBufferW& strings,
                       std::uint32_t dict, std::uint32_t current) {
        return h.encodeDwgForWriterMaterial(v, &data, &handles, &strings,
            DRW::DwgNamedObjectsDictionaryHandle, 0, nullptr,
            DRW::DwgAcadGroupDictionaryHandle, 0, dict, current);
    }
};

class DwgBlockRecordXrefWireTestAccess {
public:
    static void failAfterDictionary(dwgWriter15& w) {
        w.setWriteFailurePointForTest(dwgWriter15::kAfterDefaultMaterialDictionary);
    }
    static bool tokenConflict(dwgWriter15& w, std::uint32_t owner,
                              std::uint8_t code, std::uint32_t reference) {
        w.m_currentHandle=owner; w.m_objectHandles.reset();
        dwgHandle token; token.code=code; token.ref=reference;
        w.m_objectHandles.putHandle(token);
        return w.rejectDefaultDwgMaterialFrameConflicts();
    }
};

namespace {
struct ThrowingWriter : dwgWriter24 {
    using dwgWriter24::dwgWriter24;
    CompoundWriteCheckpoint checkpointPublicTransaction() const override {
        if (failAdmission) throw std::bad_alloc();
        return dwgWriter24::checkpointPublicTransaction();
    }
    CompoundWriteCheckpoint checkpointCompoundWrite() const override {
        if (failEmission) throw std::bad_alloc();
        return dwgWriter24::checkpointCompoundWrite();
    }
    bool failAdmission = false, failEmission = false;
};
bool headerWireControls() {
    for (auto version : {DRW::AC1015, DRW::AC1018, DRW::AC1021,
                         DRW::AC1024, DRW::AC1027, DRW::AC1032}) {
        DRW_Header header; dwgBufferW data, handles, strings;
        if (!DrwHeaderEncodeTestAccess::encode(header, version, data, handles, strings,
                                               0xD1u, 0xD2u)) return false;
        const auto& tokens=handles.handleOccurrences();
        const auto count=[&tokens](std::uint32_t reference) {
            return std::count_if(tokens.begin(), tokens.end(), [reference](const auto& token) {
                return token.reference==reference && token.code==DRW::DwgHardPointer;
            });
        };
        if (count(0xD1u)!=(version>DRW::AC1015?1:0)
            || count(0xD2u)!=(version>DRW::AC1018?1:0)) return false;
        if (version>DRW::AC1018 && (tokens.size()<5u || tokens[3].reference!=0xD2u
            || tokens[3].code!=DRW::DwgHardPointer)) return false;
        if (version>DRW::AC1015) {
            const auto dict=std::find_if(tokens.begin(),tokens.end(),[](const auto& token) {
                return token.reference==0xD1u;
            });
            if(dict==tokens.end() || dict==tokens.begin() || dict+1==tokens.end()
                || (dict-1)->code!=DRW::DwgHardPointer || (dict-1)->reference!=0u
                || (dict+1)->code!=DRW::DwgHardPointer || (dict+1)->reference!=0u) return false;
        }
    }
    return true;
}
bool directControls() {
    if (!headerWireControls()) return false;
    for (const auto code : {6u, 8u, 10u, 12u, 9u}) {
        DRW_Header relativeHeader;
        ThrowingWriter relative(nullptr, &relativeHeader);
        if (!relative.stageDefaultDwgMaterialGraph()) return false;
        const auto target=relative.defaultDwgMaterialGraphHandles()[2];
        const auto owner=(code==6u||code==10u)?target-1u:target+1u;
        const auto delta=(code==10u||code==12u)?1u:0u;
        if (!DwgBlockRecordXrefWireTestAccess::tokenConflict(
                relative, owner, static_cast<std::uint8_t>(code), delta)) return false;
    }
    DRW_Header header;
    ThrowingWriter writer(nullptr, &header);
    auto initial = writer.checkpointPublicTransaction();
    if (!writer.registerDwgNamedObjectDictionaryEntry("acad_material", 0xB100u)) return false;
    auto high = writer.highWaterHandle();
    if (writer.stageDefaultDwgMaterialGraph() || writer.highWaterHandle() != high) return false;
    writer.rollbackCompoundWrite(initial);
    high = writer.highWaterHandle(); writer.failAdmission = true;
    if (writer.stageDefaultDwgMaterialGraph() || writer.highWaterHandle() != high) return false;
    writer.failAdmission = false;
    if (!writer.stageDefaultDwgMaterialGraph()) return false;
    const auto handles = writer.defaultDwgMaterialGraphHandles();
    for (std::size_t i=0; i<handles.size(); ++i)
        if (handles[i] != high+i) return false;
    if (!writer.prepareDwgClassManifest() || !writer.writeFileHeaderStub()
        || !writer.writeDwgHeader() || !writer.writeDwgClasses()) return false;
    writer.freezeDwgClasses();
    if (!writer.writeDwgObjects()) return false;
    const auto checkpoint = writer.checkpointCompoundWrite();
    const auto bytes = writer.buffer();
    writer.failEmission = true;
    if (writer.completeDeferredDwgObjects() || writer.buffer() != bytes) return false;
    writer.failEmission = false;
    DwgBlockRecordXrefWireTestAccess::failAfterDictionary(writer);
    if (writer.completeDeferredDwgObjects() || writer.buffer() != bytes) return false;
    const auto after = writer.checkpointCompoundWrite();
    if (checkpoint.objectMapSize != after.objectMapSize
        || checkpoint.defaultMaterialHandles != after.defaultMaterialHandles
        || checkpoint.defaultMaterialGraphEmitted != after.defaultMaterialGraphEmitted
        || checkpoint.classInstanceEmittedHandles != after.classInstanceEmittedHandles
        || checkpoint.classInstanceEmissionCounts != after.classInstanceEmissionCounts)
        return false;
    return writer.completeDeferredDwgObjects()
        && writer.validateDwgNamedObjectDictionaryEntries()
        && writer.validateDwgClassInstanceCounts() && writer.validateDwgClassInstanceFrames();
}

enum class Mode { Normal, EarlyRoot, SourceRoot, HeaderInvalid, HeaderGlobal, EarlyMaterialClass, LateMaterialClass,
                  EarlyRawSection, LateRawSection, LateRoot, NodAlias, GroupAlias, DictionaryOwner,
                  DictionaryChild, DictionaryHandle, XrecordOwner, RawObject, CallerMaterial,
                  SuppliedLayer, LateEntityHandle };
struct Adapter : LC3CompatCheck, DRW_DwgDefaultMaterialProvider, DRW_DwgDefaultPlotStyleProvider {
    explicit Adapter(dwgRW& writer) : output(writer) {}
    bool wantsDwgDefaultMaterialGraph() const override { return enabled; }
    bool dwgDefaultMaterialOpaqueReferencesAbsent(std::uint32_t a, std::uint32_t b,
            std::uint32_t c, std::uint32_t d) const override {
        return opaqueAbsent && a != 0 && b != 0 && c != 0 && d != 0;
    }
    bool wantsDwgDefaultPlotStyleGraph() const override { return true; }
    void dwgDefaultMaterialGraphPlanned(std::uint32_t a, std::uint32_t b,
                                        std::uint32_t c, std::uint32_t d) override {
        handles = {a,b,c,d};
        if (mode == Mode::HeaderGlobal && header != nullptr) {
            std::ostringstream value; value << std::hex << d;
            header->addStr("$CMATERIAL", value.str(), 347);
        }
    }
    void writeHeader(DRW_Header& h) override {
        header=&h;
        for (const auto* key : {"$CMATERIAL", "CMATERIAL"}) {
            const auto it=h.vars.find(key);
            if (it!=h.vars.end()) { delete it->second; h.vars.erase(it); }
        }
        if (mode == Mode::HeaderInvalid) h.addStr("$CMATERIAL", "B200", 347);
    }
    void writeDwgClasses() override {
        if (mode == Mode::EarlyRoot || mode == Mode::SourceRoot)
            (void)output.registerDwgNamedObjectDictionaryEntry("acad_material", 0xB100u);
        if (mode == Mode::EarlyMaterialClass) {
            DRW_Material material; material.handle=0xB101u;
            (void)output.registerMaterialObjectClass(&material);
        }
        if (mode == Mode::EarlyRawSection) {
            DRW_RawDwgSection section; section.m_name="AcDb:OpaqueProbe";
            section.m_version=version; section.m_data={1,2,3};
            (void)output.writeRawDwgSection(&section);
        }
    }
    void writeLTypes() override {
        if (mode == Mode::LateMaterialClass) {
            DRW_Material material; material.handle=0xB101u;
            rejected=!output.registerMaterialObjectClass(&material);
        }
        if (mode == Mode::LateRawSection) {
            DRW_RawDwgSection section; section.m_name="AcDb:OpaqueProbe";
            section.m_version=version; section.m_data={1,2,3};
            rejected=!output.writeRawDwgSection(&section);
        }
        if (mode == Mode::LateRoot)
            rejected = !output.registerDwgNamedObjectDictionaryEntry("aCaD_mAtErIaL", 0xB100u);
        if (mode == Mode::NodAlias)
            rejected = !output.registerDwgNamedObjectDictionaryEntry("ALIAS", handles[slot]);
        if (mode == Mode::GroupAlias)
            rejected = !output.registerDwgGroupDictionaryEntry("ALIAS", handles[slot]);
    }
    void writeLayers() override {
        if (mode == Mode::SuppliedLayer) {
            DRW_Layer layer; layer.name="0"; layer.materialHandle.ref=0xB200u;
            supplied = output.addLayer(&layer);
        }
    }
    void writeEntities() override {
        DRW_Line line; line.basePoint={1.,2.,3.}; line.secPoint={4.,5.,6.};
        if (mode == Mode::LateEntityHandle) line.handle=handles[slot];
        (void)output.writeLine(&line);
    }
    void writeObjects() override {
        if (mode == Mode::SourceRoot) {
            DRW_Dictionary source; source.handle=0xB100u;
            source.parentHandle=DRW::DwgNamedObjectsDictionaryHandle;
            (void)output.writeDictionary(&source);
        }
        if (mode == Mode::DictionaryOwner || mode == Mode::DictionaryChild || mode == Mode::DictionaryHandle) {
            DRW_Dictionary d; d.handle=0xC500; d.parentHandle=DRW::DwgNamedObjectsDictionaryHandle;
            if (mode == Mode::DictionaryOwner) d.parentHandle=handles[slot];
            if (mode == Mode::DictionaryChild) d.m_entries.push_back({"illegal",handles[slot]});
            if (mode == Mode::DictionaryHandle) d.handle=handles[slot];
            rejected = !output.writeDictionary(&d);
        }
        if (mode == Mode::XrecordOwner) {
            DRW_XRecord x; x.handle=0xC501; x.parentHandle=handles[slot];
            rejected = !output.writeXRecord(&x);
        }
        if (mode == Mode::CallerMaterial) {
            DRW_Material m; m.handle=0xC502; rejected=!output.writeMaterial(&m);
        }
        if (mode == Mode::RawObject) {
            DRW_UnsupportedObject raw; rejected=!output.writeRawDwgObject(&raw);
        }
    }
    dwgRW& output; DRW_Header* header=nullptr;
    DRW::Version version=DRW::AC1024;
    bool enabled=true, opaqueAbsent=true, rejected=false, supplied=false;
    Mode mode=Mode::Normal; std::size_t slot=0;
    std::array<std::uint32_t,4> handles{};
};
struct Capture : LC3CompatCheck {
    void addNamedObjectDictionary(const DRW_Dictionary& value) override {
        for (const auto& e:value.m_entries)
            if(e.m_name=="ACAD_MATERIAL" || e.m_name=="acad_material") root=e.m_handle;
    }
    void addDictionary(const DRW_Dictionary& value) override {
        if(value.m_entries.size()!=3u) return;
        if(value.m_entries[0].m_name=="ByBlock" && value.m_entries[1].m_name=="ByLayer"
           && value.m_entries[2].m_name=="Global") {
            dictionary=value.handle; owner=value.parentHandle;
            for(std::size_t i=0;i<3;++i) children[i]=value.m_entries[i].m_handle;
        }
    }
    void addMaterial(const DRW_Material& m) override {
        if (m.parentHandle!=dictionary
            || m.reactorHandles!=std::vector<std::uint32_t>{dictionary}
            || (requireSemantic && (!m.hasCompleteDwgSemanticFields()
                || m.m_diffuseColorValue!=0xc2ffffffu || m.m_channelFlags!=63))
            || (!requireSemantic && (m.hasCompleteDwgSemanticFields()
                || !m.hasDwgRawData() || m.dwgRawDataVersion()!=DRW::AC1021
                || m.dwgRawDataBitSize()!=550u))) valid=false;
        materials.push_back({m.m_name,m.handle});
    }
    void addLayer(const DRW_Layer& layer) override {
        if(layer.name=="0") layerMaterial=layer.materialHandle.ref;
    }
    bool matches(const std::array<std::uint32_t,4>& h, bool supplied) const {
        if (!valid || root!=h[0] || dictionary!=h[0] || owner!=DRW::DwgNamedObjectsDictionaryHandle
            || children!=std::array<std::uint32_t,3>{h[1],h[2],h[3]}
            || materials.size()!=3u || layerMaterial!=(supplied?0xB200u:h[3])) return false;
        const std::array<std::string,3> names{"ByBlock","ByLayer","Global"};
        for(std::size_t i=0;i<3;++i)
            if(std::find(materials.begin(),materials.end(),std::make_pair(names[i],h[i+1]))==materials.end()) return false;
        return true;
    }
    std::uint32_t root=0,dictionary=0,owner=0,layerMaterial=0;
    std::array<std::uint32_t,3> children{};
    std::vector<std::pair<std::string,std::uint32_t>> materials;
    bool valid=true, requireSemantic=true;
};
bool inspect(const std::string& path,const std::array<std::uint32_t,4>& h,bool supplied,
             DRW::Version version) {
    dwgRW reader(path.c_str()); Capture capture;
    // AC1021 retains MATERIAL semantic fields as opaque data; peer readback
    // separately checks those fields for the bounded source-derived profile.
    capture.requireSemantic = version != DRW::AC1021;
    return reader.read(&capture,false) && capture.matches(h,supplied);
}
}
int main(int argc,char**argv) {
    const bool keep=argc==2 && std::string(argv[1])=="--keep";
    if(argc>2 || (argc==2 && !keep) || !directControls()) return 1;
    const auto dir=std::filesystem::temp_directory_path() /
        ("libdxfrw-material-graph-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    if (!std::filesystem::create_directory(dir)) return 1;
    struct Cleanup {std::filesystem::path p; bool keep; ~Cleanup(){if(!keep){std::error_code e;std::filesystem::remove_all(p,e);}}} cleanup{dir,keep};
    for (const auto version : {DRW::AC1021, DRW::AC1024}) {
        const auto path=(dir/(version == DRW::AC1021 ? "graph-ac1021.dwg" : "graph-ac1024.dwg")).string(); dwgRW output(path.c_str());
        if(!output.reserveHandle(0x30u)||!output.reserveHandle(0x31u)||!output.reserveHandle(0xB200u)) return 1;
        Adapter adapter(output);
        adapter.version=version;
        if(!output.write(&adapter,version,true)||!inspect(path,adapter.handles,false,version)) {std::cerr<<"normal failed\n";return 1;}
        for(auto h:adapter.handles) if(h==0x30u||h==0x31u||h==0xB200u) return 1;
        adapter.mode=Mode::SuppliedLayer;
        if(!output.write(&adapter,version,true)||!adapter.supplied||!inspect(path,adapter.handles,true,version)) {std::cerr<<"supplied layer failed\n";return 1;}
        adapter.mode=Mode::HeaderGlobal;
        if(!output.write(&adapter,version,true)||!inspect(path,adapter.handles,false,version))return 1;
        if(keep) std::filesystem::copy_file(path,dir/(version == DRW::AC1021 ? "graph-global-ac1021.dwg" : "graph-global-ac1024.dwg"));
        for(Mode mode:{Mode::EarlyRoot,Mode::HeaderInvalid,Mode::EarlyMaterialClass,Mode::LateMaterialClass,
             Mode::EarlyRawSection,Mode::LateRawSection,Mode::LateRoot,Mode::NodAlias,Mode::GroupAlias,
             Mode::DictionaryOwner,Mode::DictionaryChild,Mode::DictionaryHandle,Mode::XrecordOwner,
             Mode::RawObject,Mode::CallerMaterial,Mode::LateEntityHandle}) {
            const bool slots=mode==Mode::NodAlias||mode==Mode::GroupAlias||mode==Mode::DictionaryOwner
                ||mode==Mode::DictionaryChild||mode==Mode::DictionaryHandle||mode==Mode::XrecordOwner
                ||mode==Mode::LateEntityHandle;
            for(std::size_t slot=0;slot<(slots?4u:1u);++slot) {
                {std::ofstream f(path,std::ios::binary);f<<"prior destination";}
                adapter.mode=mode;adapter.slot=slot;adapter.rejected=false;
                if(output.write(&adapter,version,true)) {std::cerr<<"collision published mode="<<int(mode)<<" slot="<<slot<<"\n";return 1;}
                std::ifstream f(path,std::ios::binary);if(std::string(std::istreambuf_iterator<char>(f),{})!="prior destination") return 1;
                adapter.mode=Mode::Normal;
                if(!output.write(&adapter,version,true)||!inspect(path,adapter.handles,false,version)) {std::cerr<<"retry failed mode="<<int(mode)<<" slot="<<slot<<"\n";return 1;}
            }
        }
        adapter.mode=Mode::Normal;
        adapter.opaqueAbsent=false;
        {std::ofstream f(path,std::ios::binary);f<<"prior destination";}
        if(output.write(&adapter,version,true)) return 1;
        {std::ifstream f(path,std::ios::binary);if(std::string(std::istreambuf_iterator<char>(f),{})!="prior destination")return 1;}
        adapter.opaqueAbsent=true;
        if(!output.write(&adapter,version,true)||!inspect(path,adapter.handles,false,version))return 1;
        for (const auto unsupported : {DRW::AC1015, DRW::AC1018, DRW::AC1027, DRW::AC1032}) {
            {std::ofstream f(path,std::ios::binary);f<<"prior destination";}
            if(output.write(&adapter,unsupported,true)) return 1;
            std::ifstream f(path,std::ios::binary);
            if(std::string(std::istreambuf_iterator<char>(f),{})!="prior destination")return 1;
        }
        adapter.enabled=false;
        if(!output.write(&adapter,version,true))return 1;
        dwgRW reader(path.c_str());Capture capture;
        if(!reader.read(&capture,false)||capture.root!=0||!capture.materials.empty())return 1;
        adapter.mode=Mode::SourceRoot;
        if(!output.write(&adapter,version,true))return 1;
        {dwgRW sourceReader(path.c_str()); Capture source;
         if(!sourceReader.read(&source,false)||source.root!=0xB100u||!source.materials.empty())return 1;}
        adapter.mode=Mode::Normal; adapter.enabled=true;
        if(!output.write(&adapter,version,true)||!inspect(path,adapter.handles,false,version))return 1;
        if(keep)std::cout<<"OUTPUT "<<path<<'\n';
    }
    std::cout<<"DWG default material opt-in PASS\n";return 0;
}
