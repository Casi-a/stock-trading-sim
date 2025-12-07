#include "FutureContract.h"

// 생성자
FutureContract::FutureContract(const std::string& sym,
                               const std::string& nm,
                               Instrument* underlyingAsset,
                               double leverageFactor,
                               const std::string& expiry)
    // 부모 생성자 호출: 선물 초기 가격 = 기초자산 가격 * 레버리지
    : Instrument(sym, nm,
                 underlyingAsset ? underlyingAsset->getPrice() * leverageFactor : 0.0,
                 InstrumentType::Future),
      underlying(underlyingAsset),
      leverage(leverageFactor),
      expiryDate(expiry) {
}

// Getter 구현
Instrument* FutureContract::getUnderlying() const {
    return underlying;
}

double FutureContract::getLeverage() const {
    return leverage;
}

const std::string& FutureContract::getExpiryDate() const {
    return expiryDate;
}

// 기초 자산 가격 변동에 따라 선물 가격 재계산
// 변동률에 레버리지를 적용하여 선물 본래 목적(적은 금액으로 큰 수익률)을 구현
void FutureContract::recalcPrice() {
    if (!underlying) return;

    double oldPrice = getPrice();
    double changeRate = underlying->getChangeRate();

    double newPrice = oldPrice * (1.0 + (changeRate / 100.0) * leverage);
    updatePrice(newPrice);
}

// 만기일 확인: 오늘 날짜가 만기일보다 크면 만기된 것
bool FutureContract::isExpired(const std::string& today) const {
    return today > expiryDate;
}
