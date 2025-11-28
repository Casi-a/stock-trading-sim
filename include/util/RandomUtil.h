#pragma once

class RandomUtil {
public:
    // min ~ max 사이 랜덤 실수 반환
    static double getDouble(double min, double max);
    
    // min ~ max 사이 랜덤 정수 반환
    static int getInt(int min, int max);
};