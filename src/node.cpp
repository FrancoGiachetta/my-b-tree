#include <concepts>
#include <cstdint>
#include <libbtree/btree.hpp>
#include <libbtree/node.hpp>

using namespace node;

template <std::totally_ordered K> static Ordering compare_keys(K key1, K key2)
{
    if (key1 < key2)
        return Ordering::Less;
    else
        return key1 > key2 ? Ordering::Greater : Ordering::Equal;
}

BTREE_TEMPLATE std::optional<V> BTreeNode<N, K, V>::get(K key)
{
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::insert(K key, V value)
{
    if (should_split())
    {
    }

    return is_leaf() ? insertInLeaf(key, value) : insertInNode(key, value);
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::insertInLeaf(K key, V value)
{
    auto new_entry = new std::tuple(key, value);

    for (std::uint16_t i = 0; i < values_length_; i++)
    {
        auto entry = values_[i];
        // Compare the current key with the one to insert.
        switch (compare_keys(entry[0], key))
        {
        case Ordering::Less:
            // Insert value.
            values_length_++;
            return true;
        case Ordering::Greater:
            break;
        case Ordering::Equal:
            return false;
        }
    }

    values_[++values_length_] = new_entry;

    return true;
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::insertInNode(K key, V value)
{
}
