#pragma once
#include "Account.h"
#include "Market.h"
#include "OrderManager.h"

/**
 * Class: RiskManager
 * 자산 건전성을 감시하고 강제 청산을 수행합니다.
 */
class RiskManager {
public:
    /**
     * 리스크 체크
     * 1. 마진콜: 총 자산이 초기 자산의 20% 미만이면 -> 모든 포지션 강제 매도
     * 2. 선물 만기: FutureContract의 만기일이 지났으면 -> 해당 포지션 강제 매도
     */
    static void checkRisk(Account* acc, Market* mkt, OrderManager* om);
};