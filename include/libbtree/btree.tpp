using namespace btree;

BTREE_TEMPLATE std::optional<V> BTree<N, K, V>::get(K key)
{
}

BTREE_TEMPLATE bool BTree<N, K, V>::insert(K key, V value)
{
    if (this->root_.insert(key, value))
    {
        length_++;
        return true;
    }

    return false;
}

BTREE_TEMPLATE std::optional<V> BTree<N, K, V>::remove(K key)
{
}

BTREE_TEMPLATE void BTree<N, K, V>::traverse(void (*callback)(K key, V value))
{
    root_.traverse(callback);
}
