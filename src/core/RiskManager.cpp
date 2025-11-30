#include "../../include/core/RiskManager.h"
#include "../../include/core/Account.h"
#include "../../include/core/Market.h"
#include "../../include/core/OrderManager.h"
#include "../../include/model/Instrument.h"
#include "../../include/model/FutureContract.h" // dynamic_cast용
#include "../../include/model/Position.h"
#include "../../include/util/TimeUtil.h"        // 날짜 확인용
#include <iostream>
#include <vector>
using namespace std;

void RiskManager::checkRisk(Account* acc, Market* mkt, OrderManager* om) {
    // 0. 안전 장치
    if (acc == nullptr || mkt == nullptr || om == nullptr) {
        return;
    }

    // ===========================
    // 1. 마진콜 (Margin Call) 체크
    // - 총 자산이 초기자산 20% 미만 -> 강제 매도
    // ===========================
    double currentEquity = acc->getTotalEquity();   // 현재 총 자산 (현금 + 주식평가액)
    double initialEquity = acc->getInitialEquity(); // 초기 자본금

    // 조건: 자산이 초기 자본의 20% 미만으로 떨어졌을 때
    if (currentEquity < initialEquity * 0.2) {
        cout << "\n자산이 20% 미만입니다. 강제 청산을 진행합니다.\n" << endl;

        // 매도 시 positions 벡터가 변경되므로, 복사본을 만들어 순회해야 안전
        vector<Position*> currentPositions = acc->getPositions();

        for (Position* pos : currentPositions) {
            Instrument* inst = pos->getInstrument();
            string symbol = inst->getSymbol();
            int qty = pos->getQuantity();

            // OrderManager를 통해 시장가 매도 실행 (isBuy = false)
            bool result = om->executeOrder(acc, mkt, symbol, qty, false);
            
            if (result) {
                cout << "[강제청산] " << symbol << " (" << qty << "주) 매도 완료." << endl;
            }
        }
        
        // 마진콜이 발생했다면 선물 만기 체크는 할 필요가 없으므로 종료
        return; 
    }

    // ================================
    // 2. 선물 만기 (Futures Expiry) 체크
    // ================================
    string today = TimeUtil::getToday(); // 오늘 날짜 (YYYY-MM-DD)
    
    // 삭제 대상을 저장할 리스트 (반복자 무효화 방지)
    vector<string> expiredSymbols;

    // 2-1. 만기된 선물 종목 식별
    const vector<Position*>& positions = acc->getPositions();
    for (Position* pos : positions) {
        Instrument* inst = pos->getInstrument();
        
        // dynamic_cast를 사용하여 이 상품이 FutureContract인지 확인
        FutureContract* future = dynamic_cast<FutureContract*>(inst);

        if (future != nullptr) {
            // 선물 계약이 맞다면 만기일 체크
            if (future->isExpired(today)) {
                expiredSymbols.push_back(inst->getSymbol());
            }
        }
    }

    // 2-2. 식별된 종목 강제 청산
    for (const string& symbol : expiredSymbols) {
        // 청산 전 현재 수량을 다시 확인 (안전성 확보)
        Position* pos = acc->findPosition(symbol);
        if (pos != nullptr) {
            int qty = pos->getQuantity();
            
            // 만기 청산 매도 실행
            bool result = om->executeOrder(acc, mkt, symbol, qty, false);

            if (result) {
                cout << "\n[알림] 선물 만기 도래: " << symbol << " (" << qty << "주) 자동 청산되었습니다." << endl;
            }
        }
    }
}