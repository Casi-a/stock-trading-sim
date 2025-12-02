#include "RandomUtil.h"
#include <random>
#include <chrono>

// 지정된 범위 내의 랜덤 실수 반환
double RandomUtil::randomRate(double min, double max) {
    static std::mt19937 rng(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}
