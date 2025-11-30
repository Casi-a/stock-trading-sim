#include "../../include/model/CustomETF.h"
#include <iostream>
using namespace std;
/**
 * Logic 구성 종목 추가
 * - 입력받은 비중(ratio)을 합쳤을 때 1.0(100%)을 넘지 않는지 검사해야 합니다.
 * - 유효하다면 리스트에 추가하고, 즉시 현재가를 갱신합니다.
 */
void CustomETF::addComponent(Instrument* inst, double ratio) {
    // 0. 유효성 검사
    if (inst == nullptr || ratio <= 0.0) {
        cout << "[Error] 유효하지 않은 종목이거나 비중입니다." << endl;
        return;
    }

    // 1. 현재 총 비중 계산
    double currentTotalRatio = 0.0;
    for (const auto& comp : components) {
        currentTotalRatio += comp.second; // pair의 second가 비중(ratio)
    }

    // 2. 비중 초과 여부 확인
    if (currentTotalRatio + ratio > 1.0) {
        cout << "[Error] ETF 구성 비중의 합은 100%(1.0)를 초과할 수 없습니다." << endl;
        cout << " - 현재 합계: " << currentTotalRatio 
                  << ", 추가하려는 비중: " << ratio << endl;
        return;
    }

    // 3. 구성 요소 추가
    components.push_back(make_pair(inst, ratio));

    // 4. 가격 즉시 재계산 (새로운 종목이 들어왔으니 가격이 변함)
    updatePrice();
}

/**
 * Logic 가격 재계산 (Override)
 * - ETF의 가격 = Σ (개별 종목 가격 * 개별 종목 비중)
 * - Market::tick()에서 이 함수가 호출되면, 구성 종목들의 최신 가격을 반영한다.
 */
void CustomETF::updatePrice() {
    double newPrice = 0.0;

    // 구성 종목을 순회하며 가중 평균 가격 계산
    for (const auto& comp : components) {
        Instrument* inst = comp.first;  // 종목 포인터
        double ratio = comp.second;     // 비중

        // 해당 종목의 현재가 * 비중
        newPrice += inst->getPrice() * ratio;
    }

    // 부모 클래스(Instrument)의 currentPrice 갱신
    this->currentPrice = newPrice;
}