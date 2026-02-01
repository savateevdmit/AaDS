#include "RandomStreamGen.h"
#include "HashFuncGen.h"
#include <iostream>
#include <unordered_map>

void testHashUniformity() {
    RandomStreamGen gen(10000);
    HashFuncGen hasher;

    const int bins = 100;
    std::vector<int> distribution(bins, 0);

    for (const auto& str : gen.getFullStream()) {
        uint32_t h = hasher.hashFNV(str);
        int bin = (h % bins);
        distribution[bin]++;
    }

    for (int i = 0; i < bins; i++) {
        std::cout << "Bin " << i << ": " << distribution[i] << "\n";
    }

    std::cout << "\nExpected per bin: " << 10000/bins << "\n";
}
