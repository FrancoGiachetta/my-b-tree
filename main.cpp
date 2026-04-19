#include <libbtree/btree.hpp>
#include <print>
#include <string>

using namespace btree;

int main()
{
    BTree bt = BTree<4, int, std::string>();

    bt.insert(1, "Hola");
    bt.insert(2, "como");
    bt.insert(3, "te");
    bt.insert(4, "va");
    bt.insert(5, "a");
    bt.insert(6, "vos");

    bt.traverse([](int key, std::string value) { std::print("KEY: {}, VALUE: {}\n", key, value); });

    return 0;
}
