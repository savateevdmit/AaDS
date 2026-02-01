#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>
#include <algorithm>

int main() {
    double x1;
    x1 = 0.0;
    double y1;
    y1 = 0.0;
    double r1;
    r1 = 0.0;

    double x2;
    x2 = 0.0;
    double y2;
    y2 = 0.0;
    double r2;
    r2 = 0.0;

    double x3;
    x3 = 0.0;
    double y3;
    y3 = 0.0;
    double r3;
    r3 = 0.0;

    std::cin >> x1 >> y1 >> r1;
    std::cin >> x2 >> y2 >> r2;
    std::cin >> x3 >> y3 >> r3;

    double left1;
    left1 = x1 - r1;
    double right1;
    right1 = x1 + r1;
    double bottom1;
    bottom1 = y1 - r1;
    double top1;
    top1 = y1 + r1;

    double left2;
    left2 = x2 - r2;
    double right2;
    right2 = x2 + r2;
    double bottom2;
    bottom2 = y2 - r2;
    double top2;
    top2 = y2 + r2;

    double left3;
    left3 = x3 - r3;
    double right3;
    right3 = x3 + r3;
    double bottom3;
    bottom3 = y3 - r3;
    double top3;
    top3 = y3 + r3;

    double min_x;
    min_x = std::max(std::max(left1, left2), left3);
    double max_x;
    max_x = std::min(std::min(right1, right2), right3);

    double min_y;
    min_y = std::max(std::max(bottom1, bottom2), bottom3);
    double max_y;
    max_y = std::min(std::min(top1, top2), top3);

    if (min_x >= max_x || min_y >= max_y) {
        std::cout << std::setprecision(15) << 0.0 << std::endl;
        return 0;
    }

    double width;
    width = max_x - min_x;
    double height;
    height = max_y - min_y;
    double area_rect;
    area_rect = width * height;

    const int N = 60000;

    std::mt19937 gen;
    gen = std::mt19937(12345);

    std::uniform_real_distribution<double> dist;
    dist = std::uniform_real_distribution<double>(0.0, 1.0);

    long long count;
    count = 0;

    double r1_sq;
    r1_sq = r1 * r1;
    double r2_sq;
    r2_sq = r2 * r2;
    double r3_sq;
    r3_sq = r3 * r3;

    for (int i = 0; i < N; ++i) {
        double u;
        u = dist(gen);
        double v;
        v = dist(gen);

        double x;
        x = min_x + u * width;
        double y;
        y = min_y + v * height;

        double dx1;
        dx1 = x - x1;
        double dy1;
        dy1 = y - y1;
        double dist1_sq;
        dist1_sq = dx1 * dx1 + dy1 * dy1;

        double dx2;
        dx2 = x - x2;
        double dy2;
        dy2 = y - y2;
        double dist2_sq;
        dist2_sq = dx2 * dx2 + dy2 * dy2;

        double dx3;
        dx3 = x - x3;
        double dy3;
        dy3 = y - y3;
        double dist3_sq;
        dist3_sq = dx3 * dx3 + dy3 * dy3;

        bool inside1;
        inside1 = (dist1_sq <= r1_sq);
        bool inside2;
        inside2 = (dist2_sq <= r2_sq);
        bool inside3;
        inside3 = (dist3_sq <= r3_sq);

        if (inside1 && inside2 && inside3) {
            count = count + 1;
        }
    }

    double fraction;
    fraction = static_cast<double>(count) / static_cast<double>(N);
    double area_estimate;
    area_estimate = fraction * area_rect;

    std::cout << std::setprecision(15) << area_estimate << std::endl;

    return 0;
}
