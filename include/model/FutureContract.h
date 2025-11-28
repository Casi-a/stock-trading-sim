#pragma once
#include "Instrument.h"

/**
 * Class: FutureContract
 * 선물 계약 상품입니다. 만기일과 레버리지 속성을 가집니다.
 */
class FutureContract : public Instrument {
private:
    std::string expiryDate; // 만기일 (예: "2025-12-31")
    double leverage;        // 레버리지 배율 (예: 2.0 -> 변동폭 2배)

public:
    FutureContract(std::string sym, std::string nm, double price, std::string expiry, double lev)
        : Instrument(sym, nm, price), expiryDate(expiry), leverage(lev) {}

    // getter
    std::string getExpiryDate() const { return expiryDate; }
    double getLeverage() const { return leverage; }

    // 현물보다 큰 변동폭(레버리지 적용)으로 가격을 업데이트합니다.
    virtual void updatePrice() override;

    // 만기일이 오늘 날짜(today)를 지났는지 확인
    bool isExpired(std::string today) const;
};