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

#include "DummyMemMgr.h"
#include "DummyStore.h"
#include "BeginEvtHdl.h"
#include "EndEvtHdl.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(DummyMemMgr);

DummyMemMgr::DummyMemMgr(const std::string& name)
    : SvcBase(name)
{
}

DummyMemMgr::~DummyMemMgr()
{
}

bool DummyMemMgr::initialize()
{
    SniperPtr<DataMemSvc> pSvc(m_par, "DataMemSvc");
    pSvc->regist("/Event", new DummyStore());

    if ( m_par->find("InputSvc") != 0 ) {
        IIncidentHandler* bi = new BeginEvtHdl(m_par);
        bi->regist("BeginEvent");
        m_icdts.push_back(bi);
    }

    if ( m_par->find("OutputSvc") != 0 ) {
        IIncidentHandler* ei = new EndEvtHdl(m_par);
        ei->regist("EndEvent");
        m_icdts.push_back(ei);
    }

    return true;
}

bool DummyMemMgr::finalize()
{
    for ( auto it = m_icdts.rbegin(); it != m_icdts.rend(); ++it ) {
        delete *it;
    }
    return true;
}
