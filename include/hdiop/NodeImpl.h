#pragma once

#include "hdiop/Node.h"
#include "hdiop/NodePort.h"

#include <string>
#include <vector>

namespace hdiop
{

template <typename T>
class NodeImpl : public Node
{
public:
    auto& GetImports() const { return m_imports; }
    auto& GetExports() const { return m_exports; }

    void SetName(const std::string& name) { m_name = name; }
    auto& GetName() const { return m_name; }

    void ClearConnections();

protected:
    std::vector<NodePort<T>> m_imports, m_exports;

private:
    std::string m_name;

    RTTR_ENABLE(Node)

}; // NodeImpl

void make_connecting(const NodePortAddr& from, const NodePortAddr& to);
void disconnect(const NodePortAddr& from, const NodePortAddr& to);

}

#include "hdiop/NodeImpl.inl"
