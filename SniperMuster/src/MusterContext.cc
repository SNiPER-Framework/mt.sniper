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
      m_evtMax(5),
      m_done(0)
{
}
