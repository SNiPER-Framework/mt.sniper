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
#include "RootWriter/MtTTree.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperMuster/GlobalStream4Any.h"
#include <fstream>

class WriteGBufAlg : public AlgBase
{
    public :

        WriteGBufAlg(const std::string& name);
        virtual ~WriteGBufAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private :

        GlobalBuffer4Any* m_gbuf;

        std::string   m_fname;
        std::ofstream m_ofs;
};

DECLARE_ALGORITHM(WriteGBufAlg);

WriteGBufAlg::WriteGBufAlg(const std::string& name)
    : AlgBase(name)
{
    declProp("DataFile", m_fname);
}

WriteGBufAlg::~WriteGBufAlg()
{
}

bool WriteGBufAlg::initialize()
{
    m_gbuf = GlobalStream4Any::GetBufferFrom("GlobalStream");

    if ( ! m_fname.empty() ) {
        m_ofs.open(m_fname, std::ios::trunc);
    }

    return true;
}

bool WriteGBufAlg::execute()
{
    auto data = m_gbuf->pop_front();

    if ( data != nullptr ) {
        // write the event data
        auto &evt = my::any_cast<std::shared_ptr<DummyEvent> &>(data->at("event"));
        if ( ! m_fname.empty() ) {
            m_ofs << evt->getGid() << '\t' << evt->getLid() << '\t' << evt->getNum() << std::endl;
        }
        // write the ROOT TTree data
        auto &trees = my::any_cast<std::vector<MtTTree *> &>(data->at("trees"));
        for (auto tree : trees)
        {
            tree->DoFillOne();
        }
    }
    else {
        m_par->stop();
    }

    return true;
}

bool WriteGBufAlg::finalize()
{
    return true;
}
