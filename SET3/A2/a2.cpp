#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

class ArrayGenerator {
    int max_size;
    const int value_range = 6000;
    std::mt19937 rng;

    std::vector<int> random_array;
    std::vector<int> reversed_array;
    std::vector<int> almost_sorted_array;

    void generateRandomArray() {
        random_array.resize(max_size);
        std::uniform_int_distribution<int> dist(0, value_range);
        for (int i = 0; i < max_size; ++i) {
            random_array[i] = dist(rng);
        }
    }

    void generateReversedArray() {
        reversed_array.resize(max_size);
        for (int i = 0; i < max_size; ++i) {
            reversed_array[i] = max_size - i;
        }
    }

    void generateAlmostSortedArray() {
        almost_sorted_array.resize(max_size);
        for (int i = 0; i < max_size; ++i) {
            almost_sorted_array[i] = i;
        }

        std::uniform_int_distribution dist(0, max_size - 1);
        const int swaps = max_size / 100;
        for (int i = 0; i < swaps; ++i) {
            int idx1 = dist(rng);
            int idx2 = dist(rng);
            std::swap(almost_sorted_array[idx1], almost_sorted_array[idx2]);
        }
    };

public:
    explicit ArrayGenerator(int size)
        : max_size(size), rng(std::random_device{}()) {
        generateRandomArray();
        generateReversedArray();
        generateAlmostSortedArray();
    }

    std::vector<int> getRandomSubarray(int size) {
        return std::vector(random_array.begin(), random_array.begin() + size);
    }

    std::vector<int> getReversedSubarray(int size) {
        return std::vector(reversed_array.begin(), reversed_array.begin() + size);
    }

    std::vector<int> getAlmostSortedSubarray(int size) {
        return std::vector(almost_sorted_array.begin(), almost_sorted_array.begin() + size);
    }
};

class SortTester {
public:
    static void insertionSort(std::vector<int> &a, int l, int r) {
        for (int i = l + 1; i < r; ++i) {
            int key = a[i];
            int j = i - 1;
            while (j >= l && a[j] > key) {
                a[j + 1] = a[j];
                --j;
            }
            a[j + 1] = key;
        }
    }

    static void mergeRanges(std::vector<int> &a, int l, int m, int r, std::vector<int> &tmp) {
        int i = l;
        int j = m;
        int k = l;
        while (i < m && j < r) {
            if (a[i] <= a[j]) {
                tmp[k++] = a[i++];
            } else {
                tmp[k++] = a[j++];
            }
        }

        while (i < m) {
            tmp[k++] = a[i++];
        }

        while (j < r) {
            tmp[k++] = a[j++];
        }

        for (int idx = l; idx < r; ++idx) {
            a[idx] = tmp[idx];
        }
    }

    static void mergeSortStdRec(std::vector<int> &a, int l, int r, std::vector<int> &tmp) {
        if (r - l <= 1) {
            return;
        }

        int m = l + (r - l) / 2;
        mergeSortStdRec(a, l, m, tmp);
        mergeSortStdRec(a, m, r, tmp);
        mergeRanges(a, l, m, r, tmp);
    }

    static void mergeSortStandard(std::vector<int> &a) {
        std::vector<int> tmp(a.size());
        mergeSortStdRec(a, 0, static_cast<int>(a.size()), tmp);
    }

    static void mergeSortHybridRec(std::vector<int> &a, int l, int r, std::vector<int> &tmp, int threshold) {
        if (r - l <= threshold) {
            insertionSort(a, l, r);
            return;
        }

        int m = l + (r - l) / 2;
        mergeSortHybridRec(a, l, m, tmp, threshold);
        mergeSortHybridRec(a, m, r, tmp, threshold);
        mergeRanges(a, l, m, r, tmp);
    }

    static void mergeSortHybrid(std::vector<int> &a, int threshold) {
        std::vector<int> tmp(a.size());
        mergeSortHybridRec(a, 0, static_cast<int>(a.size()), tmp, threshold);
    }

    template<typename Func>
    static double measureMedianMs(const std::vector<int> &arr, Func func, int repeats) {
        std::vector<double> times;
        times.reserve(repeats);

        for (int r = 0; r < repeats; ++r) {
            std::vector<int> copy = arr;
            auto start = std::chrono::high_resolution_clock::now();
            func(copy);
            auto end = std::chrono::high_resolution_clock::now();
            auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            double ms = static_cast<double>(us) / 1000.0;
            times.push_back(ms);
        }

        std::sort(times.begin(), times.end());
        if (repeats % 2 == 1) {
            return times[repeats / 2];
        }
        return (times[repeats / 2 - 1] + times[repeats / 2]) / 2.0;
    }
};

