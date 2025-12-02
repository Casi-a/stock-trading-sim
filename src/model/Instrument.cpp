#include "Instrument.h"
#include <cmath>

// 생성자
Instrument::Instrument(const std::string& sym, const std::string& nm, double price, InstrumentType t)
    : symbol(sym), name(nm), currentPrice(price),
      highPrice(price), lowPrice(price), changeRate(0.0), type(t) {
}

// Getter 구현
const std::string& Instrument::getSymbol() const {
    return symbol;
}

const std::string& Instrument::getName() const {
    return name;
}

double Instrument::getPrice() const {
    return currentPrice;
}

double Instrument::getHigh() const {
    return highPrice;
}

double Instrument::getLow() const {
    return lowPrice;
}

double Instrument::getChangeRate() const {
    return changeRate;
}

InstrumentType Instrument::getType() const {
    return type;
}

// 가격 업데이트: 새 가격이 들어오면 현재가, 고가, 저가, 변동률을 업데이트
void Instrument::updatePrice(double newPrice) {
    const double oldPrice = currentPrice;

    // 고가/저가 갱신
    if (newPrice > highPrice) highPrice = newPrice;
    if (newPrice < lowPrice) lowPrice = newPrice;

    currentPrice = newPrice;

    // 변동률 계산: (신가격 - 구가격) / 구가격 * 100
    if (std::fabs(oldPrice) > 1e-9) {
        changeRate = (newPrice - oldPrice) / oldPrice * 100.0;
    } else {
        changeRate = 0.0;
    }
}
