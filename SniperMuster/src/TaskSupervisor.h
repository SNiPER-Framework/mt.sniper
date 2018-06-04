/* Copyright (C) 2018
   Jiaheng Zou <zoujh@ihep.ac.cn> Tao Lin <lintao@ihep.ac.cn>
   Weidong Li <liwd@ihep.ac.cn> Xingtao Huang <huangxt@sdu.edu.cn>
   This file is part of sniper.mt.
 
   sniper.mt is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   sniper.mt is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public License
   along with sniper.mt.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef SNIPER_TASK_SUPERVISOR
#define SNIPER_TASK_SUPERVISOR

#include <tbb/task.h>
#include <list>

class TaskWatchDog;
class SniperTbbTask;

class TaskSupervisor : public tbb::task
{
    public:

        TaskSupervisor() : m_stat(false) {}

        // map a sniper TaskWatchDog to a tbb::task
        bool intake(TaskWatchDog& snoopy);

        // the override of tbb::task::execute()
        tbb::task* execute();

        const char* objName() { return "TaskSupervisor"; }

    private:

        // init value is false, then set true for continuation
        bool m_stat;

        // children of this supervisor
        // no tbb::task_list::size() available, list is better
        std::list<SniperTbbTask*> m_children;

        // following interfaces are not supported
        TaskSupervisor(const TaskSupervisor&) = delete;
        TaskSupervisor& operator=(const TaskSupervisor&) = delete;
};

#endif
