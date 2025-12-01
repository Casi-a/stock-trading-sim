#pragma once
#include "Position.h"
#include "InstrumentType.h"
#include <vector>
#include <string>

// 전방 선언
class Market;

// 사용자 계좌 관리 클래스
class Account {
private:
    double cashBalance;              // 현금 잔고
    double initialEquity;            // 초기 자본금
    std::vector<Position*> positions;  // 보유 포지션 리스트

    // 특정 종목의 포지션 찾기
    Position* findPosition(std::string symbol);

public:
    // 생성자
    Account(double startCash);

    // 소멸자
    ~Account();

    // 매수
    bool buy(std::string symbol, int quantity, double price, Market* market);

    // 매도
    bool sell(std::string symbol, int quantity, double price);

    // 총 평가 자산 계산 (현금 + 보유 주식 평가액)
    double getTotalEquity() const;

    // Getter
    double getCashBalance() const;
    double getInitialEquity() const;
    const std::vector<Position*>& getPositions() const;

    // A팀 명세 누락 메서드 추가
    std::vector<Position*> getPositionsByType(InstrumentType type) const;
};
