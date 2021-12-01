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

#include "DummyIOAlg.h"
#include "DummyStore.h"
#include "DummyEvent.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"

DECLARE_ALGORITHM(DummyIOAlg);

DummyIOAlg::DummyIOAlg(const std::string &name)
    : AlgBase(name),
      m_count(0)
{
    // Using declare property
}

DummyIOAlg::~DummyIOAlg()
{
}

bool DummyIOAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperDataPtr<DummyStore> pStore(m_par, "/Event");
    if (pStore.invalid())
    {
        LogFatal << "Cann't find the data in memory" << std::endl;
        return false;
    }
    m_store = pStore.data();

    return true;
}

bool DummyIOAlg::execute()
{
    LogDebug << "in DummyIOAlg::execute()" << std::endl;

    ++m_count;

    DummyEvent *evt = m_store->event().get();
    evt->setLid(m_count);

    LogInfo << '\t' << evt->getGid() << '\t' << evt->getLid() << '\t' << evt->getNum() << std::endl;

    return true;
}

bool DummyIOAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;
    return true;
}
