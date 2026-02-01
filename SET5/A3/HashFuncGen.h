#ifndef HASHFUNCGEN_H
#define HASHFUNCGEN_H

#include <string>
#include <cstdint>

class HashFuncGen {
    uint32_t seed;

public:
    HashFuncGen(uint32_t seed_value = 0x12345678) : seed(seed_value) {}

    uint32_t hashFNV(const std::string& str) const {
        uint32_t h = 2166136261u ^ seed;
        for (char c : str) {
            h ^= static_cast<uint32_t>(c);
            h *= 16777619u;
        }
        return h;
    }
};

#endif
