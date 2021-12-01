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

#ifndef SNIPER_MUSTER_CONTEXT_H
#define SNIPER_MUSTER_CONTEXT_H

#include "SniperKernel/SniperJSON.h"
#include <atomic>

class MusterContext
{
public:
    static MusterContext &instance() { return *s_obj; }

    static MusterContext &create();
    static void destroy();

    bool doNext()
    {
        // m_done.fetch_and_add should always be executed
        // return m_done.fetch_and_add(1) < m_evtMax || m_infinite;
        return m_done++ < m_evtMax || m_infinite;
    }

    int numWorkers() { return m_nWorkers; }
    long evtMax() { return m_evtMax; }
    void setNumWorkers(int n) { m_nWorkers = n; }
    void setEvtMax(long evtMax)
    {
        m_evtMax = evtMax;
        m_infinite = evtMax < 0;
    }

    SniperJSON jsonMuster();

    void setJsonWorker(const SniperJSON &json) { m_worker = json; }
    const SniperJSON &jsonWorker() { return m_worker; }

private:
    MusterContext();
    ~MusterContext() = default;

    bool m_infinite;
    int m_nWorkers;
    long m_evtMax;
    std::atomic_long m_done;

    SniperJSON m_worker;

    static MusterContext *s_obj;

    // following interfaces are not supported
    MusterContext(const MusterContext &) = delete;
    MusterContext &operator=(const MusterContext &) = delete;
};

#endif
