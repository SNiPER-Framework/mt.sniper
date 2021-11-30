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
#include "SniperMuster/GlobalStream.h"

class FillGBufAlg : public AlgBase
{
    public :

        FillGBufAlg(const std::string& name);
        virtual ~FillGBufAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private :

        int           m_max;
        GlobalBuffer<DummyEvent>* m_gbuf;
};

DECLARE_ALGORITHM(FillGBufAlg);

FillGBufAlg::FillGBufAlg(const std::string& name)
    : AlgBase(name)
{
    declProp("GenMax", m_max = 100);
}

FillGBufAlg::~FillGBufAlg()
{
}

bool FillGBufAlg::initialize()
{
    //m_gbuf = GlobalBuffer<DummyEvent>::FromStream("GEvtStream");
    m_gbuf = GlobalStream<DummyEvent>::GetBuffer("GEvtStream");
    return true;
}

bool FillGBufAlg::execute()
{
    static int gid = 0;

    if ( gid == m_max ) {
        m_gbuf->push_back(nullptr);
        getParent()->stop();
        return true;
    }

    m_gbuf->push_back(std::shared_ptr<DummyEvent>(new DummyEvent(++gid)));

    return true;
}

bool FillGBufAlg::finalize()
{
    return true;
}

