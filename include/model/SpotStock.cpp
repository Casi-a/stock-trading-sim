#include "SpotStock.h"

// 생성자: 부모 Instrument 생성자 호출하며 타입은 Spot으로 지정
SpotStock::SpotStock(const std::string& sym, const std::string& nm, double price)
    : Instrument(sym, nm, price, InstrumentType::Spot) {
}

// 현물 주식은 자기 자신이 기준이 되는 자산이므로 재계산 불필요
void SpotStock::recalcPrice() {
    // 아무 작업도 하지 않음
}
