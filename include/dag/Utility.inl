#pragma once

namespace dag
{

template <typename T>
std::shared_ptr<Node<T>>
Utility::GetInputNode(const Node<T>& node, size_t idx)
{
    auto& imports = node.GetImports();
    if (idx < 0 || idx >= imports.size()) {
        return nullptr;
    }

    auto& conns = imports[idx].conns;
    if (conns.empty()) {
        return nullptr;
    }

    assert(imports[idx].conns.size() == 1);
    return imports[idx].conns[0].node.lock();
}

}