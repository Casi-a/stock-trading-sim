#pragma once
#include <string>
#include <iostream>

/**
 * Class: Instrument
 * 모든 금융 상품(주식, 선물, ETF)의 부모 추상 클래스입니다.
 * 공통적인 속성(이름, 가격 등)을 정의합니다.
 */
class Instrument {
protected:
    std::string symbol;      // 종목 코드 (Primary Key, 예: "NVDA", "AMAT")
    std::string name;        // 종목명 (예: "NVIDIA Corporation")
    double currentPrice;     // 현재 시장 가격

public:
    Instrument(std::string sym, std::string nm, double price)
        : symbol(sym), name(nm), currentPrice(price) {}
    
    // 
    virtual ~Instrument() {}

    // [Getters]
    std::string getSymbol() const { return symbol; }
    std::string getName() const { return name; }
    double getPrice() const { return currentPrice; }

    /**
     * Virtual Function: updatePrice
     * 가격 변동 로직을 자식 클래스에서 반드시 구현
     * - SpotStock: 랜덤 등락
     * - FutureContract: 레버리지 적용 등락
     * - CustomETF: 구성 종목 가격의 가중 평균 재계산
     */
    virtual void updatePrice() = 0; 
};