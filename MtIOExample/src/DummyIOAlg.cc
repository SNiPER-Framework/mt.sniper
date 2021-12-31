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
#include "RootWriter/RootWriter.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "TTree.h"

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
    SniperPtr<RootWriter> rw(this->getParent(), "RootWriter");
    if (!rw.valid())
    {
        LogError << "Failed to get RootWriter instance!" << std::endl;
        return false;
    }

    m_tree1 = rw->bookTree(*m_par, "FILE2/dummy1", "Dummy Test");
    m_tree1->Branch("Gid",  &m_gid,  "Gid/I");
    m_tree1->Branch("Lid",  &m_lid,  "Lid/I");
    m_tree1->Branch("iLeaf", &m_iLeaf, "iLeaf/I");

    m_tree2 = rw->bookTree(*m_par, "FILE1/dummy2", "Dummy Test");
    m_tree2->Branch("Gid",  &m_gid,  "Gid/I");
    m_tree2->Branch("Lid",  &m_lid,  "Lid/I");
    m_tree2->Branch("fLeaf", &m_fLeaf, "fLeaf/F");

    m_tree3 = rw->bookTree(*m_par, "FILE2/dummy3", "Dummy Test");
    m_tree3->Branch("Gid",  &m_gid,  "Gid/I");
    m_tree3->Branch("Lid",  &m_lid,  "Lid/I");
    m_tree3->Branch("dLeaf", &m_dLeaf, "dLeaf/D");

    SniperDataPtr<DummyStore> pStore(m_par, "/Event");
    if (pStore.invalid())
    {
        LogFatal << "Cann't find the data in memory" << std::endl;
        return false;
    }
    m_store = pStore.data();

    LogInfo << " initialized successfully" << std::endl;

    return true;
}

bool DummyIOAlg::execute()
{
    LogDebug << "in DummyIOAlg::execute()" << std::endl;

    ++m_count;

    DummyEvent *evt = m_store->event().get();
    evt->setLid(m_count);

    LogInfo << '\t' << evt->getGid() << '\t' << evt->getLid() << '\t' << evt->getNum() << std::endl;

    m_gid = evt->getGid();
    m_lid = evt->getLid();
    m_iLeaf = m_gid*2;
    m_fLeaf = m_gid*0.31;
    m_dLeaf = m_gid*0.71;

    m_tree1->Fill();
    m_tree2->Fill();
    m_tree3->Fill();

    return true;
}

bool DummyIOAlg::finalize()
{
    LogInfo << " finalized successfully" << std::endl;
    return true;
}
