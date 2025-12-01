#pragma once
#include <string>

// 거래 주문 기록 클래스
class Order {
private:
    std::string symbol;   // 종목 코드
    bool isBuy;           // true: 매수, false: 매도
    int quantity;         // 수량
    double price;         // 체결 가격
    std::string date;     // 거래 일시

public:
    // 생성자
    Order(std::string sym, bool buy, int qty, double p, std::string d);

    // Getter
    const std::string& getSymbol() const;
    bool getIsBuy() const;
    int getQuantity() const;
    double getPrice() const;
    const std::string& getDate() const;
};
