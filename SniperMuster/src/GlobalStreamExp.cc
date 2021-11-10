#include <boost/python.hpp>
#include "SniperPrivate/GlobalStreamBase.h"
#include "SniperMuster/GlobalStreamFactory.h"

namespace bp = boost::python;

//用于暴露给python 产生globalStream
GlobalStreamBase* createGlobalStream(std::string& name){
    return GlobalStreamFactory::instance().create(name);
}

struct GlobalStreamBaseWrap: GlobalStreamBase, bp::wrapper<GlobalStreamBase>{
    bool configInput(boost::python::api::object& functor){
        return this->get_override("configInput")();
    }

    bool configOutput(boost::python::api::object& functor){
        return this->get_override("configOutput")();
    }

    void configBuffer(unsigned int capacity, unsigned int cordon){
        this->get_override("configBuffer")();
    }
};


void export_GlobalStream(){
    using namespace bp;
    //暴露出一个用于创建GlobalStream的工厂函数
    def("createGlobalStream", createGlobalStream, return_value_policy<reference_existing_object>());

    class_<GlobalStreamBaseWrap, boost::noncopyable>
        ("GlobalStreamBase")
        .def("configInput", pure_virtual(&GlobalStreamBase::configInput))
        .def("configOutput", pure_virtual(&GlobalStreamBase::configOutput))
        .def("configBuffer", pure_virtual(&GlobalStreamBase::configBuffer));
}

