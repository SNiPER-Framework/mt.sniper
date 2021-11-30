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

#include "BeginEvtHdl.h"
#include "DummyInputSvc.h"
#include "DummyStore.h"
#include "DummyEvent.h"
#include "SniperKernel/ExecUnit.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/Incident.h"

BeginEvtHdl::BeginEvtHdl(ExecUnit* par)
    : IIncidentHandler(par)
{
    SniperPtr<DummyInputSvc> iSvc(par, "InputSvc");
    if ( iSvc.invalid() ) {
        LogFatal << "Cann't find InputSvc for "
                 << par->scope() << par->objName() << std::endl;
        throw SniperException("InputSvc is invalid");
    }
    m_iSvc = iSvc.data();

    SniperDataPtr<DummyStore> pStore(par, "/Event");
    if ( pStore.invalid() ) {
        LogFatal << "Cann't find Data Store" << std::endl;
        throw SniperException("Data Store is invalid");
    }
    m_store = pStore.data();
}

bool BeginEvtHdl::handle(Incident& /*incident*/)
{
    auto ref = m_iSvc->next();
    if ( ref != nullptr ) {
        m_store->adopt(ref->dptr);
        m_store->setRef(ref);
        return true;
    }

    return m_domain.stop();
}
