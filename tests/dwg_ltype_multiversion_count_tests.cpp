// SPDX-License-Identifier: GPL-2.0-or-later
#include "intern/dwgobjectframe.h"
#include "intern/dwgreader.h"
#include "intern/dwgbufferw.h"
#include "intern/dwgwriter18.h"
#include "intern/dwgwriter24.h"
#include "intern/dwgwriter27.h"
#include "intern/dwgwriter32.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
dwgHandle ref(std::uint8_t code, std::uint32_t value) {
    dwgHandle h; h.code=code; h.ref=value; h.ref64=value;
    for (auto n=value; n; n >>= 8u) ++h.size;
    return h;
}
std::vector<std::uint32_t> children(unsigned count, bool legacyOrder = false) {
    if (legacyOrder) {
        std::vector<std::uint32_t> result{DRW::DwgLtypeByBlockHandle,
            DRW::DwgLtypeByLayerHandle, DRW::DwgLtypeContinuousHandle};
        for (unsigned i=1; i<count; ++i) result.push_back(0x1000u+i);
        return result;
    }
    std::vector<std::uint32_t> result{DRW::DwgLtypeContinuousHandle};
    for (unsigned i=1; i<count; ++i) result.push_back(0x1000u+i);
    result.push_back(DRW::DwgLtypeByLayerHandle);
    result.push_back(DRW::DwgLtypeByBlockHandle);
    return result;
}
bool slots(const DRW_ObjControl& obj, unsigned count,
           bool legacyOrder = false) {
    const auto expected=children(count, legacyOrder);
    return obj.orderedHandleSlots.size()==expected.size()
        && std::equal(obj.orderedHandleSlots.begin(),obj.orderedHandleSlots.end(),expected.begin());
}
struct Body {std::vector<std::uint8_t> data;std::uint32_t handleBits;};
Body build(DRW::Version version,unsigned count,bool legacy,int corruption=0,
           bool forceLong=false,bool legacyOrder=false,
           bool previousCurrentWriterOrder=false) {
    dwgBufferW b;
    b.putObjType(version,DRW::DwgLTypeControlObjectType);
    const auto sizeBit=b.bitCount();
    if(version<DRW::AC1024)b.putRawLong32(0);
    b.putHandle(ref(0,DRW::DwgLtypeControlHandle));
    b.putBitShort(0);b.putBitLong(0);b.putBit(0);
    if(version>DRW::AC1024)b.putBit(0);
    if(legacy)b.putBitShort(count);
    else if(forceLong){b.put2Bits(0);b.putRawLong32(count);}
    else b.putBitLong(count);
    if(corruption==5)b.putBit(0);
    if(version>DRW::AC1018)b.putBit(corruption==6 ? 1 : 0);
    if(version>=DRW::AC1024 && corruption>=7 && corruption<=12) {
        const auto pad=(8u-(b.bitCount()&7u))&7u;
        for(std::uint64_t bit=0;bit<pad;++bit)
            b.putBit(corruption==10 && bit==0 ? 1 : 0);
        for(int bit=0;bit<(corruption==9 ? 23 : 24);++bit)
            b.putBit(corruption==8 && bit==12 ? 1 : 0);
    }
    const auto boundary=static_cast<std::uint32_t>(b.bitCount());
    if(version<DRW::AC1024)b.patchRawLong32AtBit(sizeBit,boundary);
    b.putHandle(ref(corruption==1?0:DRW::DwgSoftPointer,0));
    b.putHandle(ref(DRW::DwgHardOwnership,0));
    auto values=children(count, legacyOrder);
    if (previousCurrentWriterOrder && !legacyOrder)
        std::swap(values[count], values[count + 1u]);
    if(corruption==2)values[0]=DRW::DwgLtypeByLayerHandle;
    if(corruption==3)values[3]=DRW::DwgLtypeContinuousHandle;
    for(std::size_t i=0;i<values.size();++i) {
        const auto role = corruption==12 && i==count
            ? DRW::DwgSoftOwnership
            : (legacyOrder ? DRW::DwgSoftOwnership
                           : (i >= count ? DRW::DwgHardOwnership
                                         : DRW::DwgSoftOwnership));
        b.putHandle(ref(role,values[i]));
    }
    b.alignToByte();
    auto data=b.data();
    if(corruption==4)data.pop_back();
    const auto handleBits=version>=DRW::AC1024
        ? static_cast<std::uint32_t>(data.size()*8u-boundary):0u;
    return {std::move(data),handleBits};
}
bool parse(const Body& b,DRW::Version version,unsigned count,
           DRW_ObjControl& object,bool legacyOrder=false,
           bool previousCurrentWriterOrder=false) {
    dwgBuffer in(const_cast<std::uint8_t*>(b.data.data()),b.data.size());
    if (!object.parseDwg(version,&in,b.handleBits)
        || object.orderedHandleRoles.size() != count + 2u)
        return false;
    auto expected=children(count,legacyOrder);
    if(previousCurrentWriterOrder&&!legacyOrder)
        std::swap(expected[count],expected[count+1u]);
    if(object.orderedHandleSlots!=expected)return false;
    for (std::size_t index = 0; index < object.orderedHandleRoles.size(); ++index) {
        const std::uint8_t expected = legacyOrder
            ? DRW::DwgSoftOwnership
            : (index < count ? DRW::DwgSoftOwnership
                             : DRW::DwgHardOwnership);
        if (object.orderedHandleRoles[index] != expected)
            return false;
    }
    return true;
}
}

