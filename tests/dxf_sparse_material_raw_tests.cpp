// SPDX-License-Identifier: GPL-2.0-or-later
#include "lc3_compat_check.cpp"
#include "libdxfrw.h"
#include "libdwgr.h"
#include "intern/dxfwriter.h"
#include "intern/dwgbufferw.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
namespace {
struct Reader:LC3CompatCheck,DRW_DxfSparseMaterialRawPolicy {
 bool enabled=true;
 bool wantsDxfSparseMaterialRawCapture() const noexcept override{return enabled;}
 std::vector<DRW_Material> materials;std::vector<DRW_RawDxfObject> raw;
 void addMaterial(const DRW_Material& v)override{materials.push_back(v);}
 void addRawDxfObject(const DRW_RawDxfObject& v)override{if(v.name=="MATERIAL")raw.push_back(v);}
};
struct LegacyReader:LC3CompatCheck {
 unsigned materialCallbacks=0,rawCallbacks=0;
 void addMaterial(const DRW_Material&)override{++materialCallbacks;}
 void addRawDxfObject(const DRW_RawDxfObject& v)override{if(v.name=="MATERIAL")++rawCallbacks;}
};
struct DxfWriter:LC3CompatCheck {
 dxfRW& out;DRW_Material value;DRW_RawDxfObject raw;bool useRaw=false,ok=false;
 explicit DxfWriter(dxfRW& w):out(w){}
 void writeObjects()override{ok=useRaw?out.writeRawDxfObject(&raw):out.writeMaterial(&value);}
 bool finalizeDxfWrite()override{return ok;}
};
struct DwgWriter:LC3CompatCheck {
 dwgRW& out;DRW_Material value;bool registered=false,written=false;
 explicit DwgWriter(dwgRW& w):out(w){}
 void writeDwgClasses()override{registered=out.registerMaterialObjectClass(&value);}
 void writeObjects()override{written=out.writeMaterial(&value);}
 bool finalizeDwgWrite()override{return true;}
};
struct DwgProbe:DRW_Material {using DRW_Material::encodeDwg;};
std::string bytes(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
bool source(const std::filesystem::path& path,bool binary,int mode,const char* name){
 std::ofstream f(path,std::ios::binary|std::ios::trunc);if(!f)return false;
 if(binary){const char signature[]="AutoCAD Binary DXF\r\n\x1a";f.write(signature,sizeof(signature));}
 std::unique_ptr<dxfWriter> w=binary?std::unique_ptr<dxfWriter>(new dxfWriterBinary(&f)):std::unique_ptr<dxfWriter>(new dxfWriterAscii(&f));
 const auto s=[&](int c,const char* v){return w->writeString(c,v);};
 const auto i=[&](int c,int v){return w->writeInt32(c,v);};
 const auto h=[&](int c,int v){return w->writeInt16(c,v);};
 if(!s(0,"SECTION")||!s(2,"HEADER")||!s(9,"$ACADVER")||!s(1,mode==6?"AC1024":"AC1027")||!s(0,"ENDSEC")
 ||!s(0,"SECTION")||!s(2,"OBJECTS")||!s(0,"MATERIAL")||!s(5,"F000")||!s(102,"{ACAD_REACTORS")
 ||!s(330,"E")||!s(102,"}")||!s(330,mode==9?"D":"E")||!s(100,"AcDbMaterial")||!s(1,mode==8?"OTHER":name)
 ||!h(71,1)||!i(91,mode==7?-1:-1023410177)||!h(72,1))return false;
 if(mode!=11 && !h(273,mode==2?0:1))return false;
 if(mode==3 && !h(71,1))return false;
 if(mode==4 && !s(2,"extra"))return false;
 if(!i(94,mode==1?62:63))return false;
 if(mode==5 && !s(2,"late"))return false;
 if(mode==12 && !i(94,63))return false;
 if(!s(0,"MATERIAL")||!s(5,"F001")||!s(330,"E")||!s(100,"AcDbMaterial")||!s(1,"following")
 ||!s(0,"ENDSEC")||!s(0,"EOF"))return false;
 return !w->hasWriteError();
}
}
int main(){
 const auto dir=std::filesystem::temp_directory_path()/("sparse-material-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));std::filesystem::create_directories(dir);
 struct Cleanup{std::filesystem::path p;~Cleanup(){std::error_code e;std::filesystem::remove_all(p,e);}}cleanup{dir};
 for(bool binary:{false,true}){
  const auto inputPath=dir/(binary?"in-binary.dxf":"in-ascii.dxf");const auto inputName=inputPath.string();dxfRW input(inputName.c_str());
  Reader r;
  for(int mode:{1,2,3,4,5,6,7,8,9,11,12,0}){
   if(!source(inputPath,binary,mode,"ByBlock"))return 1;r.materials.clear();r.raw.clear();const bool ok=input.read(&r,false);
   if(mode!=0){if(ok||!r.materials.empty()){std::cerr<<"invalid accepted "<<mode<<'\n';return 1;}continue;}
   if(!ok||r.materials.size()!=2u||r.raw.size()!=2u){std::cerr<<"input missing\n";return 1;}
  }
  for(const char* name:{"ByLayer","Global"}){if(!source(inputPath,binary,0,name))return 1;Reader other;if(!input.read(&other,false)||other.materials.size()!=2u||!other.materials[0].requiresRawDxfMaterialReplay())return 1;}
  if(!source(inputPath,binary,0,"ByBlock"))return 1;
  LegacyReader legacy;if(input.read(&legacy,false)||legacy.materialCallbacks||legacy.rawCallbacks){std::cerr<<"legacy consumer admitted\n";return 1;}
  Reader opt;opt.enabled=false;if(input.read(&opt,false)||!opt.materials.empty()||!opt.raw.empty()){std::cerr<<"disabled facet admitted\n";return 1;}
  opt.enabled=true;if(!input.read(&opt,false)||opt.materials.size()!=2u||opt.raw.size()!=2u||!opt.materials[0].requiresRawDxfMaterialReplay()){std::cerr<<"facet retry failed\n";return 1;}
  const auto original=r.materials[0];
  if(!original.requiresRawDxfMaterialReplay()||original.hasDxfMaterialImplicitTail()
    ||!original.m_hasDiffuseColorMethod||original.m_diffuseColorMethod!=1u
    ||!original.m_hasDiffuseColorValue||original.m_diffuseColorValue!=0xC2FFFFFFu
    ||!original.m_hasDiffuseMapSource||original.m_diffuseMapSource!=1u
    ||original.m_hasRefractionMapSource||!original.m_hasChannelFlags||original.m_channelFlags!=63
    ||r.materials[1].requiresRawDxfMaterialReplay())return 1;
  DRW_Material copied=original;DRW_Material moved=std::move(copied);if(!moved.requiresRawDxfMaterialReplay())return 1;
  DRW_Material assigned;assigned=moved;if(!assigned.requiresRawDxfMaterialReplay())return 1;
  assigned.m_hasChannelFlags=false;if(!assigned.requiresRawDxfMaterialReplay())return 1;
  moved.reset();if(moved.requiresRawDxfMaterialReplay()||!original.requiresRawDxfMaterialReplay())return 1;
  DwgProbe probe;static_cast<DRW_Material&>(probe)=original;dwgBufferW data,strings,handles;
  if(probe.encodeDwg(DRW::AC1027,&data,&strings,&handles)||!data.data().empty()||!strings.data().empty()||!handles.data().empty())return 1;
  const auto dest=dir/(binary?"out-binary.dxf":"out-ascii.dxf");{std::ofstream f(dest);f<<"prior";}
  const auto destName=dest.string();dxfRW writer(destName.c_str());DxfWriter adapter(writer);adapter.value=original;adapter.raw=r.raw[0];
  if(writer.write(&adapter,DRW::AC1027,binary)||adapter.ok||bytes(dest)!="prior"){std::cerr<<"typed output was not refused\n";return 1;}
  adapter.useRaw=true;adapter.ok=false;if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.ok){std::cerr<<"raw retry failed\n";return 1;}
  Reader reread;dxfRW check(destName.c_str());if(!check.read(&reread,false)||reread.materials.size()!=1u||!reread.materials[0].requiresRawDxfMaterialReplay())return 1;
  std::vector<int> actual;for(const auto& g:reread.raw[0].groups)actual.push_back(g.code());
  const std::vector<int> expected={5,102,330,102,330,100,1,71,91,72,273,94};if(actual!=expected
    ||reread.raw[0].groups[7].i_val()!=1||reread.raw[0].groups[9].i_val()!=1
    ||reread.raw[0].groups[10].i_val()!=1||reread.raw[0].groups[11].i_val()!=63
    ||reread.materials[0].m_diffuseColorValue!=0xC2FFFFFFu)return 1;
  const auto dwgPath=dir/"out.dwg";{std::ofstream f(dwgPath);f<<"prior dwg";}
  const auto dwgName=dwgPath.string();dwgRW dwg(dwgName.c_str());DwgWriter dwgAdapter(dwg);dwgAdapter.value=original;
  if(dwg.write(&dwgAdapter,DRW::AC1027,true)||!dwgAdapter.registered||dwgAdapter.written||bytes(dwgPath)!="prior dwg")return 1;
  dwgAdapter.value.reset();dwgAdapter.value.handle=0xF000u;dwgAdapter.value.m_name="corrected";
  dwgAdapter.registered=dwgAdapter.written=false;
  if(!dwg.write(&dwgAdapter,DRW::AC1027,true)||!dwgAdapter.registered||!dwgAdapter.written){std::cerr<<"DWG retry failed\n";return 1;}
 }
 return 0;
}
