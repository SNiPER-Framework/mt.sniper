#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/EvtNavigator.h"
#include "SniperMuster/Fragment.h"
#include <memory>
#include <vector>
#include <string>

using namespace JM;
using namespace std;
typedef Fragment<EvtNavigator> EvtFrag;

class DummyFillFGBufAlg: public AlgBase{
public:
    DummyFillFGBufAlg(const string& name);
    virtual ~DummyFillFGBufAlg();

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

private:
    int m_max;
    GlobalBuffer<EvtFrag>* m_gbuf;

    shared_ptr<EvtFrag> createFrag();
    shared_ptr<EvtNavigator> createNav();
};


DummyFillFGBufAlg::DummyFillFGBufAlg(const string& name):
    AlgBase(name){
        declProp("GenMax", m_max = 100);
}

DummyFillFGBufAlg::~DummyFillFGBufAlg(){   
}

bool DummyFillFGBufAlg::initialize(){
    m_gbuf = GlobalBuffer<EvtFrag>::FromStream("GFragStream");
    return;
}

bool DummyFillFGBufAlg::execute(){//创建一个fragment，然后放入globalbuffer离
    static int gid = 0;
    if(gid == m_max){
        m_gbuf->push_back(nullptr);
        getParent()->finalize();
        return true;
    }

    m_gbuf->push_back(createFrag());

    return true;
}

bool DummyFillFGBufAlg::finalize(){
    return true;
}

shared_ptr<EvtFrag> DummyFillFGBufAlg::createFrag(){
    shared_ptr<EvtFrag> fragment(new EvtFrag);
    

    return fragment;
} 

shared_ptr<EvtNavigator> DummyFillFGBufAlg::createNav(){
    shared_ptr<EvtNavigator> event(new EvtNavigator);
    event->setTimeStamp(TTimeStamp());
    return event;
}
