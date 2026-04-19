#include <algorithm>
#include <concepts>
#include <cstdint>
#include <print>
#include <tuple>

using namespace btree;

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
    bool inserted = is_leaf() ? insertInLeaf(key, value) : insertInNode(key, value);

    if (should_split())
        split();

    return inserted;
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::insertInLeaf(K key, V value)
{
    auto new_entry = std::tuple(key, value);

    for (std::uint16_t i = 0; i < values_length_; i++)
    {
        auto entry = values_[i];
        // Compare the current key with the one to insert.
        switch (compare_keys(key, std::get<0>(entry)))
        {
        case Ordering::Less:
            // Shift values.
            for (std::uint16_t j = values_length_ - 1; j > i; j--)
                values_[j] = values_[j - 1];

            values_[i] = new_entry;
            values_length_++;

            return true;
        case Ordering::Greater:
            break;
        case Ordering::Equal:
            return false;
        }
    }

    values_[values_length_++] = new_entry;

    return true;
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::insertInNode(K key, V value)
{
    for (std::uint16_t i = 0; i < values_length_; i++)
    {
        auto entry = values_[i];

        // Compare the current key with the one to insert.
        switch (compare_keys(key, std::get<0>(entry)))
        {
        case Ordering::Less:
            if (children_[values_length_])
                return children_[values_length_]->insert(key, value);
            else
            {
                btree::BTreeNode<N, K, V> *new_node = new BTreeNode(this);

                for (std::uint16_t j = children_length_; j > i; j--)
                    children_[j] = children_[j - 1];

                children_[i] = new_node;
                children_length_++;

                return new_node->insertInLeaf(key, value);
            }
        case Ordering::Greater:
            break;
        case Ordering::Equal:
            return false;
        }
    }

    if (children_[values_length_])
        return children_[values_length_]->insert(key, value);
    else
    {
        btree::BTreeNode<N, K, V> *new_node = new BTreeNode(this);

        children_[values_length_] = new_node;
        children_length_++;

        return new_node->insertInLeaf(key, value);
    }
}

BTREE_TEMPLATE void BTreeNode<N, K, V>::split()
{
    std::tuple<K, V> middle_value = values_[middle_max_length_];
    std::tuple<K, V> left_values[N];
    std::tuple<K, V> right_values[N];

    BTreeNode<N, K, V> *left_node = new BTreeNode(this);
    BTreeNode<N, K, V> *right_node = new BTreeNode(this);

    std::copy(values_, values_ + (middle_max_length_), left_values);
    std::copy(values_ + (middle_max_length_ + 1), values_ + values_length_, right_values);

    left_node->setValues(left_values, (values_ + middle_max_length_) - values_);
    right_node->setValues(right_values, (values_ + values_length_) - (values_ + (middle_max_length_ + 1)));

    if (parent_)
        return;
    else
    {
        BTreeNode<N, K, V> *left_nodes[N + 1];
        BTreeNode<N, K, V> *right_nodes[N + 1];

        values_[0] = middle_value;

        values_length_ = 1;

        if (children_)
        {
            std::copy(children_, children_ + (middle_max_length_ - 1), left_nodes);
            std::copy(children_ + (middle_max_length_ + 1), children_ + values_length_ + 1, right_nodes);
        }

        children_[0] = left_node;
        children_[1] = right_node;

        children_length_ = 2;
    }
}

BTREE_TEMPLATE void BTreeNode<N, K, V>::traverse(void (*callback)(K key, V value))
{
    if (values_length_ == 0)
        return;

    if (is_leaf())
    {

        for (std::uint16_t i = 0; i < values_length_; i++)
        {
            std::tuple<K, V> entry = values_[i];
            callback(std::get<0>(entry), std::get<1>(entry));
        }
    }
    else
    {
        for (std::uint16_t i = 0; i < values_length_; i++)
        {
            std::tuple<K, V> entry = values_[i];
            callback(std::get<0>(entry), std::get<1>(entry));
        }

        for (std::uint16_t i = 0; i < children_length_ - 1; i++)
        {
            std::println("NEXT NODE");
            children_[i]->traverse(callback);
        }

        std::println("NEXT NODE");
        children_[values_length_]->traverse(callback);
    }
}
