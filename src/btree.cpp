#include <libbtree/btree.hpp>

template <std::uint16_t N, std::totally_ordered K, class V> std::optional<V> btree::Btree<N, K, V>::get(K key)
{
}

template <std::uint16_t N, std::totally_ordered K, class V> bool btree::Btree<N, K, V>::insert(K key, V value)
{
    if (this->root_.insert(key, value))
    {
        length_++;
        return true;
    }
}

template <std::uint16_t N, std::totally_ordered K, class V> std::optional<V> btree::Btree<N, K, V>::remove(K key)
{
}
