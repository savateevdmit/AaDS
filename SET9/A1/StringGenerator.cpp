#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

class StringGenerator {
    std::string alphabet;

    static std::string escapeJson(const std::string &s) {
        std::string t;
        for (char c: s) {
            if (c == '\\') {
                t += "\\\\";
            } else if (c == '\"') {
                t += "\\\"";
            } else if (c == '\n') {
                t += "\\n";
            } else if (c == '\r') {
                t += "\\r";
            } else if (c == '\t') {
                t += "\\t";
            } else {
                t += c;
            }
        }
        return t;
    }

    static void writeStringArray(std::ofstream &out, const std::vector<std::string> &a) {
        out << "[\n";
        for (int i = 0; i < static_cast<int>(a.size()); ++i) {
            out << "    \"" << escapeJson(a[i]) << "\"";
            if (i + 1 < static_cast<int>(a.size())) {
                out << ",";
            }
            out << "\n";
        }
        out << "]";
    }

public:
    StringGenerator() {
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";
        std::srand(std::time(nullptr));
    }

    std::string generateRandomString(int len) {
        std::string s = "";
        for (int i = 0; i < len; ++i) {
            s += alphabet[std::rand() % alphabet.length()];
        }
        return s;
    }

    std::vector<std::string> generateRandomArray(int size) {
        std::vector<std::string> arr;
        for (int i = 0; i < size; ++i) {
            int len = 10 + std::rand() % 191;
            arr.push_back(generateRandomString(len));
        }
        return arr;
    }

    std::vector<std::string> generateReverseSortedArray(int size) {
        std::vector<std::string> arr = generateRandomArray(size);
        std::sort(arr.begin(), arr.end(), std::greater<std::string>());
        return arr;
    }

    std::vector<std::string> generateNearlySortedArray(int size) {
        std::vector<std::string> arr = generateRandomArray(size);
        std::sort(arr.begin(), arr.end());

        const int swaps = size * 0.05;
        for (int i = 0; i < swaps; ++i) {
            int idx1 = std::rand() % size;
            int idx2 = std::rand() % size;
            std::swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }

    std::vector<std::string> generatePrefixArray(int size, const std::string &prefix) {
        std::vector<std::string> arr;
        for (int i = 0; i < size; ++i) {
            int remaining = (10 + std::rand() % 191) - prefix.length();
            if (remaining < 0) {
                remaining = 0;
            }
            arr.push_back(prefix + generateRandomString(remaining));
        }

        for (int i = 0; i < size; ++i) {
            int idx1 = std::rand() % size;
            int idx2 = std::rand() % size;
            std::swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }

    void writeAllArraysToJson(const std::string &fileName) {
        std::vector<std::string> a1 = generateRandomArray(3000);
        std::vector<std::string> a2 = generateReverseSortedArray(3000);
        std::vector<std::string> a3 = generateNearlySortedArray(3000);
        std::vector<std::string> a4 = generatePrefixArray(3000, "hello_hse");

        std::vector<std::string> names;
        names.push_back("random");
        names.push_back("reverse");
        names.push_back("almost_sorted");
        names.push_back("common_prefix");

        std::vector<std::vector<std::string> > all;
        all.push_back(a1);
        all.push_back(a2);
        all.push_back(a3);
        all.push_back(a4);

        std::ofstream out(fileName.c_str());
        if (!out.is_open()) {
            std::cout << "cannot open file\n";
            return;
        }

        out << "{\n";
        for (int i = 0; i < 4; ++i) {
            out << "  \"" << names[i] << "\": ";
            writeStringArray(out, all[i]);
            if (i + 1 < 4) {
                out << ",";
            }
            out << "\n";
        }
        out << "}\n";
        out.close();
        std::cout << "written to " << fileName << "\n";
    }
};

int main() {
    StringGenerator gen;
    gen.writeAllArraysToJson("data/arrays.json");
    return 0;
}
