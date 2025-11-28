#pragma once
#include "Instrument.h"

/**
 * Class: Position
 * 계좌가 보유하고 있는 특정 종목의 상태(수량, 평단가)를 나타냅니다.
 */
class Position {
private:
    Instrument* instrument; // 어떤 종목인지 포인터로 참조
    int quantity;           // 보유 수량
    double avgPrice;        // 평균 매입 단가

public:
    Position(Instrument* inst, int qty, double price)
        : instrument(inst), quantity(qty), avgPrice(price) {}

    Instrument* getInstrument() const { return instrument; }
    int getQuantity() const { return quantity; }
    double getAvgPrice() const { return avgPrice; }

    // 추가 매수 시 수량 증가 및 평단가 재계산
    void addQuantity(int qty, double currentPrice);

    // 매도 시 수량 감소
    void removeQuantity(int qty);
};