class DwgBlockRecordXrefWireTestAccess {
public:
    template<class Writer> static bool check(unsigned count) {
        DRW_Header header;Writer writer(nullptr,&header);
        auto values=children(count);
        if(!writer.emitControlObject(DRW::DwgLTypeControlObjectType,
                                     DRW::DwgLtypeControlHandle,count,values)) {
            std::cerr<<"emit failed version="<<writer.m_version<<" count="<<count<<'\n';
            return false;
        }
        const auto found=std::find_if(writer.m_objectMap.begin(),writer.m_objectMap.end(),
            [](const auto& e){return e.first==DRW::DwgLtypeControlHandle;});
        if(found==writer.m_objectMap.end())return false;
        auto& data=writer.m_buf.data();dwgBuffer in(data.data(),data.size());DwgObjectFrame frame;
        if(!frame.readAt(in,writer.m_version,found->second))return false;
        dwgBuffer body(frame.body().data(),frame.body().size());
        if(body.getObjType(writer.m_version)!=DRW::DwgLTypeControlObjectType)return false;
        std::uint32_t boundary=0;
        if(writer.m_version<DRW::AC1024)boundary=body.getRawLong32();
        else boundary=frame.objectDataBitSize();
        if(body.getHandle().ref!=DRW::DwgLtypeControlHandle||body.getBitShort()!=0
            ||body.getBitLong()!=0||body.getBit()!=0)return false;
        if(writer.m_version>DRW::AC1024&&body.getBit()!=0)return false;
        dwgBuffer tag=body.forkIndependent();
        if(tag.get2Bits()!=(count<256?1:0)||body.getBitLong()!=static_cast<std::int32_t>(count))return false;
        if(writer.m_version>DRW::AC1018&&body.getBit()!=0)return false;
        const auto cursor=body.getPosition()*8u+body.getBitPos();
        if(writer.m_version<DRW::AC1024) {
            if(cursor!=boundary)return false;
        } else {
            // The existing modern writer pads to a byte, then writes its
            // 24-bit absent-string trailer before the handle section.
            const auto pad=(8u-(cursor&7u))&7u;
            if(cursor+pad+24u!=boundary)return false;
            for(std::uint64_t bit=0;bit<pad+24u;++bit)
                if(body.getBit()!=0||!body.isGood())return false;
        }
        const auto owner=body.getHandle(),xdict=body.getHandle();
        if(owner.code!=DRW::DwgSoftPointer||owner.ref!=0
            ||xdict.code!=DRW::DwgHardOwnership||xdict.ref!=0)return false;
        for(std::size_t i=0;i<values.size();++i){const auto h=body.getHandle();
            const auto expectedRole = i < count ? DRW::DwgSoftOwnership
                                                : DRW::DwgHardOwnership;
            if(!body.isGood()||h.code!=expectedRole||h.ref!=values[i]) {
                std::cerr<<"child mismatch version="<<writer.m_version<<" index="<<i
                    <<" role="<<static_cast<unsigned>(h.code)<<" ref="<<h.ref
                    <<" expectedRole="<<static_cast<unsigned>(expectedRole)
                    <<" expectedRef="<<values[i]<<'\n';
                return false;
            }}
        dwgBuffer again(frame.body().data(),frame.body().size());DRW_ObjControl parsed;
        const bool parsedOkay=parsed.parseDwg(writer.m_version,&again,frame.bodyBitSize());
        if(!parsedOkay||!slots(parsed,count)) {
            std::cerr<<"readback mismatch version="<<writer.m_version<<" count="<<count
                <<" parsed="<<parsedOkay<<" size="<<parsed.orderedHandleSlots.size()<<'\n';
            return false;
        }
        return true;
    }
};

