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

#include "Muster.h"
#include "TaskSupervisor.h"
#include "MusterContext.h"
#include "SyncAssistor.h"
#include "SniperMuster/GlobalStreamBase.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperContext.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"
#include <tbb/task.h>
#include <tbb/global_control.h>
#include <boost/python.hpp>

namespace bp = boost::python;

Muster::Muster()
    : m_threads(0),
      m_gs(nullptr)
{
    MusterContext::create();
    sniper_context->set(Sniper::SysMode::MT);
    m_supervisor = new(tbb::task::allocate_root()) TaskSupervisor();
}

Muster::~Muster()
{
    LogInfo << "releasing all Muster Tasks" << std::endl;
    m_children.clear();
    MusterContext::destroy();
}

void Muster::setEvtMax(int evtMax)
{
    MusterContext::instance().setEvtMax(evtMax);
}

bool Muster::config(bp::api::object& functor, unsigned int nworkers)
{
    for ( unsigned int i = 0; i < nworkers; ++i )
    {
        append(functor);
    }

    MusterContext::instance().setNumWorkers(nworkers);
    if (nworkers > 0)
    {
        bp::extract<Task &> xobj(m_children.front());
        MusterContext::instance().setJsonWorker(xobj().json());
    }

    if (m_threads == 0)
    {
        m_threads = nworkers;
    }

    return true;
}

bool Muster::append(boost::python::api::object& functor)
{
    bp::api::object obj = functor();
    bp::extract<Task&> xobj(obj);

    if ( xobj.check() )
    {
        m_children.push_back(obj);
        m_supervisor->intake(xobj().Snoopy());
    }
    else
    {
        throw ContextMsgException("Invalid Task object");
    }

    return true;
}

bool Muster::run()
{
    sniper_context->set_threads(m_threads);

    tbb::global_control thread_control(tbb::global_control::max_allowed_parallelism, m_threads);

    SyncAssistor::run();

    LogInfo << "totally open " << m_threads << " tbb threads" << std::endl;
    tbb::task::spawn_root_and_wait(*m_supervisor);

    // waiting for the stop of global stream
    if (m_gs != nullptr)
    {
        m_gs->join();
    }

    // finalize the children (worker) tasks
    for (auto &child : m_children)
    {
        child.attr("Snoopy")().attr("finalize")();
    }

    LogInfo << "Muster.run() is finished" << std::endl;

    return true;
}
