#pragma once

// 금융 상품 타입 열거형
enum class InstrumentType {
    Spot,    // 현물 주식
    Future,  // 선물 계약
    ETF      // 커스텀 ETF
};
