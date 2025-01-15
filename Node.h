#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class BPlusTree;


class Node {
public:
    explicit Node(bool isLeaf = false);

    ~Node();

    bool isLeaf;

    std::vector<int> keys;
    Node *parent;

    // for inner nodes
    std::vector<Node *> children;

    // for leaf nodes
    std::vector<int> vals;
    Node *next;

    void addChild(Node *child);

    void insertPromotedKey(int key, Node *leftChild, Node *rightChild, BPlusTree *tree);

    void insertKey(int key, BPlusTree *tree);

    void insertVal(int val, BPlusTree *tree);

    void split(BPlusTree *tree);

    std::string toString();
};

#endif //NODE_H
