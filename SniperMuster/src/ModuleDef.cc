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
#include "SniperMuster/GlobalStreamBase.h"
#include <boost/python.hpp>

namespace bp = boost::python;

namespace SniperMuster
{
    GlobalStreamBase *createGlobalStream(const std::string &name);
    DLElement *createWorker(const std::string &name);
    void show();
}

struct GlobalStreamBaseWrap : GlobalStreamBase, bp::wrapper<GlobalStreamBase>
{
    GlobalStreamBaseWrap(const std::string &name)
        : GlobalStreamBase(name)
    {
    }

    bool configInput(boost::python::api::object &functor)
    {
        return this->get_override("configInput")(functor);
    }

    bool configOutput(boost::python::api::object &functor)
    {
        return this->get_override("configOutput")(functor);
    }

    void configBuffer(unsigned int capacity, unsigned int cordon)
    {
        this->get_override("configBuffer")(capacity, cordon);
    }

    void join()
    {
        this->get_override("join")();
    }

    SniperJSON json()
    {
        return this->get_override("json")();
    }
};

BOOST_PYTHON_MODULE(libSniperMuster)
{
    using namespace bp;

    def("createGlobalStream", SniperMuster::createGlobalStream, return_value_policy<manage_new_object>());
    def("createWorker", SniperMuster::createWorker, return_value_policy<manage_new_object>());
    def("show", SniperMuster::show);

    class_<Muster, boost::noncopyable>("Muster")
        .def("setEvtMax", &Muster::setEvtMax)
        .def("setIO", &Muster::setIO)
        .def("config", &Muster::config)
        .def("append", &Muster::append)
        .def("run", &Muster::run);

    class_<GlobalStreamBaseWrap, boost::noncopyable>("GlobalStreamBase", init<const std::string &>())
        .def("configInput", pure_virtual(&GlobalStreamBase::configInput))
        .def("configOutput", pure_virtual(&GlobalStreamBase::configOutput))
        .def("configBuffer", pure_virtual(&GlobalStreamBase::configBuffer))
        .def("join", pure_virtual(&GlobalStreamBase::join))
        .def("json", pure_virtual(&GlobalStreamBase::json));
}
