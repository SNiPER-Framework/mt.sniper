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

#include "ThreadAssistor.h"
#include "SyncAssistor.h"
#include "MusterContext.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperException.h"

ThreadAssistor::ThreadAssistor()
{
}

ThreadAssistor::~ThreadAssistor()
{
}

void ThreadAssistor::start(boost::python::api::object& task)
{
    m_task = task;
    std::thread thd( &ThreadAssistor::run, this);
    m_thread.swap( thd );
}

void ThreadAssistor::stop()
{
    boost::python::extract<Task&> xtask(m_task);
    xtask().stop();
}

void ThreadAssistor::join()
{
    m_thread.join();
}

void ThreadAssistor::run()
{
    boost::python::extract<Task&> xtask(m_task);
    if ( ! xtask.check() ) {
        throw ContextMsgException("Invalid Task object");
    }

    //Task& task = xtask();
    //task.setEvtMax( MusterContext::instance().evtMax() );
    TaskWatchDog& snoopy = xtask().Snoopy();

    SyncAssistor::wait();

    if ( snoopy.config() ) {
        if ( snoopy.initialize() ) {
            if ( ! snoopy.run() ) {
                //LogError << "Failed to execute algorithms" << std::endl;
            }
            snoopy.finalize();
        }
    }
}
