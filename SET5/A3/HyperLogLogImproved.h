#ifndef HYPERLOGLOGCOMPACT_H
#define HYPERLOGLOGCOMPACT_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include "HashFuncGen.h"

class HyperLogLogImproved {
    int B;
    int m;
    std::vector<uint8_t> packed_data;
    HashFuncGen hasher;

    static const int BITS_PER_REGISTER = 5;  // 5 бит на регист

    uint8_t getRegister(int index) const {
        int bit_offset = index * BITS_PER_REGISTER;
        int byte_index = bit_offset / 8;
        int bit_in_byte = bit_offset % 8;

        uint16_t two_bytes = 0;
        two_bytes = packed_data[byte_index];
        if (byte_index + 1 < packed_data.size()) {
            two_bytes |= (static_cast<uint16_t>(packed_data[byte_index + 1]) << 8);
        }

        uint8_t value = (two_bytes >> bit_in_byte) & 0x1F;
        return value;
    }

    // установить значение регистра
    void setRegister(int index, uint8_t value) {
        value &= 0x1F;

        int bit_offset = index * BITS_PER_REGISTER;
        int byte_index = bit_offset / 8;
        int bit_in_byte = bit_offset % 8;

        // читаем 2 байта
        uint16_t two_bytes = 0;
        two_bytes = packed_data[byte_index];
        if (byte_index + 1 < packed_data.size()) {
            two_bytes |= (static_cast<uint16_t>(packed_data[byte_index + 1]) << 8);
        }

        // очищаем старое значение и записываем новое
        uint16_t mask = 0x1F << bit_in_byte;
        two_bytes &= ~mask;
        two_bytes |= (static_cast<uint16_t>(value) << bit_in_byte);

        // записываем обратно
        packed_data[byte_index] = static_cast<uint8_t>(two_bytes & 0xFF);
        if (byte_index + 1 < packed_data.size()) {
            packed_data[byte_index + 1] = static_cast<uint8_t>((two_bytes >> 8) & 0xFF);
        }
    }

    // подсчёт ведущих нулей
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
        if (count > 31) {
            count = 31;
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
    explicit HyperLogLogImproved(int b = 12, uint32_t seed = 0x12345678)
        : B(b), m(1 << b), hasher(seed) {
        // вычисляем нужное количество байт для упаковки
        int total_bits = m * BITS_PER_REGISTER;
        int total_bytes = (total_bits + 7) / 8;
        packed_data.resize(total_bytes, 0);
    }

    // добавить элемент
    void add(const std::string& item) {
        uint32_t hash_value = hasher.hashFNV(item);

        int index = (hash_value >> (32 - B)) & ((1 << B) - 1);
        uint32_t remaining = hash_value << B;

        int zeros = leadingZeros(remaining, 32 - B);

        uint8_t current = getRegister(index);
        if (zeros > current) {
            setRegister(index, static_cast<uint8_t>(zeros));
        }
    }

    // оценка
    double estimate() const {
        double sum = 0.0;
        int zero_count = 0;

        for (int i = 0; i < m; i++) {
            uint8_t reg_value = getRegister(i);
            sum += 1.0 / (1u << reg_value);
            if (reg_value == 0) {
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
        std::fill(packed_data.begin(), packed_data.end(), 0);
    }

    // получить размер в байтах
    size_t getMemoryUsage() const {
        return packed_data.size();
    }
};

#endif
