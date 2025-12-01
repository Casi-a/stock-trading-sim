#pragma once
#include "Instrument.h"
#include <vector>
#include <utility>

// 커스텀 ETF 클래스: 여러 종목을 묶어서 하나의 상품처럼 거래
class CustomETF : public Instrument {
private:
    // 구성 요소: <Instrument 포인터, 비중(0.0~1.0)>
    std::vector<std::pair<Instrument*, double>> components;

public:
    // 생성자: 초기 가격은 0으로 시작 (구성 요소 추가 후 재계산)
    CustomETF(const std::string& sym, const std::string& nm);

    // 구성 종목 추가
    void addComponent(Instrument* inst, double ratio);

    // ETF 가격 재계산: Σ(개별 종목 가격 * 비중)
    void recalcPrice() override;

    // updatePrice는 recalcPrice 내부에서 사용
    void updatePrice() {
        recalcPrice();
    }
};
