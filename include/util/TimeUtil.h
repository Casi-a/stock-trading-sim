#pragma once
#include <string>

class TimeUtil {
public:
    // 현재 날짜 반환 (YYYY-MM-DD)
    static std::string getToday();
    
    // 현재 시간 반환 (HH:MM:SS)
    static std::string getNow();

    // 시간 흐름 처리 (Application 루프에서 호출)
    static void advanceTime();
};