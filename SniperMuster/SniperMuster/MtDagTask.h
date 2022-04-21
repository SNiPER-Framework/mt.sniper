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

#ifndef SNIPER_MT_DAG_TASK_H
#define SNIPER_MT_DAG_TASK_H

#include <string>
#include <tbb/flow_graph.h>
#include <unordered_map>
#include <vector>

#include "SniperKernel/DagBase.h"

class AlgBase;
class SniperJSON;

class MtDagTask : public DagBase {

public:
    MtDagTask(const std::string& name);
    virtual ~MtDagTask();

    AlgBase* insertNode(const std::string& alg);
    bool makeEdge(const std::string& alg1, const std::string& alg2);
    // The graph has been build.
    bool done();

    //the json value of this object
    virtual SniperJSON json() override;
    // eval is unavailable currently
    // eval this graph object from json
    //virtual void eval(const SniperJSON &json) override;

protected:
    virtual bool config() override;
    virtual bool initialize() override;
    virtual bool finalize() override;
    virtual bool execute() override;


private:
    tbb::flow::graph* m_graph;
    std::unordered_map<std::string, AlgBase*> m_algMap;

    typedef tbb::flow::continue_node<tbb::flow::continue_msg> node_t;
    // nodes which does not have predecessor
    std::vector<node_t*> m_heads;
    std::vector<std::pair<AlgBase*, AlgBase*> > m_edges;
    std::vector<node_t*> m_nodes;
};

#endif