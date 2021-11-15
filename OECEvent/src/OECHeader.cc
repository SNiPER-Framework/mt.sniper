#include "Event/OECHeader.h"

ClassImp(JM::OECHeader);

namespace JM{
    OECHeader::OECHeader(){
    }

    OECHeader::~OECHeader(){
    }

    EventObject* OECHeader::event(const string &value){
        if (value=="JM::OECEvent"){
            return m_event.GetObject();
        }
        return NULL;
    }
    void OECHeader::setEventEntry(const string &eventName, Long64_t &value){
        if (eventName=="JM::OECEvent"){
            m_event.setEntry(value);
        }
    }
}
