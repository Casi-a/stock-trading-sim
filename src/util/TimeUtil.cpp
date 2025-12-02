#include "TimeUtil.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// 현재 시스템 시간 반환 (HH:MM:SS 형식)
std::string TimeUtil::now() {
    using namespace std::chrono;
    auto tp = system_clock::now();
    std::time_t t = system_clock::to_time_t(tp);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &t);
#else
    local = *std::localtime(&t);
#endif
    char buf[9];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", &local);
    return std::string(buf);
}

// 오늘 날짜 반환 (YY-MM-DD 형식)
std::string TimeUtil::todayDate() {
    using namespace std::chrono;
    auto tp = system_clock::now();
    std::time_t t = system_clock::to_time_t(tp);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &t);
#else
    local = *std::localtime(&t);
#endif
    char buf[11];
    std::strftime(buf, sizeof(buf), "%y-%m-%d", &local);
    return std::string(buf);
}
