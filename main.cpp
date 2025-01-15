#include <iostream>
#include <queue>

#include "BPlusTree.h"

int main() {
    auto *tree = new BPlusTree(3);

    clock_t start_time = clock();

    for (int i = 0; i < 1000; ++i) {
        int r = rand() % 1000;
        tree->insert(r, r);
    }

    // std::cout << tree->lookup(0) << "\n";
    // std::cout << tree->lookup(100) << "\n";
    // std::cout << tree->lookup(1000) << "\n";

    double elapsed = (double) (clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << tree->toString() << "\n";

    std::cout << "__________________________________\n";
    std::cout << "Height: " << tree->height() << "\n";
    std::cout << "Benchmark: " << elapsed << "s";
}
