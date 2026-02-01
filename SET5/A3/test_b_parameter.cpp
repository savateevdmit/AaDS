#include "RandomStreamGen.h"
#include "HyperLogLog.h"
#include "ExactCounter.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

int main() {
    const size_t stream_size = 20000;
    const int num_tests = 20;

    // значения B
    std::vector<int> b_values;
    b_values.push_back(4);
    b_values.push_back(6);
    b_values.push_back(8);
    b_values.push_back(10);
    b_values.push_back(12);
    b_values.push_back(14);
    b_values.push_back(16);
    b_values.push_back(18);
    b_values.push_back(20);
    b_values.push_back(22);

    std::ofstream out("b_parameter_test.csv");
    out << "B,registers,memory_bytes,mean_error_percent,std_error_percent,theoretical_error\n";

    for (int B : b_values) {
        int m = 1 << B;
        double theoretical_error = 1.04 / std::sqrt(static_cast<double>(m)) * 100.0;

        std::vector<double> errors;
        for (int test = 0; test < num_tests; test++) {
            RandomStreamGen gen(stream_size, 100 + test);
            const std::vector<std::string>& stream = gen.getFullStream();

            ExactCounter exact;
            for (const auto & i : stream) {
                exact.add(i);
            }
            auto exact_count = static_cast<double>(exact.count());

            // HyperLogLog
            HyperLogLog hll(B);
            for (const auto & i : stream) {
                hll.add(i);
            }
            double estimate = hll.estimate();

            // относительная ошибка
            double error_percent = std::abs(estimate - exact_count) / exact_count * 100.0;
            errors.push_back(error_percent);
        }

        // средняя ошибка
        double sum = 0;
        for (double error : errors) {
            sum += error;
        }
        double mean_error = sum / errors.size();

        // стандартное отклонение ошибки
        double var_sum = 0;
        for (double error : errors) {
            double diff = error - mean_error;
            var_sum += diff * diff;
        }
        double std_error = std::sqrt(var_sum / errors.size());

        out << B << "," << m << "," << m << ","
            << mean_error << "," << std_error << ","
            << theoretical_error << "\n";

        std::cout << "B=" << B << " (m=" << m << "): "
                  << "Mean Error=" << mean_error << "%, "
                  << "Std=" << std_error << "%, "
                  << "Theoretical=" << theoretical_error << "%\n";
    }

    out.close();
    return 0;
}
