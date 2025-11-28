#pragma once
#include <vector>
#include <string>
#include "../model/Position.h"

// 전방 선언
class Market;

/**
 * Class: Account
 * 사용자의 자산과 포트폴리오(Positions)를 관리합니다.
 */
class Account {
private:
    double cashBalance;      // 현금 잔고
    double initialEquity;    // 초기 자본금 (마진콜 기준)
    
    // 보유 포지션 리스트
    std::vector<Position*> positions;

public:
    Account(double startCash);
    ~Account(); // positions 내부 객체 delete 필수

    /**
     * [Logic] 매수
     * 1. 잔액 확인
     * 2. positions 검색 -> 있으면 addQuantity, 
     *    없으면 new Position 생성 후 push_back
     * 3. 현금 차감
     */
    bool buy(std::string symbol, int quantity, double price, Market* market);

    /**
     * [Logic] 매도
     * 1. positions 검색 -> 수량 확인
     * 2. removeQuantity 호출 -> 수량 0 되면 vector에서 erase 및 delete
     * 3. 현금 증가
     */
    bool sell(std::string symbol, int quantity, double price);

    double getBalance() const { return cashBalance; }
    double getInitialEquity() const { return initialEquity; }
    
    // [Logic] 총 평가 자산 (현금 + 모든 포지션의 가치 합산)
    double getTotalEquity() const; 

    // 특정 종목 포지션 찾기
    Position* findPosition(std::string symbol);

    const std::vector<Position*>& getPositions() const { return positions; }
};