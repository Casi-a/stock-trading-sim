#include "../../include/core/Account.h"
#include "../../include/core/Market.h"       // Market::findSymbol 사용을 위해 필요
#include "../../include/model/Instrument.h"   // Instrument::getPrice 등 사용을 위해 필요
#include <iostream>
using namespace std;

// 생성자: 초기 현금과 자본금 설정
Account::Account(double startCash) 
    : cashBalance(startCash), initialEquity(startCash) {
}

// 소멸자: 동적 할당된 Position 객체들의 메모리 해제
Account::~Account() {
    for (Position* pos : positions) {
        delete pos;
    }
    positions.clear();
}

// 매수 로직
bool Account::buy(string symbol, int quantity, double price, Market* market) {
    // 0. 유효성 검사
    if (quantity <= 0 || price <= 0) return false;

    double totalCost = price * quantity;

    // 1. 잔액 확인
    if (cashBalance < totalCost) {
        // 잔액 부족
        return false;
    }

    // 2. 포지션 검색 및 처리
    Position* pos = findPosition(symbol);

    if (pos != nullptr) {
        // 이미 보유 중인 종목이면 수량 추가 (물타기 평단가 자동 계산됨)
        pos->addQuantity(quantity, price);
    } else {
        // 보유하지 않은 종목이면 Market에서 Instrument 포인터를 찾아 새로 생성
        Instrument* inst = market->findSymbol(symbol); 
        if (inst == nullptr) {
            // 시장에 없는 종목 (에러)
            return false;
        }
        
        // 새로운 포지션 객체 생성 (Heap 할당)
        Position* newPos = new Position(inst, quantity, price);
        positions.push_back(newPos);
    }

    // 3. 현금 차감
    cashBalance -= totalCost;

    return true; // 매수 성공
}

// 매도 로직
bool Account::sell(string symbol, int quantity, double price) {
    // 0. 유효성 검사
    if (quantity <= 0 || price <= 0) return false;

    // 1. 포지션 검색
    Position* pos = findPosition(symbol);
    
    // 보유하지 않은 종목이거나, 보유 수량보다 매도 수량이 많으면 실패
    if (pos == nullptr || pos->getQuantity() < quantity) {
        return false;
    }

    // 2. 현금 증가 (먼저 계산)
    double totalSaleAmount = price * quantity;
    cashBalance += totalSaleAmount;

    // 3. 수량 차감 및 포지션 정리
    pos->removeQuantity(quantity);

    // 수량이 0이 되면 목록에서 제거하고 메모리 해제
    if (pos->getQuantity() == 0) {
        // 벡터에서 해당 포지션을 찾아서 삭제
        for (auto it = positions.begin(); it != positions.end(); it++) {
            if (*it == pos) {
                positions.erase(it);
                delete pos; // 메모리 누수 방지
                break;
            }
        }
    }

    return true; // 매도 성공
}

// 총 평가 자산 계산 (현금 + 보유 주식 평가액)
double Account::getTotalEquity() const {
    double equity = cashBalance;

    for (const Position* pos : positions) {
        // Instrument의 현재가(getPrice) * 보유 수량
        double marketValue = pos->getInstrument()->getPrice() * pos->getQuantity();
        equity += marketValue;
    }

    return equity;
}

// 특정 종목의 포지션 객체 찾기
Position* Account::findPosition(string symbol) {
    for (Position* pos : positions) {
        if (pos->getInstrument()->getSymbol() == symbol) {
            return pos;
        }
    }
    return nullptr; // 못 찾으면 nullptr 반환
}