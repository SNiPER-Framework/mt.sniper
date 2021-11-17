/* Copyright (C) 2018
   Jiaheng Zou <zoujh@ihep.ac.cn> Tao Lin <lintao@ihep.ac.cn>
   Weidong Li <liwd@ihep.ac.cn> Xingtao Huang <huangxt@sdu.edu.cn>
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

#include "ThreadAssistor.h"
#include <string>
#include "SniperMuster/GlobalStreamBase.h"
#include "SniperMuster/GlobalBuffer.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include <thread>
#include <map>

template<typename T>
class GlobalStream : public GlobalStreamBase
{
    public :

        typedef GlobalBuffer<T> BufferType;

        GlobalStream(const std::string& name);
        virtual ~GlobalStream();

        virtual bool configInput(boost::python::api::object& functor);
        virtual bool configOutput(boost::python::api::object& functor);
        virtual void configBuffer(unsigned int capacity, unsigned int cordon);

        void join();

        const char*        scope()   { return ""; }
        const std::string& objName() { return m_name; }

        static BufferType* GetBuffer(const std::string& name);

        BufferType*      buffer()  { return m_buf; }

    private :

        static std::map<std::string, GlobalStream*> s_GBufMap;

        BufferType*      m_buf;
        const std::string  m_name;

        ThreadAssistor     m_ithread;
        ThreadAssistor     m_othread;

        // following interfaces are not supported
        GlobalStream() = delete;
        GlobalStream(const GlobalStream&) = delete;
        GlobalStream& operator=(const GlobalStream&) = delete;
};


namespace bp = boost::python;


template<typename T>
GlobalStream<T>::GlobalStream(const std::string& name)
    : GlobalStreamBase(name),
      m_buf(nullptr),
      m_name(name)
{
    s_GBufMap.insert(std::make_pair(name, this));
}

template<typename T>
GlobalStream<T>::~GlobalStream<T>()
{
    LogInfo << "releasing GlobalBuffer of " << m_name << std::endl;
    delete m_buf;
}

template<typename T>
bool GlobalStream<T>::configInput(bp::api::object& functor)
{
    bp::api::object task = functor();
    m_ithread.start(task);

    return true;
}

template<typename T>
bool GlobalStream<T>::configOutput(bp::api::object& functor)
{
    bp::api::object task = functor();
    m_othread.start(task);

    return true;
}

template<typename T>
void GlobalStream<T>::join()
{
    // waiting for the input Task
    m_ithread.stop();  //stop the input stream
    m_buf->setOver(1);  //notify the input stream that being waiting
    m_ithread.join();

    // waiting for the output Task
    m_buf->setOver(2);
    m_othread.join();
}

template<typename T>
void GlobalStream<T>::configBuffer(unsigned int capacity, unsigned int cordon)
{
    m_buf = new BufferType(capacity, cordon);
}

template<typename T>
typename GlobalStream<T>::BufferType* GlobalStream<T>::GetBuffer(const std::string& name){
    auto it = s_GBufMap.find(name);
    if ( it == s_GBufMap.end() ) {
        throw SniperException(std::string("No GlobalStream ") + name);
    }

    return it->second->buffer();
}

template<typename T>
typename std::map<std::string, GlobalStream<T>*> GlobalStream<T>:: s_GBufMap;

#endif
