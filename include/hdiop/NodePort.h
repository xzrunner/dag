#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hdiop
{

class Node;

struct NodePortAddr
{
    NodePortAddr() {}
    NodePortAddr(const std::shared_ptr<Node>& node, int idx)
        : node(node), idx(idx) {}
    NodePortAddr(const NodePortAddr& addr)
        : node(addr.node), idx(addr.idx) {}

    std::weak_ptr<Node> node;
    int idx = -1;
};

template <typename T>
struct NodePortVar
{
    T           type;
    std::string name;
};

template <typename T>
struct NodePort
{
    NodePort() {}
    NodePort(const NodePortVar<T>& var)
        : var(var) {}

    NodePortVar<T> var;
    std::vector<NodePortAddr> conns;
};


}