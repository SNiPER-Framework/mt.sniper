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

#ifndef DUMMY_STORE_H
#define DUMMY_STORE_H

#include "DummyEvent.h"
#include "SniperKernel/IDataBlock.h"
#include "SniperMuster/GlobalStream4Any.h"
#include <memory>

class DummyStore : public IDataBlock
{
public:
    DummyStore() : m_evt(nullptr) {}
    virtual ~DummyStore();

    void adopt(std::shared_ptr<DummyEvent> &evt);
    std::shared_ptr<DummyEvent> &event() { return m_evt; }

    // for the convenience of testing
    void setRef(GlobalBuffer4Any::Elem *ref) { m_ref = ref; }
    GlobalBuffer4Any::Elem *ref() { return m_ref; }

private:
    std::shared_ptr<DummyEvent> m_evt;
    GlobalBuffer4Any::Elem *m_ref;
};

#endif
