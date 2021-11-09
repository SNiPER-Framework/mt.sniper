#include "SniperMuster/GlobalStreamFactory.h"
#include "SniperKernel/SniperException.h"

//###############GlobalStreamFactory####################//
GlobalStreamFactory* GlobalStreamFactory::s_obj = nullptr;

GlobalStreamFactory& GlobalStreamFactory:: instance(){
    return (s_obj != nullptr)? *s_obj:*(s_obj = new GlobalStreamFactory);
}

void GlobalStreamFactory::release(){
    if(s_obj){
        delete s_obj;
        s_obj = nullptr;
    }
}

GlobalStreamBase* GlobalStreamFactory::create(std::string name){
    std::string type = name;
    std::string nobj = name;
    std::string::size_type pseg = name.find('/');
    if(pseg != std::string::npos){
        type = type.substr(0,pseg);
        nobj = nobj.substr(pseg + 1, std::string::npos);
    }

    type2CreatorMap::iterator it = m_creators.find(type);
    if(it != m_creators.end()){
        GlobalStreamBase* result = (it->second)(nobj);
        return result;
    }

    std::string msg = type + "is not a valid GlobalStream type";
    throw ContextMsgException(msg);
}

bool GlobalStreamFactory::book(const std::string& type, GSCreator creator){
    type2CreatorMap::iterator it = m_creators.find(type);
    if(it == m_creators.end()){
        m_creators.insert(std::make_pair(type, creator));
        return true;
    }

    std::string msg = type + "is a duplicated class name";
    throw ContextMsgException(msg);
}

GlobalStreamFactory::GlobalStreamFactory():
m_name("GlobalStreamFactory"){
    
}

GlobalStreamFactory::~GlobalStreamFactory()
{}

//###########################BookGlobalStream######################
BookGlobalStream::BookGlobalStream(const std::string type, GSCreator creator){
    GlobalStreamFactory::instance().book(type, creator);
}