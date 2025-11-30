#pragma once
#include <string>

/**
 * Struct: Order
 * 거래 내역 기록용 구조체입니다.
 */
struct Order {
    std::string symbol;  // 종목 코드
    bool isBuy;          // true: 매수, false: 매도
    int quantity;        // 거래 수량
    double price;        // 체결 가격
    std::string date;    // 체결 일시

    Order(std::string sym, bool buy, int qty, double p, std::string d);
};