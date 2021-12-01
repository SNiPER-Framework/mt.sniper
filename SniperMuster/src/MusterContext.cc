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

#include "MusterContext.h"

MusterContext* MusterContext::s_obj = nullptr;

MusterContext& MusterContext::create()
{
    if ( s_obj == nullptr )
    {
        s_obj = new MusterContext();
    }
    return *s_obj;
}

void MusterContext::destroy()
{
    if ( s_obj != nullptr ) {
        delete s_obj;
        s_obj = nullptr;
    }
}

MusterContext::MusterContext()
    : m_infinite(false),
      m_nWorkers(0),
      m_evtMax(5),
      m_done(0)
{
}

SniperJSON MusterContext::jsonMuster()
{
    SniperJSON j;
    j["EvtMax"].from(m_evtMax);
    j["NumWorkers"].from(m_nWorkers);
    return j;
}
