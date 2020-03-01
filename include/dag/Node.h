#pragma once

#include <string>
#include <vector>

#include <rttr/type>

namespace dag
{

template <typename T>
class Node
{
public:
    virtual void OnParmChanged(const std::string& parm_name) {}

    virtual void Execute() {}

    auto& GetImports() const { return m_imports; }
    auto& GetExports() const { return m_exports; }

    void SetName(const std::string& name) { m_name = name; }
    auto& GetName() const { return m_name; }

    bool IsDirty() const { return m_dirty; }
    void SetDirty(bool dirty) const { m_dirty = dirty; }

    void ClearConnections();

public:
    struct PortAddr
    {
        PortAddr() {}
        PortAddr(const std::shared_ptr<Node<T>>& node, int idx)
            : node(node), idx(idx) {}
        PortAddr(const PortAddr& addr)
            : node(addr.node), idx(addr.idx) {}

        std::weak_ptr<Node<T>> node;
        int idx = -1;
    };

    struct PortVar
    {
        T           type;
        std::string full_name;
        std::string short_name;
    };

    struct Port
    {
        Port() {}
        Port(const PortVar& var)
            : var(var) {}

        PortVar var;
        std::vector<PortAddr> conns;
    };

protected:
    std::vector<Port> m_imports, m_exports;

private:
    std::string m_name;

    mutable bool m_dirty = true;

    RTTR_ENABLE()

}; // Node

template <typename T>
void make_connecting(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to);

template <typename T>
void disconnect(const typename Node<T>::PortAddr& from, const typename Node<T>::PortAddr& to);

}

#include "dag/Node.inl"
