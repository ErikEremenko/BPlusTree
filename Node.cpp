#include "Node.h"
#include "BPlusTree.h"

#include <algorithm>
#include <format>
#include <sstream>

Node::Node(bool isLeaf)
    : isLeaf(isLeaf), parent(nullptr), children(), vals(), next(nullptr) {
}

Node::~Node() {
    if (!isLeaf) {
        for (const auto child: children) {
            delete child;
        }
    }
}

void Node::addChild(Node *child) {
    // handle invalid cases
    if (isLeaf || child == nullptr || child->keys.empty()) return;

    if (children.empty()) {
        children.push_back(child);
    } else {
        // insert sorted
        auto i = children.begin();
        while (i != children.end() && (*i)->keys.front() < child->keys.front()) {
            ++i;
        }
        children.insert(i, child);
    }
    child->parent = this;
}

void Node::insertPromotedKey(int key, Node *leftChild, Node *rightChild, BPlusTree *tree) {
    int i = 0;
    while (i < keys.size() && keys[i] < key) {
        ++i;
    }

    keys.insert(keys.begin() + i, key);
    // insert right child at one position after the promoted key's index (i + 1)
    children.insert(children.begin() + i + 1, rightChild);
    rightChild->parent = this;

    // splitting if necessary
    if (keys.size() > 2 * tree->k) {
        split(tree);
    }
}

void Node::split(BPlusTree *tree) {
    // get middle key
    int index = keys.size() / 2; // middle key

    if (isLeaf) {
        auto sibling = new Node(true); // right sibling node
        // the sibling's first key will be promoted into the parent's key vector
        sibling->keys.insert(sibling->keys.end(), keys.begin() + index, keys.end());
        sibling->vals.insert(sibling->vals.end(), vals.begin() + index, vals.end());

        // erase data from original leaf node (avoid duplication)
        this->keys.erase(keys.begin() + index, keys.end());
        this->vals.erase(vals.begin() + index, vals.end());

        sibling->next = next; // establishing linked list structure
        next = sibling;

        if (parent) {
            // add the promoted key (first one in the sibling vector) to parent node
            parent->insertPromotedKey(sibling->keys.front(), this, sibling, tree);
        } else {
            // promote to new root
            auto newRoot = new Node(false);
            tree->root = newRoot;
            // newRoot->insertPromotedKey(sibling->keys.front(), this, sibling, tree);
            // simple procedure as root has no data yet
            newRoot->keys.push_back(sibling->keys.front());
            newRoot->children.push_back(this);
            newRoot->children.push_back(sibling);
            // incorporating new root into tree
            this->parent = sibling->parent = tree->root = newRoot;
        }
    } else {
        // internal node
        const int promotedKey = this->keys.at(index); // saving it from being erased
        const auto sibling = new Node(false);
        sibling->keys.insert(sibling->keys.end(), keys.begin() + index + 1, keys.end());
        sibling->children.insert(sibling->children.end(), children.begin() + index + 1, children.end());

        // include the middle key (at index) as it's promoted
        this->keys.erase(keys.begin() + index, keys.end());
        this->children.erase(children.begin() + index + 1, children.end());

        // bind select child nodes to sibling node
        for (auto child: sibling->children) {
            child->parent = sibling;
        }

        if (parent) {
            // add the promoted key (first one in the sibling vector) to parent node
            parent->insertPromotedKey(promotedKey, this, sibling, tree);
        } else {
            // promote to new root
            auto newRoot = new Node(false);
            tree->root = newRoot;

            newRoot->keys.push_back(promotedKey); // promoting from the middle this time
            newRoot->children.push_back(this);
            newRoot->children.push_back(sibling);
            // incorporating new root into tree
            this->parent = sibling->parent = tree->root = newRoot;
        }
    }
}

std::string Node::toString() {
    std::ostringstream result;
    result << "[|";

    for (auto key: keys) {
        result << key << "|";
    }
    result << "] ";

    if (isLeaf) {
        result << "{ ";
        for (auto val: vals) {
            result << val << " ";
        }
        result << "} --->";
    }
    return result.str();
}

// nice to have but not really used
void Node::insertKey(const int key, BPlusTree *tree) {
    keys.insert(std::ranges::lower_bound(keys, key), key);
}

void Node::insertVal(const int val, BPlusTree *tree) {
    if (!isLeaf) return;
    vals.insert(std::ranges::lower_bound(vals, val), val);
    if (vals.size() > 2 * tree->k) {
        split(tree);
    }
}
