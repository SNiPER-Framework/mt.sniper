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

#ifndef SNIPER_SNIPER_TBB_TASK_H
#define SNIPER_SNIPER_TBB_TASK_H

#include <tbb/task.h>

class TaskWatchDog;
class MusterContext;

class SniperTbbTask : public tbb::task
{
    public:

        SniperTbbTask(TaskWatchDog& snoopy);
        ~SniperTbbTask();

        // the override of tbb::task::execute()
        tbb::task* execute();

    private:

        bool           m_first;
        TaskWatchDog&  m_snoopy;
        MusterContext& m_context;

        // following interfaces are not supported
        SniperTbbTask() = delete;
        SniperTbbTask(const SniperTbbTask&) = delete;
        SniperTbbTask& operator=(const SniperTbbTask&) = delete;
};

#endif
