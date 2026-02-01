#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "HashFuncGen.h"

class HyperLogLog {
    int B;
    int m;
    std::vector<uint8_t> registers;
    HashFuncGen hasher;

    int leadingZeros(uint32_t value, int max_bits) const {
        if (value == 0) {
            return max_bits + 1;
        }

        int count = 1;
        for (int i = max_bits - 1; i >= 0; i--) {
            uint32_t bit = (value >> i) & 1u;
            if (bit == 1) {
                break;
            }
            count++;
        }

        if (count > max_bits + 1) {
            count = max_bits + 1;
        }

        return count;
    }

    double getAlpha() const {
        if (m == 16) {
            return 0.673;
        }
        if (m == 32) {
            return 0.697;
        }
        if (m == 64) {
            return 0.709;
        }
        return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
    }

public:
    explicit HyperLogLog(int b = 10, uint32_t seed = 0x12345678)
        : B(b), m(1 << b), hasher(seed) {
        registers.resize(m, 0);
    }

    void add(const std::string& item) {
        uint32_t hash_value = hasher.hashFNV(item);

        int index = (hash_value >> (32 - B)) & ((1 << B) - 1);

        uint32_t remaining = hash_value << B;

        int zeros = leadingZeros(remaining, 32 - B);

        if (zeros > registers[index]) {
            registers[index] = static_cast<uint8_t>(zeros);
        }
    }

    double estimate() const {
        double sum = 0.0;
        int zero_count = 0;

        for (int i = 0; i < m; i++) {
            sum += 1.0 / (1u << registers[i]);
            if (registers[i] == 0) {
                zero_count++;
            }
        }

        double alpha = getAlpha();
        double est = alpha * m * m / sum;

        if (est <= 2.5 * m && zero_count > 0) {
            est = m * std::log(static_cast<double>(m) / zero_count);
        }

        double threshold = (1.0/30.0) * 4294967296.0;
        if (est > threshold) {
            est = -4294967296.0 * std::log(1.0 - est / 4294967296.0);
        }

        return est;
    }

    void reset() {
        std::fill(registers.begin(), registers.end(), 0);
    }
};

#endif
