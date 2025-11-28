#pragma once
#include "Instrument.h"

/**
 * Class: SpotStock
 * 일반 현물 주식입니다.
 */
class SpotStock : public Instrument {
public:
    SpotStock(std::string sym, std::string nm, double price)
        : Instrument(sym, nm, price) {}

    // RandomUtil을 사용하여 -5% ~ +5% 범위 내에서 가격을 랜덤하게 변동시킵니다.
    virtual void updatePrice() override;
};