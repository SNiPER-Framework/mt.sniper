/* Copyright (C) 2021
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

#ifndef SNIPER_GLOBAL_STREAM_BASE_H
#define SNIPER_GLOBAL_STREAM_BASE_H

#include "SniperKernel/DLElement.h"
#include <boost/python/object_core.hpp>

class GlobalStreamBase : public DLElement
{
public:
    GlobalStreamBase(const std::string &name);
    virtual ~GlobalStreamBase();

    // do nothing in the default initialize and finalize
    virtual bool initialize() override { return true; }
    virtual bool finalize() override { return true; }

    // pure virtual interfaces
    virtual void configBuffer(unsigned int capacity, unsigned int cordon) = 0;
    virtual bool configInput(boost::python::api::object &functor) = 0;
    virtual bool configOutput(boost::python::api::object &functor) = 0;
    virtual void join() = 0;

protected:
    static std::map<std::string, GlobalStreamBase *> s_GBufMap;
};

#endif
