#include <concepts>
#include <cstdint>
#include <optional>
#include <tuple>

namespace btree
{

template <std::uint16_t N, std::totally_ordered K, class V> class BTreeNode
{
  public:
    BTreeNode(bool is_leaf) : is_leaf_(is_leaf)
    {
    }

    bool is_leaf() const
    {
        return is_leaf_;
    }

    std::uint16_t length() const
    {
        return length_;
    }

    inline bool should_split() const
    {
        return length_ == N;
    }

    void insert(K key, V value);
    std::optional<V> remove(K key, V value);
    std::optional<V> get(K key);

  private:
    bool is_leaf_;

    std::uint16_t length_;
    std::tuple<K, V> values_[N];
    std::optional<BTreeNode<N, K, V>> parent_;
    BTreeNode<N, K, V> childreen_[N];
};

template <std::uint16_t N, std::totally_ordered K, class V> class Btree
{
  public:
    std::uint16_t order() const
    {
        return order_;
    }

    std::uint16_t length() const
    {
        return length_;
    }

    void insert(K key, V value);
    std::optional<V> remove(K key);
    std::optional<V> get(K key);

  private:
    std::uint16_t length_;
    BTreeNode<N, K, V> root_[N];
    static constexpr std::uint16_t order_ = N;
};
} // namespace btree
