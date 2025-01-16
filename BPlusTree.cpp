#include "BPlusTree.h"
#include "Node.h"

#include <algorithm>
#include <climits>
#include <queue>
#include <sstream>
#include <string>

BPlusTree::BPlusTree(const int k)
    : k(k), root(nullptr) {
}

BPlusTree::~BPlusTree() {
    delete root;
}

void BPlusTree::insert(const int key, const int val) {
    if (root == nullptr) {
        // if no root is present, create new one as leaf with the inserted data
        root = new Node(true);
        root->keys.push_back(key);
        root->vals.push_back(val);
    } else {
        // find the correct leaf node using the given key
        Node *leaf = findLeaf(key);
        if (!leaf) {
            // only really happens if there is no root node or the tree is invalid
            return;
        }

        // find correct insertion position using binary search
        auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        int i = static_cast<int>(it - leaf->keys.begin());

        if (it < leaf->keys.end() && *it == key) {
            // key exists, update value
            leaf->vals[i] = val;
        } else {
            leaf->keys.insert(it, key);
            leaf->vals.insert(leaf->vals.begin() + i, val);

            // split if necessary
            if (leaf->keys.size() > 2 * k) {
                leaf->split(this);
            }
        }
    }
}

bool BPlusTree::lookup(const int key, int val) const {
    return lookup(key) != INT_MIN;
}

int BPlusTree::lookup(const int key) const {
    const Node *leaf = findLeaf(key);
    if (leaf == nullptr) return INT_MIN; // this should never happen if root is set

    for (int i = 0; i < leaf->keys.size(); ++i) {
        // The key is not contained in the vector, abort search with error value
        if (leaf->keys[i] > key) return INT_MIN;
        // return the value corresponding to the key
        if (leaf->keys[i] == key) return leaf->vals[i];
    }
    return INT_MIN;
}

std::string BPlusTree::toString() const {
    if (root == nullptr) return "X";

    std::ostringstream result;
    std::queue<Node *> queue;
    int level = 0;

    queue.push(root);

    while (!queue.empty()) {
        result << "Level " << level++ << ":\t";

        const size_t tmp_size = queue.size();
        for (int i = 0; i < tmp_size; ++i) {
            Node *curr = queue.front();
            queue.pop();
            result << curr->toString() << " ";

            if (!curr->isLeaf) {
                for (auto child: curr->children) {
                    queue.push(child);
                }
            }
        }
        result << "\n";
    }

    return result.str();
}

int BPlusTree::height() const {
    int height = 1;
    const Node *curr = root;

    while (!curr->isLeaf) {
        curr = curr->children[0];
        height++;
    }
    return height;
}


Node *BPlusTree::findLeaf(const int key) const {
    if (!root) return nullptr;
    Node *curr = root;
    // Using linear search, since the maximum reasonable k is in the order of 100
    while (!curr->isLeaf) {
        if (key > curr->keys.back()) {
            // shortcut for large insertions
            curr = curr->children.back();
            continue;
        }
        int i = 0;
        while (i < (curr->keys.size()) && key >= curr->keys[i]) {
            ++i;
        }
        curr = curr->children[i];
    }
    return curr;
}
