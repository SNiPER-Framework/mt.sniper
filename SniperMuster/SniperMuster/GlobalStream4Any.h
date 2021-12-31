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

#ifndef GLOBAL_STREAM_FOR_ANY_H
#define GLOBAL_STREAM_FOR_ANY_H

#include "SniperMuster/GlobalStream.h"
#include <map>
#include <string>

#if __cplusplus >= 201703L

#include <any>
typedef GlobalStream<std::map<std::string, std::any>> GlobalStream4Any;
typedef GlobalBuffer<std::map<std::string, std::any>> GlobalBuffer4Any;
namespace my
{
    using std::any;
    using std::any_cast;
}

#else  // __cplusplus <= 201402L, use boost::any instead

#include <boost/any.hpp>
typedef GlobalStream<std::map<std::string, boost::any>> GlobalStream4Any;
typedef GlobalBuffer<std::map<std::string, boost::any>> GlobalBuffer4Any;
namespace my
{
    using boost::any;
    using boost::any_cast;
}

#endif  // __cplusplus

#endif  // GLOBAL_STREAM_FOR_ANY_H
