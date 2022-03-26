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

#include <tbb/flow_graph.h>
#include <unordered_set>

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/DLElement.h"
#include "SniperKernel/SniperJSON.h"
#include "SniperMuster/MtDagTask.h"

SNIPER_DECLARE_DLE(MtDagTask);

typedef const tbb::flow::continue_msg& msg_t;

MtDagTask::MtDagTask(const std::string& name)
        : TopTask(name) {
    m_tag = "MtDagTask";
    m_graph = new tbb::flow::graph();
}

MtDagTask::~MtDagTask() {
    for (node_t* cur : m_nodes) {
        delete cur;
    }
    delete m_graph;
}

AlgBase* MtDagTask::insertNode(const std::string& alg) {
    AlgBase* algPtr = this->createAlg(alg);
    m_algMap.insert({algPtr->objName(), algPtr});
    return algPtr;
}

bool MtDagTask::makeEdge(const std::string& alg1, const std::string& alg2) {

    if (!m_algMap.count(alg1) || !m_algMap.count(alg2)) {
        LogError << "Can't create edge between " << alg1 << "and" << alg2
                 << ". Please check algs has been inserted." << std::endl;
        return false;
    }
    m_edges.emplace_back(m_algMap.at(alg1), m_algMap.at(alg2));
    return true;
}

bool MtDagTask::done() {
    std::unordered_set<AlgBase*> second_nodes;
    for (const auto& p: m_edges)
        second_nodes.insert(p.second);

    // create nodes
    std::unordered_map<AlgBase*, node_t*> nodes_map;
    for (auto alg : m_algs.list()) {
        auto _alg = dynamic_cast<AlgBase *>(alg);
        node_t* cur_node = new node_t(*m_graph, [=](msg_t) {
            ScopedIncidentsPair sis{m_beginAlg.load(_alg), m_endAlg.load(_alg), *this};
            if (!_alg->execute()) {
                throw SniperException(alg->scope() + alg->objName() + " execute failed");
            }
        });
        m_nodes.emplace_back(cur_node);
        nodes_map.insert({_alg, cur_node});
        // pick up nodes which does not have predecessor
        if (!second_nodes.count(_alg)) {
            m_heads.emplace_back(cur_node);
        }
    }

    // make edges
    for (const auto& p : m_edges)
        tbb::flow::make_edge(*(nodes_map.at(p.first)), *(nodes_map.at(p.second)));

    return true;
}

SniperJSON MtDagTask::json() {
    SniperJSON j = TopTask::json();
    j["ordered_keys"].push_back(SniperJSON().from("nodes"));
    j["ordered_keys"].push_back(SniperJSON().from("edges"));

    if (!m_nodes.empty()) {
        SniperJSON& jnodes = j["nodes"];
        SniperJSON& jedges = j["edges"];
        for (auto alg : m_algs.list()) {
            auto _alg = dynamic_cast<AlgBase *>(alg);
            jnodes.push_back(_alg->json());
        }
        if (!m_edges.empty()) {
            for (const auto& p : m_edges) {
                jedges.push_back('"' + p.first->objName()
                        + '-' + p.second->objName() + '"');
            }
        }
    }
    auto& jalgs = j["algorithms"];
    jalgs = SniperJSON::loads("[]");
    return j;
}

/* eval is unavailable currently
void MtDagTask::eval(const SniperJSON& json) {
    // eval for base class
    TopTask::eval(json);

    // eval nodes and edges
    auto& nodes = json["nodes"];
    for (auto it = nodes.vec_begin(); it != nodes.vec_end(); ++it) {
        auto idStr = (*it)["identifier"].get<std::string>();
        if (idStr.front() != '[') {
            auto alg = this->insertNode(idStr);
            alg->eval(*it);
        }
    }

    auto& edges = json["edges"];
    for (auto it = edges.vec_begin(); it != edges.vec_end(); ++it) {
        auto edge = (*it).get<std::string>();
        std::string::size_type p = 0;
        while (edge[p] != '-')
            p++;
        this->makeEdge(edge.substr(0, p), edge.substr(p + 1, edge.size() - p - 1));
    }

    this->done();
}
*/

bool MtDagTask::config() {
    bool stat = TopTask::config();
    if (!stat)
        m_snoopy.setErr();
    return stat;
}

bool MtDagTask::initialize() {
    bool stat = TopTask::initialize();
    if (!stat)
        m_snoopy.setErr();
    return stat;
}

bool MtDagTask::finalize() {
    bool stat = TopTask::finalize();
    if (!stat)
        m_snoopy.setErr();
    return stat;
}

bool MtDagTask::execute() {

    if (m_limited && m_done >= m_evtMax) {
        m_snoopy.stop();
        return true;
    }

    try {
        if (m_snoopy.state() == Sniper::RunState::Stopped)
            return true;
        if (m_snoopy.isErr())
            return false;
        //BeginEvent is fired
        m_beginEvt.load(m_done).fire(*this);

        // send msg to head nodes
        for (node_t* cur_node : m_heads) {
            cur_node->try_put(tbb::flow::continue_msg());
        }
        // execute the graph
        m_graph->wait_for_all();

        //EndEvent is fired except there is an exception
        m_endEvt.load(m_done).fire(*this);
    }
    catch (StopRunThisEvent &e)
    {
        LogDebug << "stop current event and continue next one" << std::endl;
    }
    catch (StopRunProcess &e)
    {
        LogInfo << "stop run promtly." << std::endl;
        throw e;
    }
    catch (std::exception &e)
    {
        m_snoopy.setErr();
        LogError << e.what() << std::endl;
    }
    catch (...)
    {
        m_snoopy.setErr();
        LogError << "catch an unknown exception" << std::endl;
    }

    bool stat = !m_snoopy.isErr();
    if (stat)
    {
        ++m_done;
    }
    return stat;
}