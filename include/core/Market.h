#pragma once
#include <vector>
#include <string>
#include "../model/Instrument.h"

/**
 * Class: Market
 * 전체 종목 데이터를 관리하고 시세를 변동시킵니다.
 */
class Market {
private:
    std::vector<Instrument*> instruments; // 모든 상장 종목 리스트

public:
    ~Market(); // 소멸자에서 Instrument 객체들 delete

    // 하드코딩된 종목(Spot, Future) 데이터 생성 및 초기화
    void init(); 

    // 매 틱마다 모든 종목의 updatePrice() 호출
    void tick(); 

    // 종목 코드로 객체 검색 (없으면 nullptr)
    Instrument* findSymbol(std::string sym);
    
    // ETF 생성 시 시장 리스트에 등록
    void addInstrument(Instrument* inst) { instruments.push_back(inst); }
    
    // instruments 리스트 getter
    const std::vector<Instrument*>& getAllInstruments() const { return instruments; }
};