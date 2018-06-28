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

#ifndef SNIPER_GLOBAL_BUFFER_H
#define SNIPER_GLOBAL_BUFFER_H

#include <string>
#include <mutex>
#include <condition_variable>

class GlobalBuffer
{
    public :

        static GlobalBuffer* FromStream(const std::string& name);

        struct Elem {
            void*  dptr;
            // -1:invalid, 0:ready, 1:being processed, 2:done
            long   stat;
            Elem*  next;
        };

        GlobalBuffer(int capacity, int cordon);
        virtual ~GlobalBuffer();

        void  push_back(void* dptr);
        void* pop_front();
        Elem* next();

        void  setDone(Elem* data);

        void  setOver(int step);
        unsigned int rough_size();

    private :

        unsigned int  m_capacity;
        unsigned int  m_cordon;
        Elem*         m_store;

        Elem*         m_begin;
        Elem*         m_end;
        Elem*         m_ref;

        std::mutex               m_mutemp;
        std::mutex               m_mutex0;
        std::mutex               m_mutex1;
        std::condition_variable  m_slotCond;
        std::condition_variable  m_dataCond;
        std::condition_variable  m_doneCond;
};

#endif
