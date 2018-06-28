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

#ifndef SNIPER_MUSTER_CONTEXT_H
#define SNIPER_MUSTER_CONTEXT_H

#include <atomic>

class MusterContext
{
    public:
        
        static MusterContext& instance() { return *s_obj; }

        static MusterContext& create();
        static void destroy();

        bool doNext()
        {
            // m_done.fetch_and_add should always be executed
            //return m_done.fetch_and_add(1) < m_evtMax || m_infinite;
            return m_done++ < m_evtMax || m_infinite;
        }

        int  evtMax() { return m_evtMax; }
        void setEvtMax(int evtMax) { m_evtMax = evtMax; m_infinite = evtMax < 0; }
        void set_threads(int n);

    private:

        MusterContext();
        ~MusterContext() = default;

        bool m_infinite;
        int  m_evtMax;
        std::atomic_int m_done;

        static MusterContext* s_obj;

        // following interfaces are not supported
        MusterContext(const MusterContext&) = delete;
        MusterContext& operator=(const MusterContext&) = delete;
};

#endif
