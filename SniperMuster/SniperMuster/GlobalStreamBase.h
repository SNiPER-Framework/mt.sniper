#ifndef SNIPER_GLOBAL_STREAM_BASE_H
#define SNIPER_GLOBAL_STREAM_BASE_H

#include "SniperKernel/DLElement.h"
#include <boost/python/object_core.hpp>

class GlobalStreamBase : public DLElement
{
public:
    GlobalStreamBase(const std::string &name) : DLElement(name) {}
    virtual ~GlobalStreamBase() {}

    // do nothing in the default initialize and finalize
    virtual bool initialize() override { return true; }
    virtual bool finalize() override { return true; }

    // pure virtual interfaces
    virtual bool configInput(boost::python::api::object &functor) = 0;
    virtual bool configOutput(boost::python::api::object &functor) = 0;
    virtual void configBuffer(unsigned int capacity, unsigned int cordon) = 0;
    virtual void join() = 0;
};

#endif
