#pragma once
#include "Instrument.h"
#include <vector>
#include <utility> // std::pair

/**
 * Class: CustomETF
 * 사용자가 직접 구성한 ETF.
 */
class CustomETF : public Instrument {
private:
    // pair<종목 포인터, 비중(0.0 ~ 1.0)>
    std::vector<std::pair<Instrument*, double>> components;

public:
    CustomETF(std::string sym, std::string nm) 
        : Instrument(sym, nm, 0.0) {}

    /**
     * 구성 종목 추가
     * ratio의 합이 1.0을 넘지 않도록 관리하는 로직 필요.
     */
    void addComponent(Instrument* inst, double ratio);

    /**
     * 가격 재계산
     * 모든 component를 순회하며 (가격 * 비중)의 합으로 내 가격(currentPrice)을 갱신합니다.
     */
    virtual void updatePrice() override;
};