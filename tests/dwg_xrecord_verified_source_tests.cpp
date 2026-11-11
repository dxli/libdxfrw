#include "dx_iface.h"
#include "intern/dwgbufferw.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

class DxIfaceStandardDwgCaptureTestAccess {
public:
    static DRW_UnsupportedObject& raw(dx_data& d, std::uint32_t h) {
        return d.m_standardDwgObjectFrames.at({d.importGeneration,h}).raw;
    }
    static DRW_DwgFramePublication& publication(dx_data& d, std::uint32_t h) {
        return d.m_standardDwgObjectFrames.at({d.importGeneration,h}).publication;
    }
    static void transplant(dx_data& to, const dx_data& from) {
        to.m_verifiedXRecordSource = from.m_verifiedXRecordSource;
    }
};
namespace {
constexpr std::uint32_t handle=0xA602u;
class Writer final : public dx_iface {
public:
    explicit Writer(dwgRW& w):w_(w) {}
    void writeHeader(DRW_Header& h) override {h.vars.clear();}
    void writeDwgClasses() override {
        registered=w_.registerDwgNamedObjectDictionaryEntry("SOURCE",handle)
            && w_.registerDwgNamedObjectDictionaryEntry("FOLLOWING",handle+1u);
    }
    void collectDwgAppIds() override {}
    void writeBlocks() override {}
    void writeBlockRecords() override {}
    void writeEntities() override {}
    void writeLTypes() override {}
    void writeLayers() override {}
    void writeTextstyles() override {}
    void writeVports() override {}
    void writeViews() override {}
    void writeUCSs() override {}
    void writeDimstyles() override {}
    void writeAppId() override {}
    void writeObjects() override {
        DRW_XRecord x; x.handle=handle; x.parentHandle=DRW::DwgNamedObjectsDictionaryHandle;
        x.m_values.emplace_back(1,UTF8STRING("LOCALLY_GENERATED"));
        written=registered && w_.writeXRecord(&x);
        x.handle=handle+1u;
        written=written && w_.writeXRecord(&x);
    }
    dwgRW& w_; bool registered=false, written=false;
};
class FailingCallback final : public dx_iface {
public:
    bool selected=false, failed=false;
    void addXRecord(const DRW_XRecord& value) override {
        if (value.handle==handle) selected=true;
        if (value.handle==handle+1u) {failed=true; throw std::runtime_error("callback failure");}
        dx_iface::addXRecord(value);
    }
};
class Reentry final : public dx_iface {
public:
    dx_iface* nested=nullptr; dx_data* data=nullptr; std::string path;
    bool attempted=false;
    void addXRecord(const DRW_XRecord& record) override {
        dx_iface::addXRecord(record);
        if (!attempted) {attempted=true; (void)nested->fileImport(path,data,false);}
    }
};
bool frameCrcControl(const std::vector<std::uint8_t>& body) {
    dwgBufferW out; out.putModularShort(static_cast<std::int32_t>(body.size()));
    out.putBytes(body.data(),body.size()); out.putRawShort16(out.crc16(0xC0C1u,0u,out.size()));
    auto bytes=out.data(); dwgBuffer input(bytes.data(),bytes.size(),nullptr); DwgObjectFrame frame;
    if (!frame.readAt(input,DRW::AC1018,0u) || frame.body()!=body) return false;
    bytes.back()^=1u;
    if (frame.readAt(input,DRW::AC1018,0u) || !frame.body().empty()) return false;
    bytes.back()^=1u;
    return frame.readAt(input,DRW::AC1018,0u) && frame.body()==body;
}
bool physicalCrcImportControl(const std::filesystem::path& path) {
    if (std::filesystem::exists(path)) return false;
    struct Cleanup { std::filesystem::path path; ~Cleanup() {std::error_code e; std::filesystem::remove(path,e);} } cleanup{path};
    dwgRW out(path.string().c_str()); Writer writer(out);
    if (!out.write(&writer,DRW::AC1015,true) || !writer.written) return false;
    dx_data clean; dx_iface reader; reader.enableVerifiedXRecordSource(handle);
    if (!reader.fileImport(path.string(),&clean,false)
        || !dx_iface::hasUnchangedVerifiedXRecordSource(clean,handle)) return false;
    const auto& pub=DxIfaceStandardDwgCaptureTestAccess::publication(clean,handle);
    if (pub.m_sourceOffsetSpace!=DRW_DwgFrameOffsetSpace::PhysicalFile) return false;
    std::ifstream in(path,std::ios::binary);
    std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(in)),{});
    if (!in || pub.m_sourceOffset>=bytes.size()) return false;
    dwgBuffer input(bytes.data(),bytes.size(),nullptr); DwgObjectFrame frame;
    if (!frame.readAt(input,DRW::AC1015,pub.m_sourceOffset)
        || frame.body()!=DxIfaceStandardDwgCaptureTestAccess::raw(clean,handle).m_rawBytes
        || input.getPosition()<2u || input.getPosition()>bytes.size()) return false;
    const auto crcOffset=static_cast<std::size_t>(input.getPosition()-2u);
    bytes[crcOffset]^=1u;
    {std::ofstream corrupt(path,std::ios::binary|std::ios::trunc); corrupt.write(reinterpret_cast<const char*>(bytes.data()),bytes.size()); if(!corrupt)return false;}
    dx_data bad; (void)reader.fileImport(path.string(),&bad,false);
    if (dx_iface::hasUnchangedVerifiedXRecordSource(bad,handle)) return false;
    bytes[crcOffset]^=1u;
    {std::ofstream restored(path,std::ios::binary|std::ios::trunc); restored.write(reinterpret_cast<const char*>(bytes.data()),bytes.size()); if(!restored)return false;}
    dx_data retry;
    return reader.fileImport(path.string(),&retry,false)
        && dx_iface::hasUnchangedVerifiedXRecordSource(retry,handle);
}
bool run(const std::filesystem::path& path) {
    const auto fail=[](const char* s) {std::cerr<<s<<'\n'; return false;};
    dwgRW out(path.string().c_str()); Writer writer(out);
    if (!out.write(&writer,DRW::AC1018,true) || !writer.written) return fail("generated DWG write failed");
    dx_data a,b; dx_iface first,second; first.enableVerifiedXRecordSource(handle); second.enableVerifiedXRecordSource(handle);
    if (!first.fileImport(path.string(),&a,false) || !dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("first selected source not sealed");
    if (!second.fileImport(path.string(),&b,false) || !dx_iface::hasUnchangedVerifiedXRecordSource(b,handle)) return fail("second selected source not sealed");
    if (!frameCrcControl(DxIfaceStandardDwgCaptureTestAccess::raw(a,handle).m_rawBytes)) return fail("CRC corruption or retry failed");
    DxIfaceStandardDwgCaptureTestAccess::transplant(b,a);
    if (dx_iface::hasUnchangedVerifiedXRecordSource(b,handle)) return fail("cross-import capability accepted");
    auto& raw=DxIfaceStandardDwgCaptureTestAccess::raw(a,handle); raw.m_rawBytes[0]^=1u;
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("changed body accepted");
    raw.m_rawBytes[0]^=1u;
    auto& pub=DxIfaceStandardDwgCaptureTestAccess::publication(a,handle); ++pub.m_sourceMapOrdinal;
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("changed ordinal accepted");
    --pub.m_sourceMapOrdinal;
    auto selected=std::find_if(a.xRecords.begin(),a.xRecords.end(),[](const DRW_XRecord& x){return x.handle==handle;});
    if (selected==a.xRecords.end()) return fail("missing generated XRECORD");
    auto saved=*selected; selected->m_cloning^=1;
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("changed model accepted");
    *selected=saved; selected->m_values.emplace_back(1,UTF8STRING("mutation"));
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("changed legacy values accepted");
    *selected=saved; selected->m_dataEntries.emplace_back(1,UTF8STRING("mutation"));
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("changed ordered values accepted");
    *selected=saved; selected->m_handleValues.emplace_back(360,0x123u);
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("changed handle values accepted");
    *selected=saved; a.xRecords.push_back(saved);
    if (dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("duplicate model accepted");
    a.xRecords.pop_back();
    if (!dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("corrected model retry failed");
    if (first.fileImport(path.string()+".missing",&a,false) || dx_iface::hasUnchangedVerifiedXRecordSource(a,handle)) return fail("failed reimport retained evidence");
    dx_data retry;
    if (!first.fileImport(path.string(),&retry,false) || !dx_iface::hasUnchangedVerifiedXRecordSource(retry,handle)) return fail("failed-import retry failed");
    dx_data plain; dx_iface plainReader;
    if (!plainReader.fileImport(path.string(),&plain,false) || plain.xRecords.size()!=retry.xRecords.size()
        || plain.dwgReadOutcome.observations.objectParseFailures!=retry.dwgReadOutcome.observations.objectParseFailures
        || dx_iface::hasUnchangedVerifiedXRecordSource(plain,handle)) return fail("opt-out import changed");
    dx_data wrong; dx_iface wrongReader; wrongReader.enableVerifiedXRecordSource(0xBEEFu);
    if (!wrongReader.fileImport(path.string(),&wrong,false) || dx_iface::hasUnchangedVerifiedXRecordSource(wrong,handle)) return fail("wrong selected handle accepted");
    dx_data failed; FailingCallback bad; bad.enableVerifiedXRecordSource(handle);
    (void)bad.fileImport(path.string(),&failed,false);
    if (!bad.selected || !bad.failed || failed.dwgReadOutcome.observations.objectParseFailures==0u
        || dx_iface::hasUnchangedVerifiedXRecordSource(failed,handle)) return fail("later callback failure sealed evidence");
    dx_data nestedData; Reentry reentry; reentry.data=&nestedData; reentry.path=path.string(); reentry.nested=&reentry; reentry.enableVerifiedXRecordSource(handle);
    (void)reentry.fileImport(path.string(),&nestedData,false);
    if (!reentry.attempted || dx_iface::hasUnchangedVerifiedXRecordSource(nestedData,handle)) return fail("same-iface reentry sealed evidence");
    dx_data crossData; Reentry outer; dx_iface inner; outer.data=&crossData; outer.path=path.string(); outer.nested=&inner;
    outer.enableVerifiedXRecordSource(handle); inner.enableVerifiedXRecordSource(handle);
    (void)outer.fileImport(path.string(),&crossData,false);
    if (!outer.attempted || dx_iface::hasUnchangedVerifiedXRecordSource(crossData,handle)) return fail("cross-iface reentry sealed evidence");
    if (!physicalCrcImportControl(path.string()+".ac1015.dwg")) return fail("AC1015 physical CRC refusal/retry failed");
    return true;
}
}
int main(int argc,char**argv) {
    if(argc!=2)return 2;
    const std::filesystem::path path(argv[1]);
    if (std::filesystem::exists(path)) {std::cerr<<"scratch path already exists\n";return 1;}
    const bool ok=run(path); std::error_code e; std::filesystem::remove(path,e);
    if(ok) std::cout<<"verified XRECORD source controls passed\n";
    return ok?0:1;
}
