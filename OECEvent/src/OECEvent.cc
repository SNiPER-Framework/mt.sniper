#include "Event/OECEvent.h"
#include <iostream>

ClassImp(JM::OECEvent);
namespace JM{
    OECEvent::OECEvent(){
        m_time=TTimeStamp(0,0);
        f_energy=-1;
        f_vertexX=0;
        f_vertexY=0;
        f_vertexZ=0;
        i_tag=0;
    }

    OECEvent::~OECEvent(){
    }

    string OECEvent::getIDA(){
        return "CD";
    }

    bool OECEvent::isMuon(){
        int i_temptag=unsigned(i_tag<<8)>>31;
        if (i_temptag==1) return true;
        else return false;
    }

    bool OECEvent::isMuonRelated(){
        int i_temptag=unsigned(i_tag<<9)>>31;
        if (i_temptag==1) return true;
        else return false;
    }

    bool OECEvent::addTag(string tag){
        int i_temptag=0;
        if (!tag.compare("muonCD")){
            i_temptag=0x10800000;
        }
        else if (!tag.compare("muonRelated")){
            i_temptag=0x00400000;
        }
        else if (!tag.compare("pIBD")){
            i_temptag=0x10204000;
        }
        else if (!tag.compare("dIBD")){
            i_temptag=0x10200400;
        }
        else if (!tag.compare("singles_LE")){
            i_temptag=0x10200001;
        }
        else if (!tag.compare("singles_ME")){
            i_temptag=0x10200002;
        }
        else if (!tag.compare("singles_HE")){
            i_temptag=0x10200004;
        }
        else if (!tag.compare("singles_XE")){
            i_temptag=0x10200008;
        }
        else if (!tag.compare("Other")){
            i_temptag=0x10200800;
        }
        else if (!tag.compare("IBD")){
            i_temptag=0x10200400;
        }
        else if (!tag.compare("BiPo")){
            i_temptag=0x10200200;
        }
        else if (!tag.compare("ProtonDecay")){
            i_temptag=0x10200100;
        }
        else if (!tag.compare("SpallationNeutron")){
            i_temptag=0x10200010;
        }
        else {
            //LogError<<"unreconginised string!!!"<<endl;
            return false;
        }
        i_tag=i_tag|i_temptag;
        return true;
        //i_tag=0;
        ////int i_temptag=0;
        ////if (!tag.compare("singles_LE")){
        ////    temptag=0x1040;
        ////}
        ////else if (!tag.compare("singles_ME")){
        ////    temptag=0x1040;
        ////}
        ////else if (!tag.compare("singles_HE")){
        ////    temptag=0x1040;
        ////}
        ////else if (!tag.compare("singles_XE")){
        ////    temptag=0x1040;
        ////}
        ////else if (!tag.compare("muWP")){
        ////    temptag=0x2010;
        ////}
        ////else if (!tag.compare("muTT")){
        ////    temptag=0x4010;
        ////}
        ////else if (!tag.compare("muShower")){
        ////    temptag=0x1010;
        ////}
        ////else if (!tag.compare("muShrTrack")){
        ////    temptag=0x1010;
        ////}
        ////else if (!tag.compare("muStop")){
        ////    temptag=0x1010;
        ////}
        ////else if (!tag.compare("muStpTrack")){
        ////    temptag=0x1010;
        ////}
        ////else if (!tag.compare("muSglTrack")){
        ////    temptag=0x1010;
        ////}
        ////else if (!tag.compare("muDblTrack")){
        ////    temptag=0x1010;
        ////}
        ////else if (!tag.compare("muNoTrack")){
        ////    temptag=0x1010;
        ////}
        ////else {
        ////    LogError<<"no tag named:"<<tag<<endl;
        ////    return false;
        ////}
        //if (!strcmp(tag.c_str(),"singles_LE")) i_tag=1;
        //else if (!strcmp(tag.c_str(),"singles_ME")) i_tag=2;
        //else if (!strcmp(tag.c_str(),"singles_HE")) i_tag=3;
        //else if (!strcmp(tag.c_str(),"singles_XE")) i_tag=4;
        //return true;
    }
}
