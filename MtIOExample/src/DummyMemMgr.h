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

#ifndef DUMMY_MEM_MGR_H
#define DUMMY_MEM_MGR_H

#include "SniperKernel/SvcBase.h"
#include <list>

class IIncidentHandler;

class DummyMemMgr : public SvcBase
{
    public :

        DummyMemMgr(const std::string& name);
        virtual ~DummyMemMgr();

        bool initialize();
        bool finalize();

    private :

        std::list<IIncidentHandler*> m_icdts;
};

#endif
