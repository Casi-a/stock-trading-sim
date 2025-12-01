#pragma once
#include "Instrument.h"
#include <vector>
#include <string>

// 주식 시장 시뮬레이터 클래스
class Market {
private:
    std::vector<Instrument*> instruments;  // 모든 종목 리스트

    // 특정 심볼이 이미 리스트에 있는지 확인
    bool checkSymbol(const std::string& symbol) const;

public:
    // 생성자
    Market() = default;

    // 소멸자: 동적 할당된 Instrument 객체들 해제
    ~Market();

    // 종목 추가
    void addInstrument(Instrument* inst);

    // 모든 종목 반환 (읽기 전용)
    const std::vector<Instrument*>& getAll() const;

    // 심볼로 종목 검색
    Instrument* findBySymbol(const std::string& symbol) const;

    // 시간 흐름에 따른 가격 변동 처리
    void tick();

    // 변동률 상위 n개 종목 반환
    std::vector<Instrument*> getTopMovers(int n) const;
};
