#ifndef GLOBAL_STREAM_FACTORY_H
#define GLOBAL_STREAM_FACTORY_H

#include <string>
#include <map>
#include "SniperPrivate/GlobalStreamBase.h"

class GlobalStreamFactory{//这是一个单例
public:
    typedef GlobalStreamBase* (*GSCreator)(const std::string&);
    
    //get the singleton instance
    static GlobalStreamFactory& instance();

    //release the singleton instance
    static void release();

    //工厂生产一个GlobalStream 
    GlobalStreamBase* create(std::string name);

    //book the creator for GlobalStream of a specific type
    bool book(const std::string& type, GSCreator creator); 

private:
    typedef std::map<std::string, GSCreator> type2CreatorMap;

    //默认的构造函数
    GlobalStreamFactory();
    ~GlobalStreamFactory();
    
    std::string m_name;
    type2CreatorMap m_creators;

    static GlobalStreamFactory* s_obj;
};

class BookGlobalStream{
public:
    BookGlobalStream() = delete;
    BookGlobalStream(const BookGlobalStream&) = delete;
    BookGlobalStream& operator=(const BookGlobalStream&) = delete;

    typedef GlobalStreamBase* (*GSCreator)(const std::string&);
    BookGlobalStream(const std::string type, GSCreator creator);
};

template<typename T>
GlobalStreamBase* MusterCreateGS(const std::string& name){
    return new T(name);
}

#define DECLARE_GLOBAL_STREAM(GSCLASS)\
BookGlobalStream MusterBook_##GSCLASS##_(#GSCLASS, &MusterCreateGS<GSCLASS>)
#endif