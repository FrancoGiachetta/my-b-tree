#pragma once

#include <concepts>
#include <cstdint>
#include <optional>
#include <tuple>

#define BTREE_TEMPLATE template <std::uint16_t N, std::totally_ordered K, class V>

namespace node
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
    BTreeNode(bool is_leaf, std::optional<BTreeNode> parent) : parent_(parent)
    {
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
        return values_length_ == N;
    }

    bool insert(K key, V value);
    std::optional<V> remove(K key, V value);
    std::optional<V> get(K key);

  private:
    bool insertInLeaf(K key, V value);
    bool insertInNode(K key, V value);

    static constexpr std::uint16_t middle_max_length_ = N / 2;

    std::uint16_t values_length_ = 0;
    std::uint16_t children_length_ = 0;

    std::tuple<K, V> values_[N];
    BTreeNode<N, K, V> *childreen_[N + 1];

    std::optional<BTreeNode<N, K, V>> parent_ = std::nullopt;
};
} // namespace node
