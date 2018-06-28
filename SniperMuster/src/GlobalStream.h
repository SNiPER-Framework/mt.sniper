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

class GlobalBuffer;

class GlobalStream
{
    public :

        static GlobalStream* Get(const std::string& name);

        GlobalStream(const std::string& name);
        virtual ~GlobalStream();

        bool configInput(boost::python::api::object& functor);
        bool configOutput(boost::python::api::object& functor);
        void configBuffer(unsigned int capacity, unsigned int cordon);

        const std::string& objName() { return m_name; }

        GlobalBuffer*      buffer()  { return m_buf; }

    private :

        GlobalBuffer*      m_buf;
        const std::string  m_name;

        ThreadAssistor     m_ithread;
        ThreadAssistor     m_othread;

        // following interfaces are not supported
        GlobalStream() = delete;
        GlobalStream(const GlobalStream&) = delete;
        GlobalStream& operator=(const GlobalStream&) = delete;
};

#endif
