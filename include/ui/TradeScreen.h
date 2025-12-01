#pragma once
#include "Screen.h"
#include <string>
#include <limits>

// 매수/매도 화면
class TradeScreen : public Screen {
private:
    bool isBuy; // true: 매수, false: 매도

    // 헬퍼 함수: 유효한 숫자 입력 받기
    template <typename T>
    T getValidInput(const std::string& prompt, T minVal, T maxVal = std::numeric_limits<T>::max());

    // 헬퍼 함수: 심볼 입력 받기
    std::string getValidSymbolInput(const std::string& prompt);

public:
    // 생성자: 매수(true) 또는 매도(false) 화면으로 초기화
    TradeScreen(Application* app, bool buyOrSell);

    // 화면 출력 및 주문 실행 로직
    void show() override;
};
