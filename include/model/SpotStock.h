#pragma once
#include "Instrument.h"

// 현물 주식 클래스
class SpotStock : public Instrument {
public:
    SpotStock(const std::string& sym, const std::string& nm, double price);

    // 현물은 자기 자신이 기준 자산이므로 재계산 불필요
    void recalcPrice() override;
};
