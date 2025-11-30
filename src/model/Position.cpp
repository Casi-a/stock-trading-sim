#include "../../include/model/Position.h"
using namespace std;

/**
 * Logic 수량 추가 및 평단가 재계산
 * 공식:
 * 새로운 평단가 = ( (기존 수량 * 기존 평단가) + (추가 수량 * 매수 가격) ) / 전체 총 수량
 */
void Position::addQuantity(int qty, double currentPrice) {
    // 0. 유효성 검사
    if (qty <= 0 || currentPrice < 0) {
        return; 
    }

    // 1. 기존 총 매입 금액 계산
    double oldTotalCost = quantity * avgPrice;

    // 2. 새로 추가되는 매입 금액 계산
    double newCost = qty * currentPrice;

    // 3. 수량 업데이트
    quantity += qty;

    // 4. 평단가 재계산 (총 비용 / 총 수량)
    avgPrice = (oldTotalCost + newCost) / quantity;
}

/**
 * Logic 수량 감소 (부분 매도)
 * 매도 해도 평단가는 변하지 않음
 */
void Position::removeQuantity(int qty) {
    // 0. 유효성 검사
    if (qty <= 0) {
        return;
    }

    // 보유 수량보다 많이 뺄 수 없음
    if (quantity < qty) {
        // 실제 Account 로직에서 걸러지겠지만 안전을 위해 0으로 처리
        quantity = 0;
    } else {
        quantity -= qty;
    }
}