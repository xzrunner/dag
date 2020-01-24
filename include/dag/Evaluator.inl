#pragma once

#include <queue>

namespace dag
{

template <typename T>
void Evaluator::SetTreeDirty(const std::shared_ptr<Node<T>>& root)
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
bool Evaluator::HasNodeConns(const std::shared_ptr<Node<T>>& node)
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

}