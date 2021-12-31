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

#include "EndEvtHdl.h"
#include "DummyOutputSvc.h"
#include "DummyStore.h"
#include "DummyEvent.h"
#include "SniperKernel/ExecUnit.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/Incident.h"

EndEvtHdl::EndEvtHdl(ExecUnit* par)
    : IIncidentHandler(par)
{
    SniperPtr<DummyOutputSvc> oSvc(par, "OutputSvc");
    if ( oSvc.invalid() ) {
        LogFatal << "Cann't find OutputSvc for "
                 << par->scope() << par->objName() << std::endl;
        throw SniperException("OutputSvc is invalid");
    }
    m_oSvc = oSvc.data();

    SniperDataPtr<DummyStore> pStore(par, "/Event");
    if ( pStore.invalid() ) {
        LogFatal << "Cann't find Data Store" << std::endl;
        throw SniperException("Data Store is invalid");
    }
    m_store = pStore.data();
}

bool EndEvtHdl::handle(Incident& /*incident*/)
{
    m_oSvc->process( m_store->ref() );

    return true;
}
