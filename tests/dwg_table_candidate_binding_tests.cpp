#include "dx_iface.h"
#include "intern/dwgutil.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>

namespace fault {long remaining=-1;bool fired=false;bool (*predicate)() noexcept=nullptr;}
void* operator new(std::size_t n) {
    if(fault::predicate && fault::predicate()) {
        fault::predicate=nullptr;fault::fired=true;throw std::bad_alloc();
    }
    if (fault::remaining==0) {fault::remaining=-1;fault::fired=true;throw std::bad_alloc();}
    if (fault::remaining>0)--fault::remaining;
    if (void* p=std::malloc(n?n:1u))return p;
    throw std::bad_alloc();
}
void* operator new[](std::size_t n){return ::operator new(n);}
void operator delete(void* p) noexcept{std::free(p);}
void operator delete[](void* p) noexcept{std::free(p);}
void operator delete(void* p,std::size_t) noexcept{std::free(p);}
void operator delete[](void* p,std::size_t) noexcept{std::free(p);}

class DxIfaceStandardDwgCaptureTestAccess {
public:
    static DRW_UnsupportedObject& raw(dx_data& d,std::uint32_t h) {
        return d.m_opaqueDwgSourceFrames.at({d.importGeneration,h});
    }
    static const DRW_UnsupportedObject& standardRaw(const dx_data& d,std::uint32_t h) {
        return d.m_standardDwgObjectFrames.at({d.importGeneration,h}).raw;
    }
    static void deliver(dx_iface& i,const DwgVerifiedTableCandidateTicket& ticket) noexcept {
        i.dx_iface::verifiedTableCandidate(ticket);
    }
    static void transplant(dx_data& dst,const dx_data& src) {
        dst.m_verifiedTableCandidateSource=src.m_verifiedTableCandidateSource;
    }
    static bool sealStage(dx_iface& a,int stage) noexcept {
        const auto* d=a.m_verifiedXRecordReadData;
        if(!d || !d->dwgReadOutcome.operationSucceeded || d->m_verifiedXRecordSource
            || d->m_verifiedTableCandidateSource)return false;
        if(stage==1 && (a.m_verifiedXRecordPending.size()!=a.m_verifiedXRecordSelected.size()
                        || a.m_verifiedTableCandidatePending.size()!=(a.m_verifiedTableGeometrySelected?3u:2u)))return false;
        if(stage==2 && (!a.m_verifiedXRecordPending.empty() || a.m_verifiedTableCandidatePending.size()!=(a.m_verifiedTableGeometrySelected?3u:2u)))return false;
        if(stage==3 && (!a.m_verifiedXRecordPending.empty() || !a.m_verifiedTableCandidatePending.empty()))return false;
        for(const auto& row:d->m_standardDwgObjectFrames)
            if(row.first.first==d->importGeneration && !row.second.payloadMatched)return false;
        return true;
    }
};
namespace {
constexpr std::uint32_t contentHandle=0xb9u,rootHandle=0xbcu,dataHandle=0xbdu;
const std::vector<std::uint32_t> xrecords{0xbcu,0xbeu,0xbfu,0xc0u};
using Content=DwgTableContentAc1018Candidate::Result;
using Data=DwgDataTableAc1018Candidate::Result;
using Geometry=DwgTableGeometryAc1018Candidate::Result;
constexpr std::uint32_t geometryHandle=0xbau;

DRW_UnsupportedObject raw(const char* path,bool content) {
    std::ifstream f(path,std::ios::binary);
    DRW_UnsupportedObject r;r.m_rawBytes.assign(std::istreambuf_iterator<char>(f),{});
    r.m_version=DRW::AC1018;r.m_isCustomClass=r.m_hasClassDefinition=true;
    r.m_objectType=content?514:517;r.m_handle=content?contentHandle:dataHandle;
    r.m_recordName=content?"TABLECONTENT":"DATATABLE";
    r.m_className=content?"AcDbTableContent":"AcDbDataTable";
    r.m_classAppName="LOCALLY_AUTHORED";r.m_classProxyFlag=0x401u;r.m_classEntityFlagRaw=0x1f3u;
    r.m_objectSize=static_cast<std::uint32_t>(r.m_rawBytes.size());
    return r;
}
class Writer final : public dx_iface {
public:
    Writer(dwgRW& output,DRW_UnsupportedObject a,DRW_UnsupportedObject b,std::size_t big=0u,DRW_UnsupportedObject g={})
        :w(output),content(std::move(a)),table(std::move(b)),geometry(std::move(g)),bigSize(big){}
    void writeHeader(DRW_Header& h) override {h.vars.clear();}
    void writeDwgClasses() override {registered=w.registerRawDwgObjectClass(&content)&&w.registerRawDwgObjectClass(&table)
        && (!geometry.m_handle || w.registerRawDwgObjectClass(&geometry));}
    void collectDwgAppIds() override {}
    void writeBlocks() override {} void writeBlockRecords() override {} void writeEntities() override {}
    void writeLTypes() override {} void writeLayers() override {} void writeTextstyles() override {}
    void writeVports() override {} void writeViews() override {} void writeUCSs() override {}
    void writeDimstyles() override {} void writeAppId() override {}
    void writeObjects() override {
        wrote=registered;
        for (std::size_t i=0;i<xrecords.size();++i) {
            const auto h=xrecords[i];
            DRW_XRecord x;x.handle=h;x.parentHandle=h==rootHandle?0xbbu:dataHandle;
            if(bigSize && i<2u)x.m_values.emplace_back(310,std::vector<std::uint8_t>(bigSize,0xa5u));
            else x.m_values.emplace_back(1,UTF8STRING("LOCALLY_AUTHORED"));
            wrote=w.writeXRecord(&x)&&wrote;
        }
        wrote=w.writeRawDwgObject(&content)&&w.writeRawDwgObject(&table)&&wrote;
        if(geometry.m_handle)wrote=w.writeRawDwgObject(&geometry)&&wrote;
    }
    bool finalizeDwgWrite() override{return wrote;}
    bool finalizeDwgWriteStructure() override{return true;}
    dwgRW& w;DRW_UnsupportedObject content,table,geometry;std::size_t bigSize;bool registered=false,wrote=false;
};
bool write(const std::filesystem::path& path,DRW_UnsupportedObject a,DRW_UnsupportedObject b,std::size_t big=0u,const DRW_UnsupportedObject* geometry=nullptr) {
    dwgRW output(path.string().c_str());Writer writer(output,std::move(a),std::move(b),big,geometry?*geometry:DRW_UnsupportedObject{});
    return output.write(&writer,DRW::AC1018,true)&&writer.wrote;
}
bool selected(dx_iface& a){return a.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle);}
bool live(const dx_data& d) {
    Content c;Data t;
    return dx_iface::readVerifiedTableCandidates(d,xrecords,contentHandle,dataHandle,c,t)
        && c.handle==contentHandle && t.handle==dataHandle && c.parentHandle==rootHandle
        && t.parentHandle==rootHandle && t.ownedChildren==std::vector<std::uint32_t>({0xbeu,0xbfu,0xc0u});
}
class FaultAdapter final : public dx_iface {
public:
    enum class Injection {None,Candidate,Ticket};
    Injection injection{Injection::None};mutable bool armed=false;bool ticket=false;mutable bool wantedTarget=false;unsigned targetTickets=0;std::uint32_t target=dataHandle;
    bool wantsVerifiedTableCandidate(std::uint32_t h) const noexcept override {
        if(h!=contentHandle && h!=dataHandle && h!=geometryHandle)return false;
        if(h==target)wantedTarget=true;
        if(injection==Injection::Candidate && h==target && !armed){armed=true;fault::remaining=0;}
        return true;
    }
    void verifiedTableCandidate(const DwgVerifiedTableCandidateTicket& value) noexcept override {
        if(value.handle()==target)++targetTickets;
        if(injection==Injection::Ticket && value.handle()==target){ticket=true;fault::remaining=0;}
        DxIfaceStandardDwgCaptureTestAccess::deliver(*this,value);fault::remaining=-1;
    }
};
class Reentry final : public dx_iface {
public:
    dx_data* data=nullptr;dx_iface* nested=nullptr;std::string path;bool attempted=false;
    void addUnsupportedObject(const DRW_UnsupportedObject& value) override {
        dx_iface::addUnsupportedObject(value);
        if(value.m_handle==contentHandle && !attempted){attempted=true;(void)(nested?nested:this)->fileImport(path,data,false);}
    }
};
dx_iface* sealingAdapter=nullptr;int sealingStage=0;
bool sealFaultReady() noexcept {
    return sealingAdapter && DxIfaceStandardDwgCaptureTestAccess::sealStage(*sealingAdapter,sealingStage);
}
class Spoofing final : public dx_iface {
public:
    int mode{0};bool seen=false;std::uint32_t target=contentHandle;
    void addUnsupportedObject(const DRW_UnsupportedObject& value) override {
        if(value.m_handle==target && mode) {
            seen=true;auto altered=value;
            if(mode==1)altered.m_className="Other";
            else if(mode==2)altered.m_recordName="OTHER";
            else ++altered.m_objectType;
            dx_iface::addUnsupportedObject(altered);return;
        }
        dx_iface::addUnsupportedObject(value);
    }
};
class Throwing final : public dx_iface {
public:
    bool fail=true,seen=false;std::uint32_t target=dataHandle;
    void addUnsupportedObject(const DRW_UnsupportedObject& value) override {
        if(value.m_handle==target && fail){seen=true;throw std::runtime_error("callback failure");}
        dx_iface::addUnsupportedObject(value);
    }
};
std::uint32_t get32(const std::uint8_t* p) {
    return static_cast<std::uint32_t>(p[0]) | static_cast<std::uint32_t>(p[1])<<8u
        | static_cast<std::uint32_t>(p[2])<<16u | static_cast<std::uint32_t>(p[3])<<24u;
}
void put32(std::uint8_t* p,std::uint32_t n) {
    for(unsigned i=0;i<4;++i)p[i]=static_cast<std::uint8_t>(n>>(i*8u));
}
bool corruptSelectedCrc(const std::filesystem::path& path,const DRW_UnsupportedObject& source,
                        std::vector<std::uint8_t>& original) {
    std::ifstream f(path,std::ios::binary);original.assign(std::istreambuf_iterator<char>(f),{});
    if(!f || source.m_rawBytes.empty())return false;
    const auto hit=std::search(original.cbegin(),original.cend(),source.m_rawBytes.cbegin(),source.m_rawBytes.cend());
    if(hit==original.cend() || std::search(std::next(hit),original.cend(),source.m_rawBytes.cbegin(),source.m_rawBytes.cend())!=original.cend())return false;
    const auto at=static_cast<std::size_t>(hit-original.cbegin());
    dwgBuffer in(original.data(),original.size(),nullptr);DwgObjectFrame frame;bool exactFrame=false;std::size_t frameOffset=0u;
    for(std::size_t prefix=2u;prefix<=4u;prefix+=2u) {
        if(at>=prefix && frame.readAt(in,DRW::AC1018,at-prefix) && frame.body()==source.m_rawBytes
            && in.getPosition()==at+source.m_rawBytes.size()+2u) {exactFrame=true;frameOffset=at-prefix;break;}
    }
    if(!exactFrame)return false;
    std::size_t page=0u;std::vector<std::uint8_t> header(32u);
    for(std::size_t p=0x100u;p+32u<=at;p+=0x20u) {
        std::copy_n(original.data()+p,32u,header.data());dwgCompressor::decrypt18Hdr(header.data(),32u,p);
        const auto size=get32(header.data()+8u);
        if(get32(header.data())!=0x4163043bu || size!=get32(header.data()+12u)
            || size>original.size()-(p+32u) || at<p+32u
            || at+source.m_rawBytes.size()+2u>p+32u+size)continue;
        const auto dataCrc=dwgUtil::checksum18(0u,original.data()+p+32u,size);
        const auto expectedHeader=get32(header.data()+24u);put32(header.data()+24u,0u);
        if(dataCrc!=get32(header.data()+28u) || dwgUtil::checksum18(dataCrc,header.data(),32u)!=expectedHeader)continue;
        if(page!=0u)return false;page=p;
    }
    if(page==0u)return false;
    auto bytes=original;
    const auto crcAt=at+source.m_rawBytes.size();bytes[crcAt]^=1u;
    std::copy_n(bytes.data()+page,32u,header.data());dwgCompressor::decrypt18Hdr(header.data(),32u,page);
    const auto size=get32(header.data()+8u);const auto dataCrc=dwgUtil::checksum18(0u,bytes.data()+page+32u,size);
    put32(header.data()+28u,dataCrc);put32(header.data()+24u,0u);
    put32(header.data()+24u,dwgUtil::checksum18(dataCrc,header.data(),32u));
    dwgCompressor::decrypt18Hdr(header.data(),32u,page);
    std::copy(header.begin(),header.end(),bytes.begin()+static_cast<std::ptrdiff_t>(page));
    dwgBuffer damaged(bytes.data(),bytes.size(),nullptr);
    if(frame.readAt(damaged,DRW::AC1018,frameOffset))return false;
    std::ofstream out(path,std::ios::binary|std::ios::trunc);
    out.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());return static_cast<bool>(out);
}
bool run(const char* contentPath,const char* dataPath,const char* wrongPath,
         const char* largeContentPath,const std::filesystem::path& path) {
    auto content=raw(contentPath,true),table=raw(dataPath,false);
    if(content.m_rawBytes.empty()||table.m_rawBytes.empty()||!write(path,content,table))return false;
    dx_iface reader;dx_data baseline;if(!selected(reader))return false;
    auto invalid=xrecords;invalid[0]=DRW::NoHandle;
    if(reader.enableVerifiedTableCandidates(invalid,contentHandle,dataHandle)
        || reader.enableVerifiedTableCandidates(xrecords,DRW::NoHandle,dataHandle)
        || reader.enableVerifiedTableCandidates(xrecords,contentHandle,DRW::NoHandle)
        || reader.enableVerifiedTableCandidates(xrecords,contentHandle,contentHandle)
        || reader.enableVerifiedTableCandidates(xrecords,rootHandle,dataHandle))return false;
    invalid=xrecords;invalid.push_back(rootHandle);
    if(reader.enableVerifiedTableCandidates(invalid,contentHandle,dataHandle))return false;
    if(!reader.fileImport(path.string(),&baseline,false)||!live(baseline))return false;
    auto& snapshot=DxIfaceStandardDwgCaptureTestAccess::raw(baseline,dataHandle);
    snapshot.m_rawBytes[0]^=1u;
    Content c;Data t;c.handle=1u;t.handle=1u;
    if(dx_iface::hasUnchangedVerifiedXRecordSource(baseline,rootHandle)
        || dx_iface::readVerifiedTableCandidates(baseline,xrecords,contentHandle,dataHandle,c,t)
        || c.handle || t.handle)return false;
    snapshot.m_rawBytes[0]^=1u;if(!live(baseline))return false;
    const auto child=std::find_if(baseline.xRecords.begin(),baseline.xRecords.end(),[](const DRW_XRecord& x){return x.handle==0xbeu;});
    if(child==baseline.xRecords.end())return false;child->m_cloning^=1;
    if(live(baseline)||dx_iface::hasUnchangedVerifiedXRecordSource(baseline,rootHandle))return false;
    child->m_cloning^=1;if(!live(baseline))return false;
    auto wrong=xrecords;wrong[1]=0xc1u;
    if(dx_iface::readVerifiedTableCandidates(baseline,wrong,contentHandle,dataHandle,c,t)
        || dx_iface::readVerifiedTableCandidates(baseline,xrecords,dataHandle,contentHandle,c,t))return false;
    dx_data second;if(!reader.fileImport(path.string(),&second,false)||!live(second))return false;
    DxIfaceStandardDwgCaptureTestAccess::transplant(second,baseline);
    if(live(second)||dx_iface::hasUnchangedVerifiedXRecordSource(second,rootHandle))return false;
    if(!reader.enableVerifiedTableCandidates(xrecords,contentHandle,0xcau))return false;
    dx_data absent;(void)reader.fileImport(path.string(),&absent,false);
    if(live(absent)||dx_iface::hasUnchangedVerifiedXRecordSource(absent,rootHandle))return false;
    if(!selected(reader))return false;dx_data retry;
    if(!reader.fileImport(path.string(),&retry,false)||!live(retry))return false;
    for(int stage=1;stage<=3;++stage) {
        dx_iface a;if(!selected(a))return false;dx_data failed;
        sealingAdapter=&a;sealingStage=stage;fault::predicate=sealFaultReady;fault::fired=false;
        const bool ok=a.fileImport(path.string(),&failed,false);
        fault::predicate=nullptr;sealingAdapter=nullptr;
        if(!ok||!fault::fired||live(failed)||dx_iface::hasUnchangedVerifiedXRecordSource(failed,rootHandle))return false;
        dx_data again;if(!a.fileImport(path.string(),&again,false)||!live(again))return false;
    }
    for(const auto injection:{FaultAdapter::Injection::Candidate,FaultAdapter::Injection::Ticket}) {
        FaultAdapter a;if(!selected(a))return false;a.injection=injection;fault::fired=false;dx_data failed;
        const bool ok=a.fileImport(path.string(),&failed,false);fault::remaining=-1;
        if(!ok||!fault::fired||live(failed)||dx_iface::hasUnchangedVerifiedXRecordSource(failed,rootHandle)
            || (injection==FaultAdapter::Injection::Ticket&&!a.ticket))return false;
        a.injection=FaultAdapter::Injection::None;dx_data again;
        if(!a.fileImport(path.string(),&again,false)||!live(again))return false;
    }
    Reentry nested;dx_data nestedData;nested.data=&nestedData;nested.path=path.string();
    if(!selected(nested))return false;(void)nested.fileImport(path.string(),&nestedData,false);
    if(!nested.attempted||live(nestedData)||dx_iface::hasUnchangedVerifiedXRecordSource(nestedData,rootHandle))return false;
    dx_data nestedRetry;if(!nested.fileImport(path.string(),&nestedRetry,false)||!live(nestedRetry))return false;
    Reentry outer;dx_iface inner;dx_data crossData;outer.data=&crossData;outer.nested=&inner;outer.path=path.string();
    if(!selected(outer)||!selected(inner))return false;(void)outer.fileImport(path.string(),&crossData,false);
    if(!outer.attempted||live(crossData)||dx_iface::hasUnchangedVerifiedXRecordSource(crossData,rootHandle))return false;
    dx_data crossRetry;if(!outer.fileImport(path.string(),&crossRetry,false)||!live(crossRetry))return false;
    Throwing throwing;if(!selected(throwing))return false;dx_data failed;
    (void)throwing.fileImport(path.string(),&failed,false);
    if(!throwing.seen||live(failed)||dx_iface::hasUnchangedVerifiedXRecordSource(failed,rootHandle))return false;
    throwing.fail=false;dx_data callbackRetry;
    if(!throwing.fileImport(path.string(),&callbackRetry,false)||!live(callbackRetry))return false;
    for(int mode=1;mode<=3;++mode) {
        Spoofing spoof;if(!selected(spoof))return false;spoof.mode=mode;dx_data altered;
        (void)spoof.fileImport(path.string(),&altered,false);
        if(!spoof.seen||live(altered)||dx_iface::hasUnchangedVerifiedXRecordSource(altered,rootHandle))return false;
        spoof.mode=0;dx_data fresh;if(!spoof.fileImport(path.string(),&fresh,false)||!live(fresh))return false;
    }
    const auto badRole=raw(wrongPath,false);const auto bad=path.string()+".bad.dwg";
    struct Cleanup {std::filesystem::path p;~Cleanup(){std::error_code e;std::filesystem::remove(p,e);}} cleanup{bad};
    if(!write(bad,content,badRole))return false;
    dx_data badData;(void)reader.fileImport(bad,&badData,false);
    if(live(badData)||dx_iface::hasUnchangedVerifiedXRecordSource(badData,rootHandle))return false;
    dx_data finalRetry;if(!reader.fileImport(path.string(),&finalRetry,false)||!live(finalRetry))return false;
    const auto output=path.string()+".dxf";
    Cleanup outputCleanup{output};
    {std::ofstream sentinel(output,std::ios::binary|std::ios::trunc);sentinel<<"SOURCE_SENTINEL";if(!sentinel)return false;}
    if(reader.fileExport(output,DRW::AC1018,false,&finalRetry,false,true,
            dx_dxfSourceTextPolicy::Compatible,DRW::DxfTextWritePolicy::Compatible,
            dx_dxfProxyGraphicsPolicy::Compatible,dx_dwgReadCompletenessPolicy::RequireComplete))return false;
    {std::ifstream in(output,std::ios::binary);const std::string value((std::istreambuf_iterator<char>(in)),{});
     if(value!="SOURCE_SENTINEL")return false;}
    std::vector<std::uint8_t> original;
    if(!corruptSelectedCrc(path,DxIfaceStandardDwgCaptureTestAccess::raw(finalRetry,contentHandle),original))return false;
    dx_data crcBad;(void)reader.fileImport(path.string(),&crcBad,false);
    if(live(crcBad)||dx_iface::hasUnchangedVerifiedXRecordSource(crcBad,rootHandle))return false;
    bool targetFrameFailure=false;
    const auto& integrity=crcBad.dwgReadOutcome.observations.integrity;
    if(integrity.extractionOmitted!=0u || integrity.reportedUpstreamDropped!=0u
        || integrity.aggregateSaturated)return false;
    for(std::size_t i=0;i<integrity.retained;++i) {
        const auto& observation=integrity.rows[i];
        if(observation.kind==DwgIntegrityCheckKind::DataPageChecksum
            || observation.kind==DwgIntegrityCheckKind::DataPageCrc)return false;
        if(observation.kind==DwgIntegrityCheckKind::ObjectFrameBounds
            && observation.hasLogicalHandle && observation.logicalHandle==contentHandle)
            targetFrameFailure=true;
    }
    if(!targetFrameFailure)return false;
    {std::ofstream f(path,std::ios::binary|std::ios::trunc);f.write(reinterpret_cast<const char*>(original.data()),original.size());if(!f)return false;}
    dx_data crcRetry;if(!reader.fileImport(path.string(),&crcRetry,false)||!live(crcRetry))return false;
    auto larger=raw(largeContentPath,true);const auto largePath=path.string()+".large.dwg";Cleanup largeCleanup{largePath};
    Content largeCandidate;if(!DwgTableContentAc1018Candidate::inspect(larger,largeCandidate))return false;
    if(!write(largePath,larger,table,1035000u))return false;
    dx_iface xonly;if(!xonly.enableVerifiedXRecordSources(xrecords))return false;dx_data quotaSource;
    if(!xonly.fileImport(largePath,&quotaSource,false)||!dx_iface::hasUnchangedVerifiedXRecordSources(quotaSource,xrecords))return false;
    std::size_t total=0;for(const auto h:xrecords)total+=DxIfaceStandardDwgCaptureTestAccess::standardRaw(quotaSource,h).m_rawBytes.size();
    if(total>=2u*1024u*1024u || total+larger.m_rawBytes.size()+table.m_rawBytes.size()<=2u*1024u*1024u)return false;
    dx_iface quota;if(!selected(quota))return false;dx_data refusedQuota;
    if(!quota.fileImport(largePath,&refusedQuota,false)||live(refusedQuota)
        || dx_iface::hasUnchangedVerifiedXRecordSource(refusedQuota,rootHandle))return false;
    dx_data quotaRetry;if(!quota.fileImport(path.string(),&quotaRetry,false)||!live(quotaRetry))return false;
    std::cout<<"combined source-binding, mutation, failure and retry controls passed\n";
    return true;
}
DRW_UnsupportedObject rawGeometry(const char* path) {
    auto value=raw(path,true);value.m_objectType=515;value.m_handle=geometryHandle;
    value.m_recordName="TABLEGEOMETRY";value.m_className="AcDbTableGeometry";return value;
}
bool selectedGeometry(dx_iface& a) {
    return a.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle,geometryHandle);
}
bool liveGeometry(const dx_data& d) {
    Content c;Data t;Geometry g;
    return dx_iface::readVerifiedTableCandidates(d,xrecords,contentHandle,dataHandle,geometryHandle,c,t,g)
        && c.handle==contentHandle && t.handle==dataHandle && g.handle==geometryHandle
        && c.parentHandle==rootHandle && t.parentHandle==rootHandle && g.parentHandle==rootHandle;
}
bool runGeometry(const char* contentPath,const char* dataPath,const char* geometryPath,
                 const char* badGeometryPath,const std::filesystem::path& path) {
#define CHECK_G(expr) do { if(!(expr)){std::cerr<<"geometry binding failed line "<<__LINE__<<'\n';return false;} } while(false)
    auto content=raw(contentPath,true),table=raw(dataPath,false),geometry=rawGeometry(geometryPath);
    CHECK_G(!geometry.m_rawBytes.empty());CHECK_G(write(path,content,table,0,&geometry));
    dx_iface reader;CHECK_G(selectedGeometry(reader));
    CHECK_G(!reader.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle,DRW::NoHandle));
    CHECK_G(!reader.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle,contentHandle));
    CHECK_G(!reader.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle,dataHandle));
    CHECK_G(!reader.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle,rootHandle));
    dx_data baseline;CHECK_G(reader.fileImport(path.string(),&baseline,false));CHECK_G(liveGeometry(baseline));
    CHECK_G(dx_iface::hasUnchangedVerifiedXRecordSources(baseline,xrecords));
    Content c;Data t;Geometry g;c.handle=t.handle=g.handle=1u;
    CHECK_G(!dx_iface::readVerifiedTableCandidates(baseline,xrecords,contentHandle,dataHandle,c,t));
    CHECK_G(!dx_iface::readVerifiedTableCandidates(baseline,xrecords,contentHandle,dataHandle,0xcau,c,t,g));
    CHECK_G(!c.handle && !t.handle && !g.handle);
    auto& snapshot=DxIfaceStandardDwgCaptureTestAccess::raw(baseline,geometryHandle);
    snapshot.m_rawBytes[0]^=1u;
    CHECK_G(!liveGeometry(baseline));CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(baseline,rootHandle));
    snapshot.m_rawBytes[0]^=1u;CHECK_G(liveGeometry(baseline));
    dx_data second;CHECK_G(reader.fileImport(path.string(),&second,false));CHECK_G(liveGeometry(second));
    DxIfaceStandardDwgCaptureTestAccess::transplant(second,baseline);
    CHECK_G(!liveGeometry(second));CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(second,rootHandle));
    CHECK_G(reader.enableVerifiedTableCandidates(xrecords,contentHandle,dataHandle,0xcau));
    dx_data missing;(void)reader.fileImport(path.string(),&missing,false);
    CHECK_G(!liveGeometry(missing));CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(missing,rootHandle));
    CHECK_G(selectedGeometry(reader));dx_data retry;CHECK_G(reader.fileImport(path.string(),&retry,false));CHECK_G(liveGeometry(retry));
    for(int stage=1;stage<=3;++stage) {
        dx_iface a;CHECK_G(selectedGeometry(a));dx_data failed;sealingAdapter=&a;sealingStage=stage;
        fault::predicate=sealFaultReady;fault::fired=false;const bool ok=a.fileImport(path.string(),&failed,false);
        fault::predicate=nullptr;sealingAdapter=nullptr;
        CHECK_G(ok && fault::fired);CHECK_G(!liveGeometry(failed));
        CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(failed,rootHandle));
        dx_data again;CHECK_G(a.fileImport(path.string(),&again,false));CHECK_G(liveGeometry(again));
    }
    for(const auto injection:{FaultAdapter::Injection::Candidate,FaultAdapter::Injection::Ticket}) {
        FaultAdapter a;CHECK_G(selectedGeometry(a));a.target=geometryHandle;a.injection=injection;fault::fired=false;dx_data failed;
        const bool ok=a.fileImport(path.string(),&failed,false);fault::remaining=-1;
        CHECK_G(ok && fault::fired);CHECK_G(!liveGeometry(failed));
        CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(failed,rootHandle));
        CHECK_G(injection!=FaultAdapter::Injection::Ticket || a.ticket);
        a.injection=FaultAdapter::Injection::None;dx_data again;
        CHECK_G(a.fileImport(path.string(),&again,false));CHECK_G(liveGeometry(again));
    }
    Reentry same;dx_data sameData;same.data=&sameData;same.path=path.string();CHECK_G(selectedGeometry(same));
    (void)same.fileImport(path.string(),&sameData,false);CHECK_G(same.attempted && !liveGeometry(sameData));
    CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(sameData,rootHandle));
    dx_data sameRetry;CHECK_G(same.fileImport(path.string(),&sameRetry,false));CHECK_G(liveGeometry(sameRetry));
    Reentry outer;dx_iface inner;dx_data cross;outer.data=&cross;outer.nested=&inner;outer.path=path.string();
    CHECK_G(selectedGeometry(outer) && selectedGeometry(inner));(void)outer.fileImport(path.string(),&cross,false);
    CHECK_G(outer.attempted && !liveGeometry(cross));CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(cross,rootHandle));
    dx_data crossRetry;CHECK_G(outer.fileImport(path.string(),&crossRetry,false));CHECK_G(liveGeometry(crossRetry));
    Throwing throwing;throwing.target=geometryHandle;CHECK_G(selectedGeometry(throwing));dx_data thrown;
    (void)throwing.fileImport(path.string(),&thrown,false);CHECK_G(throwing.seen && !liveGeometry(thrown));
    CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(thrown,rootHandle));
    throwing.fail=false;dx_data throwRetry;CHECK_G(throwing.fileImport(path.string(),&throwRetry,false));CHECK_G(liveGeometry(throwRetry));
    for(int mode=1;mode<=3;++mode) {
        Spoofing spoof;spoof.target=geometryHandle;spoof.mode=mode;CHECK_G(selectedGeometry(spoof));dx_data altered;
        (void)spoof.fileImport(path.string(),&altered,false);CHECK_G(spoof.seen && !liveGeometry(altered));
        CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(altered,rootHandle));
        spoof.mode=0;dx_data fresh;CHECK_G(spoof.fileImport(path.string(),&fresh,false));CHECK_G(liveGeometry(fresh));
    }
    const auto bad=path.string()+".geometry-bad.dwg";
    struct Cleanup {std::filesystem::path p;~Cleanup(){std::error_code e;std::filesystem::remove(p,e);}} cleanup{bad};
    auto badGeometry=rawGeometry(badGeometryPath);CHECK_G(write(bad,content,table,0,&badGeometry));dx_data badData;
    (void)reader.fileImport(bad,&badData,false);CHECK_G(!liveGeometry(badData));
    CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(badData,rootHandle));
    dx_data good;CHECK_G(reader.fileImport(path.string(),&good,false));CHECK_G(liveGeometry(good));
    const auto output=path.string()+".geometry.dxf";Cleanup outputCleanup{output};
    {std::ofstream f(output,std::ios::binary|std::ios::trunc);f<<"SOURCE_SENTINEL";CHECK_G(static_cast<bool>(f));}
    CHECK_G(!reader.fileExport(output,DRW::AC1018,false,&good,false,true,dx_dxfSourceTextPolicy::Compatible,
        DRW::DxfTextWritePolicy::Compatible,dx_dxfProxyGraphicsPolicy::Compatible,dx_dwgReadCompletenessPolicy::RequireComplete));
    {std::ifstream f(output,std::ios::binary);const std::string value((std::istreambuf_iterator<char>(f)),{});CHECK_G(value=="SOURCE_SENTINEL");}
    std::vector<std::uint8_t> original;CHECK_G(corruptSelectedCrc(path,DxIfaceStandardDwgCaptureTestAccess::raw(good,geometryHandle),original));
    dx_data crcBad;(void)reader.fileImport(path.string(),&crcBad,false);CHECK_G(!liveGeometry(crcBad));
    CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(crcBad,rootHandle));
    const auto& integrity=crcBad.dwgReadOutcome.observations.integrity;
    CHECK_G(!integrity.extractionOmitted && !integrity.reportedUpstreamDropped && !integrity.aggregateSaturated);
    bool targetFailure=false;
    for(std::size_t i=0;i<integrity.retained;++i) {
        const auto& row=integrity.rows[i];CHECK_G(row.kind!=DwgIntegrityCheckKind::DataPageChecksum && row.kind!=DwgIntegrityCheckKind::DataPageCrc);
        if(row.kind==DwgIntegrityCheckKind::ObjectFrameBounds && row.hasLogicalHandle && row.logicalHandle==geometryHandle)targetFailure=true;
    }
    CHECK_G(targetFailure);
    {std::ofstream f(path,std::ios::binary|std::ios::trunc);f.write(reinterpret_cast<const char*>(original.data()),original.size());CHECK_G(static_cast<bool>(f));}
    dx_data crcRetry;CHECK_G(reader.fileImport(path.string(),&crcRetry,false));CHECK_G(liveGeometry(crcRetry));
    auto oversized=geometry;oversized.m_rawBytes.resize(4097u,0u);oversized.m_objectSize=4097u;
    CHECK_G(write(bad,content,table,0,&oversized));dx_data quota;FaultAdapter quotaReader;
    quotaReader.target=geometryHandle;CHECK_G(selectedGeometry(quotaReader));
    CHECK_G(quotaReader.fileImport(bad,&quota,false));CHECK_G(quotaReader.wantedTarget && quotaReader.targetTickets==0u);CHECK_G(!liveGeometry(quota));
    CHECK_G(!dx_iface::hasUnchangedVerifiedXRecordSource(quota,rootHandle));
    dx_data quotaRetry;CHECK_G(quotaReader.fileImport(path.string(),&quotaRetry,false));
    CHECK_G(quotaReader.targetTickets==1u && liveGeometry(quotaRetry));
    std::cout<<"geometry bundle failure and retry controls passed\n";
#undef CHECK_G
    return true;
}

}
int main(int argc,char**argv){if(argc!=8)return 2;std::filesystem::path p(argv[5]);if(std::filesystem::exists(p))return 2;const bool ok=run(argv[1],argv[2],argv[3],argv[4],p) && runGeometry(argv[1],argv[2],argv[6],argv[7],p);std::error_code e;std::filesystem::remove(p,e);return ok?0:1;}
