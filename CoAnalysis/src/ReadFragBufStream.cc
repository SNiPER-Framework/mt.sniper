#include "ReadFragBufStream.h"
#include "SniperMuster/GlobalStream.h"

typedef GlobalBuffer<Fragment<JM::EvtNavigator>> FragBuf; 
typedef GlobalStream<Fragment<JM::EvtNavigator>> FragStream;

ReadFragBufStream::ReadFragBufStream():
    m_fbuf(nullptr),
    m_frag(nullptr),
    m_cEvt(0){}

bool ReadFragBufStream::initialize(){
    m_fbuf = FragStream::GetBuffer("GFragStream");
    std::cout << __FILE__ << " gbuf: " << m_fbuf << std::endl;
    return true;
}

bool ReadFragBufStream::next(int step, bool read){
    checkFrag();//确保Fragment准备就绪

    if(m_frag == nullptr){
        return false;
    }

    return true;
}

TObject* ReadFragBufStream::get(){
    JM::EvtNavigator* nav = (m_frag->evtDeque[m_cEvt]).get();    
    m_cEvt++;
    return nav; 
}

void ReadFragBufStream::checkFrag(){//不检查fragment中是否有事例，要求往GlobalBuffer里放入的时候确保fragment里有事例
    if(m_frag != nullptr && m_cEvt < m_frag->evtDeque.size()) return;
    
    //更新fragment
    std::cout << __FILE__ << " gbuf: " << m_fbuf << std::endl;
    auto _pElm = m_fbuf->next();
    m_frag = _pElm->dptr;
    std::cout << __FILE__ << " frag: " << m_frag << std::endl;
    m_fbuf->setDone(_pElm);      //将拿到的Elm的状态设为完成，允许被Buffer清理掉

    m_cEvt =0;
    if(!m_frag) return;//如果拿进来的fragment为nullptr，就检查完毕
}