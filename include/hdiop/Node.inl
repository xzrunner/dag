#pragma once

namespace hdiop
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
    if (!f_node || !t_node) {
        return;
    }

    assert(f_node->get_type().is_derived_from<Node<T>>());
    auto& f_port = std::static_pointer_cast<Node<T>>(f_node)->GetExports()[from.idx];
    bool finded = false;
    for (auto itr = f_port.conns.begin(); itr != f_port.conns.end(); )
    {
        if (itr->node.lock() == t_node && itr->idx == to.idx) {
            itr = const_cast<Node<T>::Port&>(f_port).conns.erase(itr);
            finded = true;
        } else {
            ++itr;
        }
    }
    assert(finded);

    assert(t_node->get_type().is_derived_from<Node<T>>());
    auto& t_port = std::static_pointer_cast<Node<T>>(t_node)->GetImports()[to.idx];
    finded = false;
    for (auto itr = t_port.conns.begin(); itr != t_port.conns.end(); )
    {
        if (itr->node.lock() == f_node && itr->idx == from.idx) {
            itr = const_cast<Node<T>::Port&>(t_port).conns.erase(itr);
            finded = true;
        } else {
            ++itr;
        }
    }
    assert(finded);
}

}