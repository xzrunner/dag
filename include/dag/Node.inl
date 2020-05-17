#pragma once

#include <assert.h>

namespace dag
{

template <typename T>
void Node<T>::ClearConnections()
{
    for (auto& p : m_imports) {
        p.conns.clear();
    }
    for (auto& p : m_exports) {
        p.conns.clear();
    }
}

template <typename T>
void make_connecting(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to)
{
	{
		auto node = from.node.lock();
		assert(node && node->get_type().is_derived_from<Node<T>>());
		auto& ports = std::static_pointer_cast<Node<T>>(node)->GetExports();
		assert(from.idx >= 0 && from.idx < static_cast<int>(ports.size()));
		const_cast<Node<T>::Port&>(ports[from.idx]).conns.push_back(to);
	}
	{
		auto node = to.node.lock();
        assert(node && node->get_type().is_derived_from<Node<T>>());
		auto& ports = std::static_pointer_cast<Node<T>>(node)->GetImports();
		assert(to.idx >= 0 && to.idx < static_cast<int>(ports.size())
            && ports[to.idx].conns.empty());
		const_cast<Node<T>::Port&>(ports[to.idx]).conns.push_back(from);
	}
}

template <typename T>
void disconnect(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to)
{
    auto f_node = from.node.lock();
    auto t_node = to.node.lock();
    if (f_node && t_node) {
        disconnect(*f_node, from.idx, *t_node, to.idx);
    }
}

template <typename T>
void disconnect(const Node<T>& f_node, int f_idx, const Node<T>& t_node, int t_idx)
{
    assert(f_node.get_type().is_derived_from<Node<T>>());
    auto& f_port = static_cast<const Node<T>&>(f_node).GetExports()[f_idx];
    bool finded = false;
    for (auto itr = f_port.conns.begin(); itr != f_port.conns.end(); )
    {
        if (itr->node.lock().get() == &t_node && itr->idx == t_idx) {
            itr = const_cast<Node<T>::Port&>(f_port).conns.erase(itr);
            finded = true;
        } else {
            ++itr;
        }
    }
    assert(finded);

    assert(t_node.get_type().is_derived_from<Node<T>>());
    auto& t_port = static_cast<const Node<T>&>(t_node).GetImports()[t_idx];
    finded = false;
    for (auto itr = t_port.conns.begin(); itr != t_port.conns.end(); )
    {
        if (itr->node.lock().get() == &f_node && itr->idx == f_idx) {
            itr = const_cast<Node<T>::Port&>(t_port).conns.erase(itr);
            finded = true;
        } else {
            ++itr;
        }
    }
    assert(finded);
}

}