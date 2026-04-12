#include <concepts>
#include <cstdint>
#include <libbtree/btree.hpp>
#include <optional>

template <std::uint16_t N, std::totally_ordered K, class V> std::optional<V> btree::Btree<N, K, V>::get(K key)
{
}

template <std::uint16_t N, std::totally_ordered K, class V> void btree::Btree<N, K, V>::insert(K key, V value)
{
}

template <std::uint16_t N, std::totally_ordered K, class V> std::optional<V> btree::Btree<N, K, V>::remove(K key)
{
}
