// SPDX-License-Identifier: GPL-2.0-or-later
#include "lc3_compat_check.cpp"
#include "libdxfrw.h"
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

struct EllipseWriter : LC3CompatCheck {
    explicit EllipseWriter(dxfRW& value):writer(value) {}
    void writeEntities() override {accepted=writer.writeEllipse(&ellipse);}
    dxfRW& writer;DRW_Ellipse ellipse;bool accepted=false;
};
struct EllipseReader : LC3CompatCheck {
    void addEllipse(const DRW_Ellipse& value) override {ellipse=value;++count;}
    DRW_Ellipse ellipse;int count=0;
};
struct BoundaryWriter : LC3CompatCheck {
    BoundaryWriter(dxfRW& value,bool polygon):writer(value),mpolygon(polygon) {
        edge=std::make_shared<DRW_Ellipse>();edge->secPoint={1e200,0.,0.};edge->ratio=2.;
        edge->staparam=.25;edge->endparam=1.25;
        auto loop=std::make_shared<DRW_HatchLoop>(0);loop->numedges=1;loop->objlist.push_back(edge);
        if(mpolygon){polygonData.name="SOLID";polygonData.looplist.push_back(loop);}
        else {hatch.name="SOLID";hatch.looplist.push_back(loop);}
    }
    void writeEntities() override {accepted=mpolygon?writer.writeMPolygon(&polygonData):writer.writeHatch(&hatch);}
    dxfRW& writer;bool mpolygon,accepted=false;DRW_Hatch hatch;DRW_MPolygon polygonData;
    std::shared_ptr<DRW_Ellipse> edge;
};
struct BoundaryReader : LC3CompatCheck {
    void addHatch(const DRW_Hatch* value) override {capture(value);}
    void addMPolygon(const DRW_MPolygon* value) override {capture(value);}
    void capture(const DRW_Hatch* value) {
        ++count;
        if(value&&value->looplist.size()==1u&&value->looplist[0]
            &&value->looplist[0]->objlist.size()==1u) {
            const auto* ellipse=dynamic_cast<const DRW_Ellipse*>(value->looplist[0]->objlist[0].get());
            if(ellipse){axis=ellipse->secPoint;ratio=ellipse->ratio;}
        }
    }
    int count=0;DRW_Coord axis;double ratio=0.;
};
bool close(double a,double b) {return std::abs(a-b)<=1e-12*std::max(1.,std::abs(b));}
int main() {
    const auto path=std::filesystem::temp_directory_path()/
        ("libdxfrw-extreme-ellipse-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count())+".dxf");
    struct Cleanup{std::filesystem::path path;~Cleanup(){std::error_code error;std::filesystem::remove(path,error);}}cleanup{path};
    for(bool binary:{false,true})for(int mode:{0,1,2,3}) {
        dxfRW writer(path.string().c_str());EllipseWriter adapter(writer);
        const double scale=mode==0?1e200:mode==3?1e-200:1.;
        const double normal=mode==1?1e200:mode==2?1e-200:1.;
        adapter.ellipse.secPoint={scale,0.,0.};adapter.ellipse.extPoint={0.,0.,normal};
        adapter.ellipse.ratio=2.;adapter.ellipse.staparam=.25;adapter.ellipse.endparam=1.25;
        if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.accepted) return 1;
        if(adapter.ellipse.secPoint.x!=scale||adapter.ellipse.secPoint.y!=0.||adapter.ellipse.ratio!=2.)return 1;
        dxfRW reader(path.string().c_str());EllipseReader captured;
        if(!reader.read(&captured,false)||captured.count!=1
            ||!close(captured.ellipse.secPoint.x,0.)||!close(captured.ellipse.secPoint.y/scale,2.)
            ||captured.ellipse.ratio!=.5
            ||!close(captured.ellipse.endparam-captured.ellipse.staparam,1.)) {
            std::cerr<<"large finite ellipse axis normalization failed mode="<<mode<<'\n';return 1;
        }
        const double oldX=scale*std::cos(.25),oldY=2.*scale*std::sin(.25);
        const double newX=-scale*std::sin(captured.ellipse.staparam);
        const double newY=2.*scale*std::cos(captured.ellipse.staparam);
        if(!close(newX/scale,oldX/scale)||!close(newY/scale,oldY/scale))return 1;
        {std::ofstream sentinel(path,std::ios::binary|std::ios::trunc);sentinel<<"unchanged";}
        adapter.ellipse.secPoint={1e308,0.,0.};adapter.ellipse.extPoint={0.,0.,1.};adapter.ellipse.ratio=4.;
        if(writer.write(&adapter,DRW::AC1027,binary)||adapter.accepted)return 1;
        {std::ifstream sentinel(path,std::ios::binary);if(std::string(std::istreambuf_iterator<char>(sentinel),{})!="unchanged")return 1;}
        if(adapter.ellipse.secPoint.x!=1e308||adapter.ellipse.ratio!=4.)return 1;
        adapter.ellipse.secPoint={1.,0.,0.};
        if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.accepted)return 1;
        {std::ofstream sentinel(path,std::ios::binary|std::ios::trunc);sentinel<<"unchanged";}
        adapter.ellipse.staparam=-1e308;adapter.ellipse.endparam=1e308;
        if(writer.write(&adapter,DRW::AC1027,binary)||adapter.accepted)return 1;
        {std::ifstream sentinel(path,std::ios::binary);if(std::string(std::istreambuf_iterator<char>(sentinel),{})!="unchanged")return 1;}
        if(adapter.ellipse.staparam!=-1e308||adapter.ellipse.endparam!=1e308)return 1;
        adapter.ellipse.staparam=.25;adapter.ellipse.endparam=1.25;
        if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.accepted)return 1;
    }
    for(bool binary:{false,true})for(bool polygon:{false,true}) {
        dxfRW writer(path.string().c_str());BoundaryWriter adapter(writer,polygon);
        if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.accepted)return 1;
        if(adapter.edge->secPoint.x!=1e200||adapter.edge->ratio!=2.)return 1;
        dxfRW reader(path.string().c_str());BoundaryReader capture;
        if(!reader.read(&capture,false)||capture.count!=1||!close(capture.axis.x,0.)
            ||!close(capture.axis.y/1e200,2.)||capture.ratio!=.5)return 1;
        {std::ofstream sentinel(path,std::ios::binary|std::ios::trunc);sentinel<<"unchanged";}
        adapter.edge->secPoint={1e308,0.,0.};adapter.edge->ratio=4.;
        if(writer.write(&adapter,DRW::AC1027,binary)||adapter.accepted)return 1;
        {std::ifstream sentinel(path,std::ios::binary);if(std::string(std::istreambuf_iterator<char>(sentinel),{})!="unchanged")return 1;}
        if(adapter.edge->secPoint.x!=1e308||adapter.edge->ratio!=4.)return 1;
        adapter.edge->secPoint={1.,0.,0.};
        if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.accepted)return 1;
        {std::ofstream sentinel(path,std::ios::binary|std::ios::trunc);sentinel<<"unchanged";}
        adapter.edge->staparam=0.;adapter.edge->endparam=1e308;
        if(writer.write(&adapter,DRW::AC1027,binary)||adapter.accepted)return 1;
        {std::ifstream sentinel(path,std::ios::binary);if(std::string(std::istreambuf_iterator<char>(sentinel),{})!="unchanged")return 1;}
        if(adapter.edge->endparam!=1e308)return 1;
        adapter.edge->staparam=.25;adapter.edge->endparam=1.25;
        if(!writer.write(&adapter,DRW::AC1027,binary)||!adapter.accepted)return 1;
    }
    std::cout<<"extreme ELLIPSE axis normalization and retry PASS\n";return 0;
}
