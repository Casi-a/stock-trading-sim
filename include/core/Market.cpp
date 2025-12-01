#include "Market.h"
#include "RandomUtil.h"
#include <algorithm>
#include <iostream>

// 특정 심볼이 이미 리스트에 있는지 확인
bool Market::checkSymbol(const std::string& symbol) const {
    for (Instrument* inst : instruments) {
        if (inst->getSymbol() == symbol)
            return true;
    }
    return false;
}

// 소멸자: 동적 할당된 Instrument 객체들 해제
Market::~Market() {
    for (Instrument* inst : instruments) {
        delete inst;
    }
}

// 종목 추가
void Market::addInstrument(Instrument* inst) {
    if (inst == nullptr) {
        return;
    }
    instruments.push_back(inst);
}

// 모든 종목 반환 (읽기 전용)
const std::vector<Instrument*>& Market::getAll() const {
    return instruments;
}

// 심볼로 종목 검색
Instrument* Market::findBySymbol(const std::string& symbol) const {
    for (Instrument* inst : instruments) {
        if (inst->getSymbol() == symbol) return inst;
    }
    return nullptr;
}

// 시간 흐름(tick)에 따른 가격 변동 처리
void Market::tick() {
    // 1) Spot 가격 랜덤 변동
    for (Instrument* inst : instruments) {
        if (inst->getType() == InstrumentType::Spot) {
            // -3% ~ +3% 사이의 랜덤 변동률
            double rate = RandomUtil::randomRate(-0.03, 0.03);
            // 변동률 적용해서 새로운 가격 계산
            double newPrice = inst->getPrice() * (1.0 + rate);
            // 가격이 0.01 미만으로 내려가지 않도록 방지
            if (newPrice < 0.01) newPrice = 0.01;
            inst->updatePrice(newPrice);
        }
    }

    // 2) 파생상품(선물 등) 가격 재계산
    for (Instrument* inst : instruments) {
        if (inst->getType() != InstrumentType::Spot) {
            inst->recalcPrice();
        }
    }
}

// 변동률 상위 n개 종목 반환
std::vector<Instrument*> Market::getTopMovers(int n) const {
    std::vector<Instrument*> copy = instruments;
    std::sort(copy.begin(), copy.end(),
              [](const Instrument* a, const Instrument* b) {
                  return a->getChangeRate() > b->getChangeRate();
              });
    if (static_cast<int>(copy.size()) > n) copy.resize(n);
    return copy;
}
