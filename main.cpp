#include <iostream>
#include <queue>

#include "BPlusTree.h"

int main() {
    auto *tree = new BPlusTree(3);

    clock_t start_time = clock();

    for (int i = 0; i < 1000; ++i) {
        int r = std::rand() % 1000;
        tree->insert(r, r);
    }

    std::cout << tree->lookup(0) << "\n";
    std::cout << tree->lookup(100) << "\n";
    std::cout << tree->lookup(1000) << "\n";


    // larger test
    // for (int i = 0; i < 1'000'000; ++i) {
    //     int r = std::rand() % 1'000'000;
    //     tree->insert(r, r);
    // }
    // for (int i = 0; i < 100; ++i) {
    //     std::cout << tree->lookup(std::rand() % 100) << std::endl;
    // }

    double elapsed = (double) (clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << tree->toString() << std::endl;

    std::cout << "__________________________________" << std::endl;
    std::cout << "Height: " << tree->height() << std::endl;
    std::cout << "Benchmark: " << elapsed << "s";
}
