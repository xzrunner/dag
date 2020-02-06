#pragma once

#include <queue>
#include <stack>

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

template <typename T>
std::vector<size_t> Evaluator::TopologicalSorting(const std::vector<std::shared_ptr<Node<T>>>& nodes)
{
    // prepare
    std::vector<int> in_deg(nodes.size(), 0);
    std::vector<std::vector<int>> out_devices(nodes.size());
    for (int i = 0, n = nodes.size(); i < n; ++i)
    {
        auto& device = nodes[i];
        for (auto& port : device->GetImports())
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
                    out_devices[j].push_back(i);
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
        for (auto& i : out_devices[v]) {
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