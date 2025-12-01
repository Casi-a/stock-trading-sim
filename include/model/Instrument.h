#pragma once
#include "InstrumentType.h"
#include <string>

// 모든 금융 상품의 공통 속성과 기능을 정의하는 추상 베이스 클래스
class Instrument {
protected:
    std::string symbol;      // 종목 코드
    std::string name;        // 종목 이름
    double currentPrice;     // 현재가
    double highPrice;        // 고가
    double lowPrice;         // 저가
    double changeRate;       // 변동률 (%)
    InstrumentType type;     // 종목 타입

public:
    // 생성자
    Instrument(const std::string& sym, const std::string& nm, double price, InstrumentType t);

    // 가상 소멸자
    virtual ~Instrument() = default;

    // Getter 메서드들
    const std::string& getSymbol() const;
    const std::string& getName() const;
    double getPrice() const;
    double getHigh() const;
    double getLow() const;
    double getChangeRate() const;
    InstrumentType getType() const;

    // 가격 업데이트 (고가, 저가, 변동률 자동 계산)
    virtual void updatePrice(double newPrice);

    // 파생 상품 가격 재계산 (순수 가상 함수)
    virtual void recalcPrice() = 0;
};
