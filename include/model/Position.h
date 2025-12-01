#pragma once
#include "Instrument.h"

// 특정 종목의 보유 내역을 나타내는 클래스
class Position {
private:
    Instrument* instrument;  // 보유 종목 포인터
    int quantity;            // 보유 수량
    double avgPrice;         // 평균 매입가

public:
    // 생성자
    Position(Instrument* inst, int qty, double price);

    // Getter
    Instrument* getInstrument() const;
    int getQuantity() const;
    double getAvgPrice() const;

    // 수량 추가 및 평단가 재계산 (물타기)
    void addQuantity(int qty, double currentPrice);

    // 수량 감소 (부분 매도)
    void removeQuantity(int qty);
};
