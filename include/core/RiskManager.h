#pragma once
#include <string>

// 전방 선언
class Account;
class Market;
class OrderManager;

// 리스크 관리 클래스 (마진콜, 선물 만기 처리)
class RiskManager {
public:
    // 생성자
    RiskManager() = default;

    // 리스크 체크: 마진콜 및 선물 만기
    void checkRisk(Account* acc, Market* mkt, OrderManager* om);
};
