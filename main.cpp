#include <iostream>
#include <queue>

#include "BPlusTree.h"

int main() {
    auto *tree = new BPlusTree(3);

    clock_t start_time = clock();

    // small example
    // for (int i = 1; i <= 10; ++i) {
    //     tree->insert(i, i);
    // }
    //
    // std::cout << tree->lookup(3) << "\n";
    // std::cout << tree->lookup(10) << "\n";
    // std::cout << tree->lookup(0) << "\n";


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
    //     std::cout << tree->lookup(std::rand() % 1000) << std::endl;
    // }


    // stress test
    // set k to a larger value like 32 or 50
    // for (int i = 0; i < 100'000'000; ++i) {
    //     int r = std::rand();
    //     tree->insert(r, r);
    // }

    double elapsed = (double) (clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << tree->toString() << std::endl;

    std::cout << "__________________________________" << std::endl;
    std::cout << "Height: " << tree->height() << std::endl;
    std::cout << "Benchmark: " << elapsed << "s";
}
