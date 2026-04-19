#pragma once

#include <cstdint>
#include <libbtree/node.hpp>
#include <optional>

namespace btree
{

BTREE_TEMPLATE class BTree
{
  public:
    BTree()
    {
        root_ = BTreeNode<N, K, V>();
    }

    std::uint16_t order() const
    {
        return order_;
    }

    std::uint16_t length() const
    {
        return length_;
    }

    bool insert(K key, V value);
    std::optional<V> remove(K key);
    std::optional<V> get(K key);
    void traverse(void (*callback)(K key, V value));

  private:
    static constexpr std::uint16_t order_ = N;

    std::uint16_t length_ = 0;

    BTreeNode<N, K, V> root_;
};
} // namespace btree

#include "btree.tpp"
