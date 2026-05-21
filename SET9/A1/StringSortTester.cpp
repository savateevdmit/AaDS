#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "StandardAlgorithms.h"
#include "SpecialAlgorithms.h"

std::vector<std::string> parseArray(const std::string &json, const std::string &key) {
    std::vector<std::string> result;
    size_t pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) {
        return result;
    }

    pos = json.find("[", pos);
    if (pos == std::string::npos) {
        return result;
    }

    size_t endPos = json.find("]", pos);
    if (endPos == std::string::npos) {
        return result;
    }

    size_t current = pos + 1;
    while (current < endPos) {
        size_t quote1 = json.find("\"", current);
        if (quote1 == std::string::npos || quote1 > endPos) {
            break;
        }

        size_t quote2 = json.find("\"", quote1 + 1);
        if (quote2 == std::string::npos || quote2 > endPos) {
            break;
        }

        result.push_back(json.substr(quote1 + 1, quote2 - quote1 - 1));
        current = quote2 + 1;
    }
    return result;
}

class StringSortTester {
    int tries;

public:
    StringSortTester() {
        tries = 5;
    }

    void runTests(const std::string &inFileName, const std::string &outFileName) const {
        std::ifstream inFile(inFileName.c_str());
        if (!inFile.is_open()) {
            std::cout << "cannot open " << inFileName << "\n";
            return;
        }

        std::string jsonContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        std::vector<std::string> names;
        names.push_back("random");
        names.push_back("reverse");
        names.push_back("almost_sorted");
        names.push_back("common_prefix");

        std::vector<std::vector<std::string> > all;
        for (int i = 0; i < 4; ++i) {
            all.push_back(parseArray(jsonContent, names[i]));
            if (all[i].empty()) {
                std::cout << "error reading array " << names[i] << " from json\n";
                return;
            }
        }

        std::ofstream out(outFileName.c_str());
        if (!out.is_open()) {
            std::cout << "cannot open " << outFileName << "\n";
            return;
        }

        out << "{\n";
        out << "  \"measurements\": [\n";

        bool first = true;

        for (int t = 0; t < 4; ++t) {
            for (int size = 100; size <= 3000; size += 100) {
                if (size > all[t].size()) {
                    break;
                }

                std::vector<std::string> base;
                base.reserve(size);
                for (int i = 0; i < size; ++i) {
                    base.push_back(all[t][i]);
                }

                for (int alg = 0; alg < 6; ++alg) {
                    long long sumTime = 0;
                    long long sumCnt = 0;

                    for (int rep = 0; rep < tries; ++rep) {
                        std::vector<std::string> cur = base;
                        long long cnt = 0;
                        auto st = std::chrono::high_resolution_clock::now();

                        if (alg == 0) {
                            standardQuickSort(cur, 0, (int) cur.size() - 1, cnt);
                        } else if (alg == 1) {
                            standardMergeSort(cur, 0, (int) cur.size() - 1, cnt);
                        } else if (alg == 2) {
                            stringQuickSort(cur, 0, (int) cur.size() - 1, 0, cnt);
                        } else if (alg == 3) {
                            stringMergeSort(cur, cnt);
                        } else if (alg == 4) {
                            msdRadixSort(cur, cnt);
                        } else if (alg == 5) {
                            msdRadixSortWithCutoff(cur, cnt);
                        }

                        auto fn = std::chrono::high_resolution_clock::now();
                        long long tm = std::chrono::duration_cast<std::chrono::microseconds>(fn - st).count();
                        sumTime += tm;
                        sumCnt += cnt;
                    }

                    if (!first) {
                        out << ",\n";
                    }
                    first = false;

                    out << "    {\n";
                    out << "      \"array_type\": \"" << names[t] << "\",\n";
                    out << "      \"size\": " << size << ",\n";

                    std::string algName;
                    if (alg == 0) {
                        algName = "QUICKSORT (Стандартный)";
                    } else if (alg == 1) {
                        algName = "MERGESORT (Стандартный)";
                    } else if (alg == 2) {
                        algName = "STRING QUICKSORT (Тернарный)";
                    } else if (alg == 3) {
                        algName = "STRING MERGESORT";
                    } else if (alg == 4) {
                        algName = "MSD RADIX SORT (Без переключения)";
                    } else if (alg == 5) {
                        algName = "MSD RADIX SORT (С переключением)";
                    }

                    out << "      \"algorithm\": \"" << algName << "\",\n";
                    out << "      \"avg_time_us\": " << (sumTime / tries) << ",\n";
                    out << "      \"avg_char_comparisons\": " << (sumCnt / tries) << "\n";
                    out << "    }";
                }
            }
        }

        out << "\n  ]\n";
        out << "}\n";
        out.close();
        std::cout << "written to " << outFileName << "\n";
    }
};

int main() {
    StringSortTester tester;
    tester.runTests("data/arrays.json", "data/measurements.json");
    return 0;
}
