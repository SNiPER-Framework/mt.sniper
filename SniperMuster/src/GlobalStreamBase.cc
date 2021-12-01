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

#include "SniperMuster/GlobalStreamBase.h"

std::map<std::string, GlobalStreamBase *> GlobalStreamBase::s_GBufMap;

GlobalStreamBase::GlobalStreamBase(const std::string &name)
    : DLElement(name)
{
    s_GBufMap.insert(std::make_pair(name, this));
}

GlobalStreamBase::~GlobalStreamBase()
{
}

SniperJSON GlobalStreamBase::json_of_streams()
{
    SniperJSON j;
    for (auto &s : s_GBufMap)
    {
        SniperJSON sj = s.second->json();
        sj["name"].from(s.first);
        j.push_back(sj);
    }

    //if no GlobalStream is created, make it be an empty vector
    if (!j.valid())
    {
        j.from(std::vector<char>());
    }

    return j;
}
