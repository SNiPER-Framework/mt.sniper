#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperMuster/Fragment.h"
#include "EvtNavigator/EvtNavigator.h"
#include "EvtNavigator/NavBuffer.h"
#include <memory>

using namespace std;
using namespace JM;

typedef Fragment<EvtNavigator> Frag;

class InputFragAlg: public AlgBase{
public:
    InputFragAlg(string& name);
    virtual ~InputFragAlg();

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

private:
    int m_max;
    GlobalBuffer<Frag>* m_gbuf;
    NavBuffer*          m_nbuf;  

    int m_maxFragSize;
    int m_curFragSize;
    shared_ptr<Frag> m_frag;
    void addEvent();
    void fillFBuf();
    void fillBWindow();
    void fillEWindow();
};

DECLARE_ALGORITHM(InputFragAlg);

InputFragAlg::InputFragAlg(string& name):
    AlgBase(name),
    m_frag(new Frag){}

InputFragAlg::~InputFragAlg(){}

bool InputFragAlg::initialize(){
    m_gbuf = GlobalBuffer<Frag>::FromStream("GFragStream");
    
    //get the instance of NavBuffer
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_nbuf = navBuf.data();
    return true;
}


bool InputFragAlg::execute(){
    addEvent();
    
    if(m_curFragSize >= m_maxFragSize){//判断是否应该填入Fragment Buffer
        fillFBuf();
        m_frag.reset(new Frag);
        addEvent();
    }

    return true;
}

bool InputFragAlg::finalize(){
    return true;
}

void InputFragAlg::addEvent(){
    if(m_curFragSize == 0){//如果当前被填的Fragment为空，需要先将冗余窗口填进来
        m_frag->evtDeque.insert(m_frag->evtDeque.end(), m_nbuf->begin(), m_nbuf->current());
        m_frag->lbegin = 
    }

    m_frag->evtDeque.push_back(*m_nbuf->current());
    m_curFragSize++;
}

void InputFragAlg::fillFBuf(){
    fillEWindow();
    m_gbuf->push_back(m_frag);
    m_frag = nullptr;
    m_curFragSize = 0;
}



void InputFragAlg::fillEWindow(){}