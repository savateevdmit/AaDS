// main_compact.cpp
#include "RandomStreamGen.h"
#include "HyperLogLog.h"
#include "HyperLogLogImproved.h"
#include "ExactCounter.h"
#include <iostream>
#include <fstream>
#include <vector>

int main() {
    const int B = 12;
    const int num_streams = 50;
    const size_t stream_size = 100000;

    std::vector<double> steps = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50,
                                   55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

    // Результаты для стандартного и улучшенного
    std::vector<std::vector<double>> standard_estimates(steps.size());
    std::vector<std::vector<double>> compact_estimates(steps.size());
    std::vector<double> exact_values(steps.size(), 0);

    for (int stream_id = 0; stream_id < num_streams; stream_id++) {
        if (stream_id % 10 == 0) {
            std::cout << "stream " << stream_id << "/" << num_streams << "\n";
        }

        RandomStreamGen gen(stream_size, 42 + stream_id);

        for (size_t step_idx = 0; step_idx < steps.size(); step_idx++) {
            double percent = steps[step_idx];
            auto stream_part = gen.getStreamPart(percent);

            // стандартный HyperLogLog
            HyperLogLog hll_standard(B);
            for (const auto& item : stream_part) {
                hll_standard.add(item);
            }
            double estimate_std = hll_standard.estimate();
            standard_estimates[step_idx].push_back(estimate_std);

            // улучшенный HyperLogLog
            HyperLogLogImproved hll_compact(B);
            for (const auto& item : stream_part) {
                hll_compact.add(item);
            }
            double estimate_cmp = hll_compact.estimate();
            compact_estimates[step_idx].push_back(estimate_cmp);

            if (stream_id == 0) {
                ExactCounter exact;
                for (const auto& item : stream_part) {
                    exact.add(item);
                }
                exact_values[step_idx] = exact.count();
            }
        }
    }

    // анализ памяти
    HyperLogLog hll_std(B);
    HyperLogLogImproved hll_cmp(B);

    size_t memory_std = sizeof(uint8_t) * (1 << B);
    size_t memory_cmp = hll_cmp.getMemoryUsage();

    std::cout << "Standard HyperLogLog: " << memory_std << " bytes\n";
    std::cout << "Improved HyperLogLog: " << memory_cmp << " bytes\n";
    std::cout << "Savings: " << (memory_std - memory_cmp) << " bytes ";
    std::cout << "(" << (100.0 * (memory_std - memory_cmp) / memory_std) << "%)\n";

    // Сохранение результатов
    std::ofstream out("results_improved.csv");
    out << "step,percent,exact,mean_std,std_dev_std,mean_compact,std_dev_compact\n";

    for (size_t step_idx = 0; step_idx < steps.size(); step_idx++) {
        double percent = steps[step_idx];
        double exact = exact_values[step_idx];

        double sum_std = 0;
        for (double val : standard_estimates[step_idx]) {
            sum_std += val;
        }
        double mean_std = sum_std / num_streams;

        double var_sum_std = 0;
        for (double val : standard_estimates[step_idx]) {
            var_sum_std += (val - mean_std) * (val - mean_std);
        }
        double std_dev_std = std::sqrt(var_sum_std / num_streams);

        double sum_cmp = 0;
        for (double val : compact_estimates[step_idx]) {
            sum_cmp += val;
        }
        double mean_cmp = sum_cmp / num_streams;

        double var_sum_cmp = 0;
        for (double val : compact_estimates[step_idx]) {
            var_sum_cmp += (val - mean_cmp) * (val - mean_cmp);
        }
        double std_dev_cmp = std::sqrt(var_sum_cmp / num_streams);

        out << step_idx << ","
            << percent << ","
            << exact << ","
            << mean_std << ","
            << std_dev_std << ","
            << mean_cmp << ","
            << std_dev_cmp << "\n";
    }

    out.close();
    std::cout << "\nResults saved to results_compact.csv\n";

    return 0;
}
