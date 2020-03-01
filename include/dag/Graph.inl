#pragma once

#include "dag/Node.h"

#include <queue>
#include <stack>

namespace dag
{

template <typename T>
void Graph<T>::NodeChaged(const std::shared_ptr<Node<T>>& node)
{
    SetTreeDirty(node);

    m_dirty = true;
}

template <typename T>
void Graph<T>::AddNode(const std::shared_ptr<Node<T>>& node)
{
    auto name = node->GetName();
    while (name.empty() || m_nodes_map.find(name) != m_nodes_map.end())
    {
        if (node->GetName().empty()) {
            name = "node" + std::to_string(m_next_id++);
        } else {
            name = node->GetName() + std::to_string(m_next_id++);
        }
    }
    node->SetName(name);

    m_nodes_map.insert({ name, node });

    m_dirty = true;
}

template <typename T>
void Graph<T>::RemoveNode(const std::shared_ptr<Node<T>>& node)
{
    if (m_nodes_map.empty()) {
        return;
    }

    auto itr = m_nodes_map.find(node->GetName());
    if (itr == m_nodes_map.end()) {
        return;
    }

    SetTreeDirty(node);

    m_nodes_map.erase(itr);

    m_dirty = true;
}

template <typename T>
void Graph<T>::ClearAllNodes()
{
    if (m_nodes_map.empty()) {
        return;
    }

    m_nodes_map.clear();

    m_dirty = true;
}

template <typename T>
void Graph<T>::PropChanged(const std::shared_ptr<Node<T>>& node)
{
    SetTreeDirty(node);

    m_dirty = true;
}

template <typename T>
void Graph<T>::Connect(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to)
{
    make_connecting<T>(from, to);

    auto node = to.node.lock();
    assert(node);
    SetTreeDirty(node);

    m_dirty = true;
}

template <typename T>
void Graph<T>::Disconnect(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to)
{
    disconnect<T>(from, to);

    auto node = to.node.lock();
    assert(node);
    SetTreeDirty(node);

    m_dirty = true;
}

template <typename T>
void Graph<T>::RebuildConnections(const std::vector<std::pair<typename Node<T>::PortAddr, typename Node<T>::PortAddr>>& conns)
{
    // update dirty
    for (auto itr : m_nodes_map) {
        if (HasNodeConns(itr.second)) {
            SetTreeDirty(itr.second);
        }
    }

    // remove conns
    for (auto itr : m_nodes_map) {
        itr.second->ClearConnections();
    }

    // add conns
    for (auto& conn : conns)
    {
        auto node = conn.second.node.lock();
        assert(node);
        SetTreeDirty(node);
        make_connecting<T>(conn.first, conn.second);
    }

    m_dirty = true;
}

template <typename T>
void Graph<T>::MakeDirty()
{
    m_dirty = true;

    for (auto& itr : m_nodes_map) {
        itr.second->SetDirty(true);
    }
}

template <typename T>
void Graph<T>::Update()
{
    if (m_nodes_map.empty()) {
        return;
    }

    std::vector<std::shared_ptr<Node<T>>> nodes;
    nodes.reserve(m_nodes_map.size());
    for (auto itr : m_nodes_map) {
        nodes.push_back(itr.second);
    }
    auto sorted_idx = TopologicalSorting(nodes);

    for (auto& idx : sorted_idx)
    {
        auto node = nodes[idx];
        if (node->IsDirty()) {
            node->Execute();
            node->SetDirty(false);
        }
    }
}

template <typename T>
void Graph<T>::SetTreeDirty(const std::shared_ptr<Node<T>>& root)
{
    std::queue<const Node<T>*> buf;
    buf.push(root.get());
    while (!buf.empty())
    {
        auto n = buf.front(); buf.pop();
        n->SetDirty(true);
        for (auto& port : n->GetExports())
        {
            for (auto& conn : port.conns)
            {
                auto node = conn.node.lock();
                assert(node && node->get_type().is_derived_from<Node<T>>());
                buf.push(std::static_pointer_cast<Node<T>>(node).get());
            }
        }
    }
}

template <typename T>
bool Graph<T>::HasNodeConns(const std::shared_ptr<Node<T>>& node)
{
    for (auto& i : node->GetImports()) {
        if (!i.conns.empty()) {
            return true;
        }
    }
    for (auto& o : node->GetExports()) {
        if (!o.conns.empty()) {
            return true;
        }
    }
    return false;
}

template <typename T>
std::vector<size_t> Graph<T>::TopologicalSorting(const std::vector<std::shared_ptr<Node<T>>>& nodes)
{
    // prepare
    std::vector<int> in_deg(nodes.size(), 0);
    std::vector<std::vector<int>> out_nodes(nodes.size());
    for (int i = 0, n = nodes.size(); i < n; ++i)
    {
        auto& node = nodes[i];
        for (auto& port : node->GetImports())
        {
            if (port.conns.empty()) {
                continue;
            }

            assert(port.conns.size() == 1);
            auto from = port.conns[0].node.lock();
            assert(from);
            for (int j = 0, m = nodes.size(); j < m; ++j) {
                if (from == nodes[j]) {
                    in_deg[i]++;
                    out_nodes[j].push_back(i);
                    break;
                }
            }
        }
    }

    // sort
    std::stack<int> st;
    for (int i = 0, n = in_deg.size(); i < n; ++i) {
        if (in_deg[i] == 0) {
            st.push(i);
        }
    }
    std::vector<size_t> ret;
    while (!st.empty())
    {
        int v = st.top();
        st.pop();
        ret.push_back(v);
        for (auto& i : out_nodes[v]) {
            assert(in_deg[i] > 0);
            in_deg[i]--;
            if (in_deg[i] == 0) {
                st.push(i);
            }
        }
    }

    return ret;
}

}