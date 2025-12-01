#pragma once
#include <string>

// 시간 및 날짜 관련 유틸리티 클래스
class TimeUtil {
public:
    // 현재 시스템 시간 반환 (HH:MM:SS)
    static std::string now();

    // 오늘 날짜 반환 (YY-MM-DD)
    static std::string todayDate();
};
