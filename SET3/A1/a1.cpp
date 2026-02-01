#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>
#include <fstream>

int main() {
    std::ofstream csv_file;
    csv_file.open("../SET3/A1/results.csv");

    double sqrt5;
    sqrt5 = std::sqrt(5.0);

    double R;
    R = 2.0 + sqrt5 / 2.0;

    std::random_device rd;
    std::mt19937 gen;
    gen = std::mt19937(rd());

    std::uniform_real_distribution<> dis;
    dis = std::uniform_real_distribution(0.0, 1.0);

    double r1_sq;
    r1_sq = 1.0;

    double r2_sq;
    r2_sq = 5.0 / 4.0;

    double r3_sq;
    r3_sq = 5.0 / 4.0;

    double tight_min;
    tight_min = 2.0 - sqrt5 / 2.0;

    double tight_side;
    tight_side = 2.0 - tight_min;

    csv_file << "N,res1,res2\n";

    for (int N = 100; N <= 100000; N += 500) {
        int count1;
        count1 = 0;

        for (int i = 0; i < N; ++i) {
            double x;
            x = dis(gen) * R;

            double y;
            y = dis(gen) * R;

            double dx1;
            dx1 = x - 1.0;

            double dy1;
            dy1 = y - 1.0;

            double dist1_sq;
            dist1_sq = dx1 * dx1 + dy1 * dy1;

            double dx2;
            dx2 = x - 1.5;

            double dy2;
            dy2 = y - 2.0;

            double dist2_sq;
            dist2_sq = dx2 * dx2 + dy2 * dy2;

            double dx3;
            dx3 = x - 2.0;

            double dy3;
            dy3 = y - 1.5;

            double dist3_sq;
            dist3_sq = dx3 * dx3 + dy3 * dy3;

            bool in1;
            in1 = (dist1_sq <= r1_sq);

            bool in2;
            in2 = (dist2_sq <= r2_sq);

            bool in3;
            in3 = (dist3_sq <= r3_sq);

            if (in1 && in2 && in3) {
                count1 = count1 + 1;
            }
        }

        double area_wide_rect;
        area_wide_rect = R * R;

        double est1;
        est1 = (static_cast<double>(count1) * area_wide_rect) / static_cast<double>(N);

        int count2;
        count2 = 0;

        for (int i = 0; i < N; ++i) {
            double x;
            x = tight_min + dis(gen) * tight_side;

            double y;
            y = tight_min + dis(gen) * tight_side;

            double dx1;
            dx1 = x - 1.0;

            double dy1;
            dy1 = y - 1.0;

            double dist1_sq;
            dist1_sq = dx1 * dx1 + dy1 * dy1;

            double dx2;
            dx2 = x - 1.5;

            double dy2;
            dy2 = y - 2.0;

            double dist2_sq;
            dist2_sq = dx2 * dx2 + dy2 * dy2;

            double dx3;
            dx3 = x - 2.0;

            double dy3;
            dy3 = y - 1.5;

            double dist3_sq;
            dist3_sq = dx3 * dx3 + dy3 * dy3;

            bool in1;
            in1 = (dist1_sq <= r1_sq);

            bool in2;
            in2 = (dist2_sq <= r2_sq);

            bool in3;
            in3 = (dist3_sq <= r3_sq);

            if (in1 && in2 && in3) {
                count2 = count2 + 1;
            }
        }

        double area_tight_rect;
        area_tight_rect = tight_side * tight_side;

        double est2;
        est2 = (static_cast<double>(count2) * area_tight_rect) / static_cast<double>(N);

        csv_file << N << ",";
        csv_file << std::setprecision(10) << est1 << ",";
        csv_file << std::setprecision(10) << est2 << "\n";
    }

    csv_file.close();
    return 0;
}