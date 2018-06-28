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

#ifndef END_EVENT_HANDLER_H
#define END_EVENT_HANDLER_H

#include "SniperKernel/IIncidentHandler.h"

class Task;
class DummyOutputSvc;
class DummyStore;

class EndEvtHdl : public IIncidentHandler
{
    public :

        EndEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        DummyOutputSvc* m_oSvc;
        DummyStore*     m_store;
};

#endif
