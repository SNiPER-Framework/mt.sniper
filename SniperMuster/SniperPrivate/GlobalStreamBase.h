#ifndef SNIPER_GLOBAL_STREAM_BASE_H
#define SNIPER_GLOBAL_STREAM_BASE_H

#include <boost/python.hpp>

class GlobalStreamBase{
public:
    virtual bool configInput(boost::python::api::object& functor) = 0;
    virtual bool configOutput(boost::python::api::object& functor) = 0;
    virtual void configBuffer(unsigned int capacity, unsigned int cordon) = 0;
};

#endif