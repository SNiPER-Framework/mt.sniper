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

#include "GlobalStream.h"
#include "SniperMuster/GlobalBuffer.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include <thread>
#include <map>

namespace bp = boost::python;

static std::map<std::string, GlobalStream*> s_GBufMap;

GlobalStream* GlobalStream::Get(const std::string& name)
{
    auto it = s_GBufMap.find(name);
    if ( it == s_GBufMap.end() ) {
        throw SniperException(std::string("No GlobalStream ") + name);
    }
    return it->second;
}

GlobalStream::GlobalStream(const std::string& name)
    : m_buf(nullptr),
      m_name(name)
{
    s_GBufMap.insert(std::make_pair(name, this));
}

GlobalStream::~GlobalStream()
{
    // waiting for the input Task
    m_ithread.stop();  //stop the input stream
    m_buf->setOver(1);  //notify the input stream that being waiting
    m_ithread.join();

    // waiting for the output Task
    m_buf->setOver(2);
    m_othread.join();

    LogInfo << "releasing GlobalBuffer of " << m_name << std::endl;
    delete m_buf;
}

bool GlobalStream::configInput(bp::api::object& functor)
{
    bp::api::object task = functor();
    m_ithread.start(task);

    return true;
}

bool GlobalStream::configOutput(bp::api::object& functor)
{
    bp::api::object task = functor();
    m_othread.start(task);

    return true;
}

void GlobalStream::configBuffer(unsigned int capacity, unsigned int cordon)
{
    m_buf = new GlobalBuffer(capacity, cordon);
}
