#pragma once

#include "dag/Node.h"

#include <map>

namespace dag
{

template <typename T>
class Graph
{
public:
    // update node
    void NodeChaged(const std::shared_ptr<Node<T>>& node);

    // update nodes
    void AddNode(const std::shared_ptr<Node<T>>& node);
    void RemoveNode(const std::shared_ptr<Node<T>>& node);
    void ClearAllNodes();

    // update node prop
    void PropChanged(const std::shared_ptr<Node<T>>& node);

    // update node conn
    void Connect(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to);
    void Disconnect(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to);
    void RebuildConnections(const std::vector<std::pair<typename Node<T>::PortAddr, typename Node<T>::PortAddr>>& conns);

    void MakeDirty();

    void Update();

    auto& GetAllNodes() const { return m_nodes_map; }

public:
    static void SetTreeDirty(const std::shared_ptr<Node<T>>& root);
    static bool HasNodeConns(const std::shared_ptr<Node<T>>& node);
    static std::vector<size_t> TopologicalSorting(const std::vector<std::shared_ptr<Node<T>>>& nodes);

private:
    std::map<std::string, std::shared_ptr<Node<T>>> m_nodes_map;

    bool m_dirty = false;

    size_t m_next_id = 0;

}; // Graph

}

#include "dag/Graph.inl"