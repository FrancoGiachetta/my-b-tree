#include <algorithm>
#include <concepts>
#include <cstdint>
#include <libbtree/node.hpp>
#include <print>

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

    return is_leaf() ? insertInLeaf(key, value) : insertInNode(key, value);
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::insertInLeaf(K key, V value)
{
    if (should_split())
    {
        return split_and_insert(key, value);
    }

    auto new_entry = std::tuple(key, value);

    for (std::uint16_t i = 0; i < values_length_; i++)
    {
        auto entry = values_[i];
        // Compare the current key with the one to insert.
        switch (compare_keys(key, std::get<0>(entry)))
        {
        case Ordering::Less:
            // Shift values.
            for (std::uint16_t j = values_length_; j > i; j--)
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
    if (should_split())
    {
        return split_and_insert(key, value);
    }

    for (std::uint16_t i = 0; i < values_length_; i++)
    {
        auto entry = values_[i];

        // Compare the current key with the one to insert.
        switch (compare_keys(key, std::get<0>(entry)))
        {
        case Ordering::Less:
            if (children_[i])
                return children_[i]->insert(key, value);
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

    if (children_[children_length_])
        return children_[values_length_]->insert(key, value);
    else
    {
        btree::BTreeNode<N, K, V> *new_node = new BTreeNode(this);

        children_[children_length_++] = new_node;

        return new_node->insertInLeaf(key, value);
    }
}

BTREE_TEMPLATE void BTreeNode<N, K, V>::insertChild(BTreeNode<N, K, V> *node)
{
    std::tuple<K, V> *node_values = node->getValues();

    for (std::uint16_t i = 0; i < values_length_; i++)
    {
        auto entry = values_[i];

        // Compare the current key with the one to insert.
        switch (compare_keys(std::get<0>(node_values[0]), std::get<0>(entry)))
        {
        case Ordering::Less: {
            for (std::uint16_t j = children_length_; j > i; j--)
                children_[j] = children_[j - 1];

            children_[i] = node;
            children_length_++;

            return;
        }
        case Ordering::Greater:
            break;
        case Ordering::Equal:
            return;
        }
    }

    children_[children_length_++] = node;
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
        std::println();
    }
    else
    {
        for (std::uint16_t i = 0; i < values_length_; i++)
        {
            std::tuple<K, V> entry = values_[i];
            callback(std::get<0>(entry), std::get<1>(entry));
        }
        std::println();

        for (std::uint16_t i = 0; i < children_length_; i++)
        {
            std::println("{{");
            children_[i]->traverse(callback);
            std::println("}}");
        }
    }
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::split_and_insert(K key, V value)
{

    if (parent_)
        return splitWithParent(key, value);
    else
    {
        splitWithoutParent();
        return is_leaf() ? insertInLeaf(key, value) : insertInNode(key, value);
    }
}

BTREE_TEMPLATE bool BTreeNode<N, K, V>::splitWithParent(K key, V value)
{
    for (int i = 0; i < values_length_; i++)
        std::println("KEY: {}", values_[i]);
    std::tuple<K, V> middle_value = values_[middle_max_length_];

    Ordering comparison = compare_keys(key, std::get<0>(middle_value));

    if (comparison == Ordering::Equal)
        return false;

    BTreeNode<N, K, V> *left_node = new BTreeNode(this);
    BTreeNode<N, K, V> *left_children[N];
    BTreeNode<N, K, V> *right_children[N];

    left_node->setValues(values_, (values_ + middle_max_length_) - values_);

    values_length_ = values_length_ / 2;

    switch (comparison)
    {
    case Ordering::Less: {
        for (std::uint16_t i = 0; i < values_length_; i++)
            values_[i] = values_[i + values_length_];

        parent_->insertInLeaf(key, value);
        parent_->insertChild(left_node);

        return true;
    }
    case Ordering::Greater: {
        for (std::uint16_t i = 0; i < values_length_; i++)
            values_[i] = values_[i + values_length_ + 1];

        values_length_--;

        parent_->insertInLeaf(std::get<0>(middle_value), std::get<1>(middle_value));
        parent_->insertChild(left_node);
        this->insert(key, value);

        return true;
    }
    }
}

BTREE_TEMPLATE void BTreeNode<N, K, V>::splitWithoutParent()
{
    std::tuple<K, V> middle_value = values_[middle_max_length_];
    BTreeNode<N, K, V> *left_node = new BTreeNode(this);
    BTreeNode<N, K, V> *right_node = new BTreeNode(this);
    BTreeNode<N, K, V> *left_children[N];
    BTreeNode<N, K, V> *right_children[N];

    left_node->setValues(values_, (values_ + middle_max_length_) - values_);
    right_node->setValues(values_ + (middle_max_length_), (values_ + values_length_) - (values_ + middle_max_length_));

    if (children_)
    {
        std::copy(children_, children_ + (middle_max_length_), left_children);
        std::copy(children_ + (middle_max_length_ + 1), children_ + values_length_ + 1, right_children);
    }

    values_[0] = middle_value;
    values_length_ = 1;

    children_[0] = left_node;
    children_[1] = right_node;

    children_length_ = 2;
}
