#include "intern/dwg_table_roundtrip_xrecord_ac1018_candidate.h"
#include "intern/dwgbufferw.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
namespace fault { long remaining=-1; std::size_t count=0; bool active=false; }
void* operator new(std::size_t n) {
    if (fault::active) ++fault::count;
    if (fault::remaining==0) {fault::remaining=-1;throw std::bad_alloc();}
    if (fault::remaining>0) --fault::remaining;
    if (void* p=std::malloc(n?n:1u)) return p;
    throw std::bad_alloc();
}
void* operator new[](std::size_t n) {return ::operator new(n);}
void operator delete(void* p) noexcept {std::free(p);}
void operator delete[](void* p) noexcept {std::free(p);}
void operator delete(void* p,std::size_t) noexcept {std::free(p);}
void operator delete[](void* p,std::size_t) noexcept {std::free(p);}
namespace {
using DwgTableRoundtripXRecordAc1018Candidate::Result;
struct Options {
    std::uint8_t selfRole{0u},ownerRole{4u},reactorRole{4u},codepage{45u};
    std::uint64_t content{0xb9u},geometry{0xbau},table{0xbdu};
    std::uint32_t owner{0xbbu},reactor{0xbbu};
    bool badMarker{false},badCode{false},reservedLength{false},badPoint{false};
    bool eed{false},extraReactor{false},extension{false};
    int dataEndAdjustment{0};
};
struct Fixture {DRW_UnsupportedObject raw; DRW_XRecord typed;};
void handle(dwgBufferW& b,std::uint8_t code,std::uint32_t value) {
    dwgHandle h;h.code=code;h.ref=value;b.putHandle(h);
}
void marker(dwgBufferW& b,const char* text,std::uint8_t codepage) {
    const std::string s(text);
    b.putRawShort16(102u);b.putRawShort16(static_cast<std::uint16_t>(s.size()));b.putRawChar8(codepage);
    for (unsigned char c:s) b.putRawChar8(c);
}
void ref(dwgBufferW& b,std::uint16_t code,std::uint64_t value) {b.putRawShort16(code);b.putRawLong64(value);}
void intpair(dwgBufferW& b,std::uint16_t code,std::int32_t value) {
    b.putRawShort16(code);
    if(code==70u)b.putRawShort16(static_cast<std::uint16_t>(value));
    else b.putRawLong32(static_cast<std::uint32_t>(value));
}
Fixture build(const Options& o={}) {
    Fixture f;dwgBufferW b;b.putObjType(DRW::AC1018,79u);const auto end=b.bitCount();b.putRawLong32(0u);
    handle(b,o.selfRole,0xbcu);b.putBitShort(o.eed?1u:0u);b.putBitLong(o.extraReactor?2:1);b.putBit(o.extension?0u:1u);
    if(o.reservedLength)b.put2Bits(3u);else b.putBitLong(197);
    marker(b,o.badMarker?"XCAD_ROUNDTRIP_2008_TABLE_ENTITY":"ACAD_ROUNDTRIP_2008_TABLE_ENTITY",o.codepage);
    ref(b,o.badCode?361u:360u,o.content);intpair(b,70u,2);intpair(b,90u,1);
    b.putRawShort16(10u);b.putRawDouble(o.badPoint?-0.0:0.0);b.putRawDouble(0.0);b.putRawDouble(0.0);
    intpair(b,90u,0);intpair(b,90u,2);ref(b,361u,o.geometry);
    marker(b,"ACAD_ROUNDTRIP_PRE2007_TABLE",o.codepage);intpair(b,90u,3);intpair(b,91u,3);
    marker(b,"ACAD_ROUNDTRIP_PRE2007_TABLECELL",o.codepage);ref(b,360u,o.table);b.putBitShort(1u);
    b.patchRawLong32AtBit(end,static_cast<std::uint32_t>(static_cast<std::int64_t>(b.bitCount())+o.dataEndAdjustment));
    handle(b,o.ownerRole,o.owner);handle(b,o.reactorRole,o.reactor);b.alignToByte();
    f.raw.m_version=DRW::AC1018;f.raw.m_objectType=79;f.raw.m_handle=0xbcu;
    f.raw.m_rawBytes=b.data();f.raw.m_objectSize=static_cast<std::uint32_t>(f.raw.m_rawBytes.size());
    f.typed.handle=0xbcu;f.typed.parentHandle=o.owner;f.typed.reactorHandles={o.reactor};
    f.typed.setDwgCommonObjectState(1,1u,false);f.typed.m_cloning=1;auto& e=f.typed.m_dataEntries;
    e.emplace_back(102,UTF8STRING("ACAD_ROUNDTRIP_2008_TABLE_ENTITY"));
    e.emplace_back(360,o.content);e.emplace_back(70,std::int32_t{2});e.emplace_back(90,std::int32_t{1});
    e.emplace_back(10,DRW_Coord(0,0,0));e.emplace_back(90,std::int32_t{0});e.emplace_back(90,std::int32_t{2});
    e.emplace_back(361,o.geometry);e.emplace_back(102,UTF8STRING("ACAD_ROUNDTRIP_PRE2007_TABLE"));
    e.emplace_back(90,std::int32_t{3});e.emplace_back(91,std::int32_t{3});
    e.emplace_back(102,UTF8STRING("ACAD_ROUNDTRIP_PRE2007_TABLECELL"));e.emplace_back(360,o.table);
    return f;
}
bool clear(const Result& r) {
    if(r.handle||r.owner||r.dataEndBit||r.payloadStartBit||r.payloadEndBit||r.handleEndBit)return false;
    for(const auto& x:r.references)if(x.groupCode||x.target||x.startBit||x.endBit)return false;
    return true;
}
bool matches(const Result& r) {
    return r.handle==0xbcu&&r.owner==0xbbu&&r.dataEndBit==1667u
        &&r.payloadStartBit==81u&&r.payloadEndBit==1657u&&r.handleEndBit==1699u
        &&r.references[0].groupCode==360&&r.references[0].target==0xb9u&&r.references[0].startBit==377u&&r.references[0].endBit==457u
        &&r.references[1].groupCode==361&&r.references[1].target==0xbau&&r.references[1].startBit==841u&&r.references[1].endBit==921u
        &&r.references[2].groupCode==360&&r.references[2].target==0xbdu&&r.references[2].startBit==1577u&&r.references[2].endBit==1657u;
}
bool refused(const Fixture& f){Result r;r.handle=7u;r.references[2].target=2u;return !DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)&&clear(r);}
bool generated() {
    auto f=build();const auto original=f.raw.m_rawBytes;Result r;
    if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!matches(r))return false;
    for(std::size_t n=0;n<original.size();++n){
        f.raw.m_rawBytes.assign(original.begin(),original.begin()+n);f.raw.m_objectSize=static_cast<std::uint32_t>(n);
        if(!refused(f))return false;
    }
    f=build();f.raw.m_version=DRW::AC1021;if(!refused(f))return false;
    f=build();f.raw.m_objectType=80;if(!refused(f))return false;
    f=build();f.raw.m_handle=0u;if(!refused(f))return false;
    f=build();f.raw.m_isEntity=true;if(!refused(f))return false;
    f=build();f.raw.m_isCustomClass=true;if(!refused(f))return false;
    f=build();f.raw.m_hasDataStorage=true;if(!refused(f))return false;
    f=build();f.raw.m_bodyBitSize=1;if(!refused(f))return false;
    f=build();++f.raw.m_objectSize;if(!refused(f))return false;
    f=build();f.raw.m_rawBytes.resize(4097u);f.raw.m_objectSize=4097u;if(!refused(f))return false;
    f=build();f.typed.m_dataEntries[1]=DRW_Variant(360,std::uint64_t{0xcau});if(!refused(f))return false;
    f=build();f.typed.m_dataEntries[1]=DRW_Variant(360,std::int32_t{185});if(!refused(f))return false;
    f=build();std::swap(f.typed.m_dataEntries[1],f.typed.m_dataEntries[7]);if(!refused(f))return false;
    f=build();f.typed.m_dataEntries[0]=DRW_Variant(102,UTF8STRING("wrong"));if(!refused(f))return false;
    f=build();f.typed.m_dataEntries[4]=DRW_Variant(10,DRW_Coord(-0.,0.,0.));if(!refused(f))return false;
    f=build();f.typed.m_dataEntries.pop_back();if(!refused(f))return false;
    f=build();f.typed.m_cloning=2;if(!refused(f))return false;
    f=build();f.typed.parentHandle=0xbau;if(!refused(f))return false;
    f=build();f.typed.reactorHandles[0]=0xbau;if(!refused(f))return false;
    f=build();f.typed.setDwgCommonObjectState(0,1u,false);if(!refused(f))return false;
    f=build();f.typed.xDictHandle=1u;if(!refused(f))return false;
    f=build();f.typed.m_values.emplace_back(90,std::int32_t{999});f.typed.m_handleValues.emplace_back(360,0x999u);
    if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!matches(r))return false;
    f=build();f.raw.setCommonLinkEvidence(DRW_DwgCommonLinkEvidence::Validated);f.raw.m_parentHandle=0xbbu;
    f.raw.m_numReactors=1;f.raw.m_reactorHandles={0xbbu};f.raw.m_xDictFlag=1u;
    if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!matches(r))return false;
    f.raw.m_parentHandle=0xbau;if(!refused(f))return false;
    f.raw.m_parentHandle=0xbbu;f.raw.m_reactorHandles[0]=0xbau;if(!refused(f))return false;
    f.raw.m_reactorHandles[0]=0xbbu;f.raw.m_numReactors=0;if(!refused(f))return false;
    f.raw.m_numReactors=1;f.raw.m_xDictFlag=0u;if(!refused(f))return false;
    f=build();f.raw.m_commonHandleDataValidated=true;if(!refused(f))return false;
    f=build();f.raw.setCommonLinkEvidence(DRW_DwgCommonLinkEvidence::ValidatedAbsent);if(!refused(f))return false;
    Options o;o.selfRole=5;if(!refused(build(o)))return false;
    o={};o.content=0x1234u;o.geometry=0x1235u;o.table=0x1236u;
    f=build(o);
    if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)
       ||r.references[0].target!=0x1234u||r.references[1].target!=0x1235u
       ||r.references[2].target!=0x1236u)return false;
    o={};o.ownerRole=5;if(!refused(build(o)))return false;
    o={};o.reactorRole=5;if(!refused(build(o)))return false;
    o={};o.reactor=0xbau;if(!refused(build(o)))return false;
    o={};o.owner=0xbcu;if(!refused(build(o)))return false;
    o={};o.content=0;if(!refused(build(o)))return false;
    o={};o.content=0xbcu;if(!refused(build(o)))return false;
    o={};o.geometry=o.content;if(!refused(build(o)))return false;
    o={};o.table=0x100000000ull;if(!refused(build(o)))return false;
    o={};o.table=0xbbu;if(!refused(build(o)))return false;
    o={};o.codepage=30;if(!refused(build(o)))return false;
    o={};o.badMarker=true;if(!refused(build(o)))return false;
    o={};o.badCode=true;if(!refused(build(o)))return false;
    o={};o.badPoint=true;if(!refused(build(o)))return false;
    o={};o.reservedLength=true;if(!refused(build(o)))return false;
    o={};o.eed=true;if(!refused(build(o)))return false;
    o={};o.extraReactor=true;if(!refused(build(o)))return false;
    o={};o.extension=true;if(!refused(build(o)))return false;
    o={};o.dataEndAdjustment=-1;if(!refused(build(o)))return false;
    o={};o.dataEndAdjustment=1;if(!refused(build(o)))return false;
    f=build();f.raw.m_rawBytes.push_back(0);++f.raw.m_objectSize;if(!refused(f))return false;
    f=build();f.raw.m_rawBytes.back()|=1;if(!refused(f))return false;
    f=build();f.typed.m_dataEntries[5]=DRW_Variant(90,std::int32_t{5});
    if(DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!clear(r))return false;
    f.typed.m_dataEntries[5]=DRW_Variant(90,std::int32_t{0});
    if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!matches(r))return false;
    f=build();fault::count=0;fault::active=true;
    const bool ok=DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r);fault::active=false;
    const auto count=fault::count;if(!ok||!matches(r)||count==0u||count>32u)return false;
    for(std::size_t i=0;i<count;++i){
        r.handle=1u;fault::remaining=static_cast<long>(i);
        const bool accepted=DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r);fault::remaining=-1;
        if(accepted||!clear(r)||f.raw.m_rawBytes!=original)return false;
        if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!matches(r))return false;
    }
    std::cout<<"generated ordered XRECORD controls passed; allocation faults="<<count<<'\n';return true;
}
bool callerBody(const char* path){
    std::ifstream input(path,std::ios::binary);std::vector<std::uint8_t> data((std::istreambuf_iterator<char>(input)),{});
    if(!input||data.empty())return false;auto f=build();f.raw.m_rawBytes=std::move(data);
    f.raw.m_objectSize=static_cast<std::uint32_t>(f.raw.m_rawBytes.size());Result r;
    if(!DwgTableRoundtripXRecordAc1018Candidate::inspect(f.raw,f.typed,r)||!matches(r))return false;
    std::cout<<"caller-supplied body and typed fixture match expected spans\n";return true;
}
}
int main(int argc,char** argv){if(argc>2||!generated()||(argc==2&&!callerBody(argv[1])))return 1;return 0;}
