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
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperMuster/GlobalBuffer.h"
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

        GlobalBuffer* m_gbuf;

        std::ofstream m_ofs;
};

DECLARE_ALGORITHM(WriteGBufAlg);

WriteGBufAlg::WriteGBufAlg(const std::string& name)
    : AlgBase(name)
{
}

WriteGBufAlg::~WriteGBufAlg()
{
}

bool WriteGBufAlg::initialize()
{
    m_gbuf = GlobalBuffer::FromStream("GEvtStream");

    //m_ofs.open("out_data.txt", std::ios::trunc);

    return true;
}

bool WriteGBufAlg::execute()
{
    DummyEvent* evt = static_cast<DummyEvent*>(m_gbuf->pop_front());

    if ( evt != nullptr ) {
        //m_ofs << evt->getGid() << '\t' << evt->getLid() << '\t' << evt->getNum() << std::endl;
        delete evt;
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

