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

#include "Muster.h"
#include <boost/python.hpp>

namespace bp = boost::python;

BOOST_PYTHON_MODULE(libSniperMuster)
{
    bp::class_<Muster, boost::noncopyable>("Muster")
        .def("setEvtMax", &Muster::setEvtMax)
        .def("config", &Muster::config)
        .def("append", &Muster::append)
        .def("run", &Muster::run)
        ;
}
