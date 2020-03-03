#pragma once

#include "dag/Node.h"

#include <memory>

namespace dag
{

class Utility
{
public:
    template <typename T>
    static std::shared_ptr<Node<T>>
        GetInputNode(const Node<T>& node, size_t idx);

}; // Utility

}

#include "dag/Utility.inl"