int main(int argc,char** argv){
    if(argc==8&&std::string(argv[1])=="--physical") {
        const std::string v(argv[2]);
        const DRW::Version version=v=="ac1018"?DRW::AC1018:v=="ac1024"?DRW::AC1024:
            v=="ac1027"?DRW::AC1027:v=="ac1032"?DRW::AC1032:DRW::UNKNOWNV;
        if(version==DRW::UNKNOWNV)return 2;
        const unsigned count=static_cast<unsigned>(std::stoul(argv[3]));
        const auto offset=std::stoull(argv[5]);const auto size=std::stoull(argv[6]);
        const auto suppliedBoundary=std::stoull(argv[7]);
        std::ifstream file(argv[4],std::ios::binary);std::vector<std::uint8_t> bytes(
            (std::istreambuf_iterator<char>(file)),{});
        if(!file||offset>bytes.size()||size>bytes.size()-offset)return 2;
        dwgBuffer input(bytes.data()+offset,size);
        // The caller supplies the independently extracted physical data/handle
        // boundary, so the count decoder cannot define its own expected end.
        dwgBuffer probe=input.forkIndependent();
        if(probe.getObjType(version)!=DRW::DwgLTypeControlObjectType)return 1;
        std::uint64_t boundary=0;
        if(version<DRW::AC1024)boundary=probe.getRawLong32();
        if(probe.getHandle().ref==0||probe.getBitShort()!=0||probe.getBitLong()!=0)return 1;
        if(probe.getBit()!=1)return 1; // ODA producer's omitted XDICTIONARY.
        if(version>DRW::AC1024&&probe.getBit()!=0)return 1;
        if(probe.getBitLong()!=static_cast<std::int32_t>(count))return 1;
        if(version>DRW::AC1018&&probe.getBit()!=0)return 1;
        const auto end=probe.getPosition()*8u+probe.getBitPos();
        if(end!=suppliedBoundary)return 1;
        if(version<DRW::AC1024&&boundary!=suppliedBoundary)return 1;
        boundary=suppliedBoundary;
        if(boundary>size*8u)return 1;
        const auto nullOwner=probe.getHandle();
        if(!probe.isGood()||nullOwner.code!=DRW::DwgSoftPointer||nullOwner.ref!=0)return 1;
        for(unsigned i=0;i<count+2u;++i) {
            const auto child=probe.getHandle();
            const auto expectedRole=i<count?DRW::DwgSoftOwnership
                                           :DRW::DwgHardOwnership;
            if(!probe.isGood()||child.code!=expectedRole)return 1;
        }
        const auto bs=version>=DRW::AC1024?static_cast<std::uint32_t>(size*8u-boundary):0u;
        DRW_ObjControl parsed;
        if(!parsed.parseDwg(version,&input,bs)||parsed.orderedHandleSlots.size()!=count+2u)return 1;
        std::cout<<"physical ODA LTYPE_CONTROL PASS\n";return 0;
    }
    if(argc!=1)return 2;
    for(auto version:{DRW::AC1018,
                      DRW::AC1024,DRW::AC1027,DRW::AC1032}) {
        for(unsigned count:{255u,256u,257u}) {
            bool wire=false;
            if(version==DRW::AC1018)wire=DwgBlockRecordXrefWireTestAccess::check<dwgWriter18>(count);
            if(version==DRW::AC1024)wire=DwgBlockRecordXrefWireTestAccess::check<dwgWriter24>(count);
            if(version==DRW::AC1027)wire=DwgBlockRecordXrefWireTestAccess::check<dwgWriter27>(count);
            if(version==DRW::AC1032)wire=DwgBlockRecordXrefWireTestAccess::check<dwgWriter32>(count);
            if(!wire){std::cerr<<"writer wire failure version="<<version<<" count="<<count<<'\n';return 1;}
            for(bool legacy:{false,true}){auto body=build(version,count,legacy);DRW_ObjControl c;
                if(!parse(body,version,count,c)){std::cerr<<"reader failure version="<<version<<" count="<<count<<" legacy="<<legacy<<'\n';return 1;}}
        }
        for(int bad:{1,2,3,4,5}){auto invalid=build(version,257,true,bad);DRW_ObjControl c;
            if(parse(invalid,version,257,c)){std::cerr<<"invalid legacy accepted version="<<version<<" mode="<<bad<<'\n';return 1;}
            auto retry=build(version,257,false);if(!parse(retry,version,257,c)){std::cerr<<"retry failure\n";return 1;}}
        if(version>DRW::AC1018){auto invalid=build(version,257,true,6);DRW_ObjControl c;
            if(parse(invalid,version,257,c)){std::cerr<<"string footer accepted\n";return 1;}}
        if(version>=DRW::AC1024)for(unsigned count:{255u,256u,257u})for(bool legacy:{false,true}) {
            auto padded=build(version,count,legacy,7);DRW_ObjControl c;
            if(!parse(padded,version,count,c)){std::cerr<<"old writer zero footer rejected\n";return 1;}
            for(int bad:{8,9,10}) {auto invalid=build(version,count,legacy,bad);
                if(parse(invalid,version,count,c)){std::cerr<<"malformed old writer footer accepted\n";return 1;}
                if(!parse(padded,version,count,c)){std::cerr<<"footer retry failed\n";return 1;}}
        }
        if(version>=DRW::AC1024) {
            auto wrongTailRole=build(version,257,false,12);DRW_ObjControl c;
            if(parse(wrongTailRole,version,257,c)) {
                std::cerr<<"ODA-profile phantom owner role accepted version="
                    <<version<<'\n';return 1;
            }
            auto validTail=build(version,257,false,7);
            if(!parse(validTail,version,257,c)) {
                std::cerr<<"ODA-profile phantom role retry failed version="
                    <<version<<'\n';return 1;
            }
            auto oldWriterProfile=build(version,257,false,7,false,true);
            if(!parse(oldWriterProfile,version,257,c,true)) {
                std::cerr<<"legacy writer-order profile rejected version="
                    <<version<<'\n';return 1;
            }
        }
        {
            auto previousWriterProfile=build(version,257,false,7,false,false,true);
            DRW_ObjControl control;
            if(!parse(previousWriterProfile,version,257,control,false,true)) {
                std::cerr<<"previous current-writer phantom order rejected version="
                    <<version<<'\n';return 1;
            }
        }
        for(unsigned count:{1u,255u}){auto bad=build(version,count,false,5);DRW_ObjControl c;
            if(parse(bad,version,count,c)){std::cerr<<"compact body gap accepted\n";return 1;}
            auto good=build(version,count,false);if(!parse(good,version,count,c))return 1;
            auto full=build(version,count,false,0,true);if(!parse(full,version,count,c))return 1;}
    }
    std::cout<<"multi-version LTYPE_CONTROL count and legacy retry PASS\n";return 0;
}
