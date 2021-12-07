/* Copyright (C) 2018-2021
   Institute of High Energy Physics and Shandong University
   This file is part of mt.sniper.
 
   mt.sniper is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   mt.sniper is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public License
   along with mt.sniper.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef SNIPER_GLOBAL_STREAM_H
#define SNIPER_GLOBAL_STREAM_H

#include "SniperMuster/GlobalStreamBase.h"
#include "SniperMuster/GlobalBuffer.h"
#include "ThreadAssistor.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperException.h"
#include "boost/python.hpp"

template<typename T>
class GlobalStream : public GlobalStreamBase
{
public:
    static GlobalBuffer<T> *GetBufferFrom(const std::string &stream);

    GlobalStream(const std::string &name);
    virtual ~GlobalStream();

    virtual void configBuffer(unsigned int capacity, unsigned int cordon) override;
    virtual bool configInput(boost::python::api::object &functor) override;
    virtual bool configOutput(boost::python::api::object &functor) override;

    virtual void join() override;
    virtual SniperJSON json() override { return m_json; }

    GlobalBuffer<T> *buffer() { return m_buf; }

private:
    GlobalBuffer<T> *m_buf;
    ThreadAssistor m_ithread;
    ThreadAssistor m_othread;
    SniperJSON m_json;
};

template <typename T>
GlobalBuffer<T> *GlobalStream<T>::GetBufferFrom(const std::string &stream)
{
    auto it = s_GBufMap.find(stream);
    if (it == s_GBufMap.end())
    {
        throw SniperException(std::string("No GlobalStream ") + stream);
    }
    auto pstream = dynamic_cast<GlobalStream<T>*>(it->second);
    return pstream->buffer();
}

template <typename T>
GlobalStream<T>::GlobalStream(const std::string &name)
    : GlobalStreamBase(name),
      m_buf(nullptr)
{
    m_json["ordered_keys"].from(std::vector<std::string>{
        "Name",
        "GlobalBuffer",
        "InputTask",
        "OutputTask"});
    m_json["Name"].from(name);
}

template <typename T>
GlobalStream<T>::~GlobalStream<T>()
{
    LogInfo << "releasing GlobalBuffer of " << objName() << std::endl;
    delete m_buf;
}

template <typename T>
void GlobalStream<T>::configBuffer(unsigned int capacity, unsigned int cordon)
{
    m_buf = new GlobalBuffer<T>(capacity, cordon);
    auto &jbuf = m_json["GlobalBuffer"];
    jbuf["capacity"].from(capacity);
    jbuf["cordon"].from(cordon);
}

template <typename T>
bool GlobalStream<T>::configInput(boost::python::api::object &functor)
{
    boost::python::api::object task = functor();
    m_ithread.start(task);

    boost::python::extract<Task&> xtask(task);
    m_json["InputTask"] = xtask().json();
    m_json["InputTask"].erase("sniper");

    return true;
}

template <typename T>
bool GlobalStream<T>::configOutput(boost::python::api::object &functor)
{
    boost::python::api::object task = functor();
    m_othread.start(task);

    boost::python::extract<Task&> xtask(task);
    m_json["OutputTask"] = xtask().json();
    m_json["OutputTask"].erase("sniper");

    return true;
}

template <typename T>
void GlobalStream<T>::join()
{
    // all worker threads have finished, do not fill the buffer anymore
    m_buf->setStatus(false);

    // waiting for the input Task
    m_ithread.stop();  //stop the input stream
    m_buf->setOver(1);  //notify the input stream that being waiting
    m_ithread.join();

    // waiting for the output Task
    m_buf->setOver(2);
    m_othread.join();
}

#endif
