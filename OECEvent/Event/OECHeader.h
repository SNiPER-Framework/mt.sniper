#ifndef OECHEADER_H
#define OECHEADER_H

#include "Event/HeaderObject.h"
#include "EDMUtil/SmartRef.h"
#include "OECEvent.h"

using namespace std;

namespace JM{
    class OECHeader:public HeaderObject{
        public:
            OECHeader();
            ~OECHeader();

            //virtual function
            EventObject *event(const string &value);
            void setEventEntry(const string &eventName, Long64_t &value);
            
            void setEvent(OECEvent* value){m_event=value;}


        private:
            JM::SmartRef m_event;

        public:
            ClassDef(OECHeader,2)
    };
}

#endif
