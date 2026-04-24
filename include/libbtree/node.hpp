#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <optional>
#include <print>
#include <tuple>

#define BTREE_TEMPLATE template <std::uint16_t N, std::totally_ordered K, class V>

namespace btree
{

enum class Ordering
{
    Less,
    Equal,
    Greater
};

BTREE_TEMPLATE class BTreeNode
{
  public:
    BTreeNode() : parent_(nullptr)
    {
    }

    BTreeNode(BTreeNode<N, K, V> *parent) : parent_(parent)
    {
    }

    BTreeNode(BTreeNode<N, K, V> *parent, std::tuple<K, V> values[N]) : parent_(parent), values_(values)
    {
    }

    std::tuple<K, V> *getValues()
    {
        return values_;
    }

    void setValues(std::tuple<K, V> *values, std::uint16_t length)
    {
        std::copy(values, values + length, values_);
        values_length_ = length;
    }

    inline bool is_leaf() const
    {
        return children_length_ == 0;
    }

    std::uint16_t length() const
    {
        return values_length_;
    }

    bool should_split()
    {
        return values_length_ == (N - 1);
    }

    bool insert(K key, V value);
    std::optional<V> remove(K key);
    std::optional<V> get(K key);
    void traverse(void (*callback)(K key, V value));

  private:
    bool insertInLeaf(K key, V value);
    bool insertInNode(K key, V value);
    void insertChild(BTreeNode<N, K, V> *node);

    bool split_and_insert(K key, V value);
    bool splitWithParent(K key, V value);
    void splitWithoutParent();

    static constexpr std::uint16_t middle_max_length_ = (N - 1) / 2;

    std::uint16_t values_length_ = 0;
    std::uint16_t children_length_ = 0;

    std::tuple<K, V> values_[N - 1];
    BTreeNode<N, K, V> *children_[N];

    BTreeNode<N, K, V> *parent_;
};
} // namespace btree

#include "node.tpp"
