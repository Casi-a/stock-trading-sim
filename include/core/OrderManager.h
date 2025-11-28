#pragma once
#include <vector>
#include "../model/Order.h"

class Account;
class Market;

/**
 * Class: OrderManager
 * 거래를 중개하고 기록을 남깁니다.
 */
class OrderManager {
private:
    std::vector<Order> orderHistory; // 기록 저장 공간

public:
    /**
     * 주문 실행
     * Market에서 종목 찾기 -> Account의 buy/sell 호출
     * -> 성공 시 orderHistory에 추가
     */
    bool executeOrder(Account* acc, Market* mkt, std::string symbol, int qty, bool isBuy);

    // getter
    const std::vector<Order>& getHistory() const { return orderHistory; }
};