int main() {
    const int MAX_SIZE = 100000;
    const int START_SIZE = 500;
    const int STEP = 100;
    const int REPEATS = 5;
    const std::vector thresholds = {5, 10, 20, 30, 50};

    ArrayGenerator gen(MAX_SIZE);

    std::ofstream std_random_file("standard_random.csv");
    std::ofstream std_reversed_file("standard_reversed.csv");
    std::ofstream std_almost_file("standard_almost_sorted.csv");

    std_random_file << "size,ms\n";
    std_reversed_file << "size,ms\n";
    std_almost_file << "size,ms\n";

    std::ofstream hyb_random_file("hybrid_random.csv");
    std::ofstream hyb_reversed_file("hybrid_reversed.csv");
    std::ofstream hyb_almost_file("hybrid_almost_sorted.csv");

    hyb_random_file << "size,t5,t10,t20,t30,t50\n";
    hyb_reversed_file << "size,t5,t10,t20,t30,t50\n";
    hyb_almost_file << "size,t5,t10,t20,t30,t50\n";

    for (int n = START_SIZE; n <= MAX_SIZE; n += STEP) {
        std::cout << "size = " << n << std::endl;

        std::vector<int> arr_random = gen.getRandomSubarray(n);
        std::vector<int> arr_reversed = gen.getReversedSubarray(n);
        std::vector<int> arr_almost = gen.getAlmostSortedSubarray(n);

        // --- STANDARD MERGE SORT ---
        double t_std_random = SortTester::measureMedianMs(arr_random, [](std::vector<int> &v) {
            SortTester::mergeSortStandard(v);
        }, REPEATS);

        double t_std_reversed = SortTester::measureMedianMs(arr_reversed, [](std::vector<int> &v) {
            SortTester::mergeSortStandard(v);
        }, REPEATS);

        double t_std_almost = SortTester::measureMedianMs(arr_almost, [](std::vector<int> &v) {
            SortTester::mergeSortStandard(v);
        }, REPEATS);

        std_random_file << n << "," << std::fixed << std::setprecision(6) << t_std_random << "\n";
        std_reversed_file << n << "," << std::fixed << std::setprecision(6) << t_std_reversed << "\n";
        std_almost_file << n << "," << std::fixed << std::setprecision(6) << t_std_almost << "\n";

        // --- HYBRID MERGE+INSERTION SORT ---
        std::vector<double> hyb_random_times;
        std::vector<double> hyb_reversed_times;
        std::vector<double> hyb_almost_times;

        hyb_random_times.reserve(thresholds.size());
        hyb_reversed_times.reserve(thresholds.size());
        hyb_almost_times.reserve(thresholds.size());

        for (int thr: thresholds) {
            double t_hyb_random = SortTester::measureMedianMs(arr_random, [thr](std::vector<int> &v) {
                SortTester::mergeSortHybrid(v, thr);
            }, REPEATS);

            double t_hyb_reversed = SortTester::measureMedianMs(arr_reversed, [thr](std::vector<int> &v) {
                SortTester::mergeSortHybrid(v, thr);
            }, REPEATS);

            double t_hyb_almost = SortTester::measureMedianMs(arr_almost, [thr](std::vector<int> &v) {
                SortTester::mergeSortHybrid(v, thr);
            }, REPEATS);

            hyb_random_times.push_back(t_hyb_random);
            hyb_reversed_times.push_back(t_hyb_reversed);
            hyb_almost_times.push_back(t_hyb_almost);
        }

        hyb_random_file << n;
        for (double val: hyb_random_times) {
            hyb_random_file << "," << std::fixed << std::setprecision(6) << val;
        }
        hyb_random_file << "\n";

        hyb_reversed_file << n;
        for (double val: hyb_reversed_times) {
            hyb_reversed_file << "," << std::fixed << std::setprecision(6) << val;
        }
        hyb_reversed_file << "\n";

        hyb_almost_file << n;
        for (double val: hyb_almost_times) {
            hyb_almost_file << "," << std::fixed << std::setprecision(6) << val;
        }
        hyb_almost_file << "\n";
    }

    std_random_file.close();
    std_reversed_file.close();
    std_almost_file.close();

    hyb_random_file.close();
    hyb_reversed_file.close();
    hyb_almost_file.close();

    std::cout << "done" << std::endl;
    return 0;
}
