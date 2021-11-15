#ifndef OECEVENT_H
#define OECEVENT_H
#include "Event/EventObject.h"
#include <string>

#include "TTimeStamp.h"

using namespace std;

namespace JM{
    class OECEvent:public EventObject{
        public:
            OECEvent();
            ~OECEvent();

            //get function
            std::string getIDA();//get detector
            std::string getIDB();//get top level category
            std::string getIDC();//get sub-level category
            float getEnergy(){return f_energy;}
            //float getTime(){return f_time;}
            TTimeStamp &getTime(){return m_time;}
            float getVertexX(){return f_vertexX;}
            float getVertexY(){return f_vertexY;}
            float getVertexZ(){return f_vertexZ;}
            int getTag(){return i_tag;}

            //set function
            void setEnergy(float e){f_energy=e;}
            //void setTime(float t){f_time=t;}
            void setTime(const TTimeStamp& value){m_time=value;}
            void setVertexX(float x){f_vertexX=x;}
            void setVertexY(float y){f_vertexY=y;}
            void setVertexZ(float z){f_vertexZ=z;}
            //void setTag(int tag){i_tag=tag;}
            bool addTag(string);
            bool addTag(int tag){i_tag=tag;return true;}
            bool isMuon();
            bool isMuonRelated();
            //bool addpTag(string);
            //bool addtTag(string);

        private:
            float f_energy;//reconstructed energy of single event
            TTimeStamp m_time;
            int n_clusters;
            float f_vertexX;//reconstructed vertex of single event
            float f_vertexY;
            float f_vertexZ;
            int i_tag;//the tag of event //eg: CD_muonUncor_IBD

        public:
            ClassDef(OECEvent,2)
    };
}
#endif
