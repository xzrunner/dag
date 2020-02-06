#pragma once

#include "dag/Node.h"

namespace dag
{

class Evaluator
{
public:
    template <typename T>
    static void SetTreeDirty(const std::shared_ptr<Node<T>>& root);

    template <typename T>
    static bool HasNodeConns(const std::shared_ptr<Node<T>>& node);

    template <typename T>
    static std::vector<size_t> TopologicalSorting(const std::vector<std::shared_ptr<Node<T>>>& nodes);

}; // Evaluator

}

#include "dag/Evaluator.inl"