#include <libbtree/btree.hpp>
#include <string>

using namespace btree;

int main()
{
    BTree bt = BTree<5, char, std::string>();

    bt.insert('A', "");
    bt.insert('C', "");
    bt.insert('G', "");
    bt.insert('N', "");
    bt.insert('H', "");
    bt.insert('E', "");
    bt.insert('K', "");
    bt.insert('Q', "");
    bt.insert('M', "");
    bt.insert('F', "");
    bt.insert('W', "");
    bt.insert('L', "");
    bt.insert('T', "");
    bt.insert('Z', "");

    bt.traverse([](char key, std::string value) { std::print("(KEY: {}, VALUE: {}), ", key, value); });

    return 0;
}
