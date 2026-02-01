#include "RandomStreamGen.h"
#include "HyperLogLog.h"
#include "ExactCounter.h"
#include <iostream>
#include <fstream>
#include <vector>

int main() {
    const int B = 12;               // количество бит
    const int num_streams = 50;     // количество потоков
    const size_t stream_size = 100000; // размер каждого потока

    std::vector<double> steps = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50,
                                   55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

    std::vector<std::vector<double>> all_estimates(steps.size());
    std::vector<double> exact_values(steps.size(), 0);

    for (int stream_id = 0; stream_id < num_streams; stream_id++) {
        if (stream_id % 10 == 0) {
            std::cout << "stream " << stream_id << "/" << num_streams << "\n";
        }

        RandomStreamGen gen(stream_size, 42 + stream_id);

        for (size_t step_idx = 0; step_idx < steps.size(); step_idx++) {
            double percent = steps[step_idx];
            auto stream_part = gen.getStreamPart(percent);

            HyperLogLog hll(B);
            for (const auto& item : stream_part) {
                hll.add(item);
            }
            double estimate = hll.estimate();
            all_estimates[step_idx].push_back(estimate);

            if (stream_id == 0) {
                ExactCounter exact;
                for (const auto& item : stream_part) {
                    exact.add(item);
                }
                exact_values[step_idx] = exact.count();
            }
        }
    }

    std::ofstream out("results.csv");
    out << "step,percent,exact,mean_estimate,std_dev,lower_bound,upper_bound\n";

    for (size_t step_idx = 0; step_idx < steps.size(); step_idx++) {
        double percent = steps[step_idx];
        double exact = exact_values[step_idx];

        // среднее
        double sum = 0;
        for (double val : all_estimates[step_idx]) {
            sum += val;
        }
        double mean = sum / num_streams;

        // стандартное отклонение
        double var_sum = 0;
        for (double val : all_estimates[step_idx]) {
            var_sum += (val - mean) * (val - mean);
        }
        double std_dev = std::sqrt(var_sum / num_streams);

        out << step_idx << ","
            << percent << ","
            << exact << ","
            << mean << ","
            << std_dev << ","
            << (mean - std_dev) << ","
            << (mean + std_dev) << "\n";

        std::cout << "Step " << percent << "%: Exact=" << exact
                  << ", Mean=" << mean << ", StdDev=" << std_dev << "\n";
    }

    out.close();
    return 0;
}
