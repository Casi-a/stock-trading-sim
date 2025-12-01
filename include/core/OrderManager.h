#pragma once
#include "Order.h"
#include <vector>
#include <string>

// 전방 선언
class Account;
class Market;

// 주문 관리 클래스
class OrderManager {
private:
    std::vector<Order> orderHistory;  // 주문 내역

public:
    // 생성자
    OrderManager() = default;

    // 주문 실행 (isBuy: true=매수, false=매도)
    bool executeOrder(Account* acc, Market* mkt, std::string symbol, int qty, bool isBuy);

    // 주문 내역 조회
    const std::vector<Order>& getOrderHistory() const;
};
