#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <string>

class Node;

class BPlusTree {
public:
    int k; // k : minimum of keys in node | 2k : maximum of keys in node
    Node *root;

    explicit BPlusTree(int k);

    ~BPlusTree();

    // Inserts a key-value pair into the tree. Updates existing entries.
    void insert(int key, int val);

    // Checks if the given key-value pair is stored in the tree
    [[nodiscard]] bool lookup(int key, int val) const;

    // Retrieves the corresponding value of the given key. Returns INT_MIN, if the key is not stored in the tree.
    [[nodiscard]] int lookup(int key) const;

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] int height() const;

    [[nodiscard]] Node *findLeaf(int key) const;
};

#endif //BPLUSTREE_H
