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

#include "DummyOutputSvc.h"
#include "RootWriter/MtTTreeStore.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(DummyOutputSvc);

DummyOutputSvc::DummyOutputSvc(const std::string& name)
    : SvcBase(name)
{
}

DummyOutputSvc::~DummyOutputSvc()
{
}

bool DummyOutputSvc::initialize()
{
    SniperDataPtr<MtTTreeStore> tStore(m_par, "MtTTreeStore");
    m_treeStore = tStore.data();

    m_gbuf = GlobalStream4Any::GetBufferFrom("GlobalStream");

    return true;
}

bool DummyOutputSvc::finalize()
{
    return true;
}

void DummyOutputSvc::process(GlobalBuffer4Any::Elem* ref)
{
    //put the ROOT TTree data to the associated GlobalBuffer::Elem
    auto & trees = m_treeStore->trees();
    ref->dptr->insert(std::make_pair("trees", my::any(std::vector<MtTTree *>())));
    trees.swap(my::any_cast<std::vector<MtTTree *> &>(ref->dptr->at("trees")));

    return m_gbuf->setDone(ref);
}
