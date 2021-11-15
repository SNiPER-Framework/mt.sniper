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

#ifndef SNIPER_THREAD_ASSISTOR_H
#define SNIPER_THREAD_ASSISTOR_H

#include <boost/python.hpp>
#include <thread>

class ThreadAssistor
{
    public :

        ThreadAssistor();
        virtual ~ThreadAssistor();

        void start(boost::python::api::object& task);

        void stop();

        void join();

    private :

        void run();

        std::thread m_thread;

        boost::python::api::object m_task;
};

#endif
