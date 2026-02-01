#ifndef RANDOMSTREAMGEN_H
#define RANDOMSTREAMGEN_H

#include <string>
#include <vector>
#include <random>


class RandomStreamGen {
    std::mt19937 rng;
    std::vector<std::string> stream;

    static const char allowed_chars[];
    static const int num_chars = 63; // 26 + 26 + 10 + 1

public:
    RandomStreamGen(size_t stream_size, unsigned int seed = 42) : rng(seed) {
        generateStream(stream_size);
    }

    // генерация потока
    void generateStream(size_t size) {
        stream.clear();
        stream.reserve(size);
        for (size_t i = 0; i < size; i++) {
            stream.push_back(generateRandomString());
        }
    }

    // генерация одной случайной строки
    std::string generateRandomString() {
        int len = (rng() % 30) + 1;

        std::string result;
        result.reserve(len);

        for (int i = 0; i < len; i++) {
            int index = rng() % num_chars;
            result += allowed_chars[index];
        }

        return result;
    }

    std::vector<std::string> getStreamPart(double percent) {
        if (percent <= 0) {
            return {};
        }
        if (percent >= 100) {
            return stream;
        }

        auto size = static_cast<size_t>(stream.size() * percent / 100.0);
        if (size > stream.size()) {
            size = stream.size();
        }

        return std::vector(stream.begin(), stream.begin() + size);
    }

    // получить весь поток
    const std::vector<std::string>& getFullStream() const {
        return stream;
    }

    size_t getStreamSize() const {
        return stream.size();
    }
};

const char RandomStreamGen::allowed_chars[] =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "-";

#endif
