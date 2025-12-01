#pragma once
#include "Instrument.h"
#include <string>

// 선물 계약 클래스
class FutureContract : public Instrument {
private:
    Instrument* underlying;   // 기초 자산 포인터
    double leverage;          // 레버리지 배율
    std::string expiryDate;   // 만기일 (YYYY-MM-DD)

public:
    // 생성자
    FutureContract(const std::string& sym,
                   const std::string& nm,
                   Instrument* underlying,
                   double leverageFactor,
                   const std::string& expiry);

    // Getter
    Instrument* getUnderlying() const;
    double getLeverage() const;
    const std::string& getExpiryDate() const;

    // 기초 자산 가격 변동에 따라 선물 가격 재계산
    void recalcPrice() override;

    // 만기일 확인
    bool isExpired(const std::string& today) const;
};
