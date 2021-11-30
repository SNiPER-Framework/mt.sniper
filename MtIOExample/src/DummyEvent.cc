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

#include "DummyEvent.h"

std::atomic_int DummyEvent::m_num{0};

DummyEvent::DummyEvent()
    : m_gid(-1),
      m_lid(-1)
{
    ++m_num;
}

DummyEvent::DummyEvent(int gid)
    : m_gid(gid),
      m_lid(-1)
{
    ++m_num;
}

DummyEvent::~DummyEvent()
{
    --m_num;
}

/// the declaration of dynamic stream creation
#include "SniperMuster/GlobalStream.h"
#include "SniperKernel/DeclareDLE.h"

typedef GlobalStream<DummyEvent> DummyEventStream;
SNIPER_DECLARE_DLE(DummyEventStream);
