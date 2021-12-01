/* Copyright (C) 2021
   Institute of High Energy Physics and Shandong University
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

#include "SniperMuster/GlobalStreamBase.h"
#include "MusterContext.h"
#include "SniperKernel/Sniper.h"

namespace SniperMuster
{
    //用于暴露给python 产生globalStream
    GlobalStreamBase *createGlobalStream(const std::string &name)
    {
        return dynamic_cast<GlobalStreamBase *>(Sniper::create(name));
    }

    // to create worker Tasks
    DLElement *createWorker(const std::string &name)
    {
        static int n_threads = 0;

        auto sep = name.find('/');
        auto _type = name.substr(0, sep);
        auto _name = (sep != std::string::npos) ? name.substr(sep+1) : name;

        std::stringstream ss;
        ss << _type << "/(" << ++n_threads << ')' << _name;

        return Sniper::create(ss.str());
    }

    SniperJSON json()
    {
        std::vector<std::string> keys{
            "Muster",
            "(1)Worker",
            "GlobalStreams"};

        SniperJSON j;
        j["ordered_keys"].from(keys);

        j[keys[0]] = MusterContext::instance().jsonMuster();
        j[keys[1]] = MusterContext::instance().jsonWorker();
        j[keys[2]] = GlobalStreamBase::json_of_streams();

        return j;
    }

    void show()
    {
        std::cout << json().str(2) << std::endl;
    }
}
