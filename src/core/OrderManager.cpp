#include "OrderManager.h"
#include "Account.h"
#include "Market.h"
#include "Instrument.h"
#include "TimeUtil.h"
#include <iostream>
using namespace std;

// 주문 실행 로직 구현 isbuy에 따라 매수/메도
bool OrderManager::executeOrder(Account* acc, Market* mkt, string symbol, int qty, bool isBuy) {
    // 1. 유효성 검사
    if (acc == nullptr || mkt == nullptr) {
        return false;
    }

    // 2. 종목 확인
    Instrument* inst = mkt->findSymbol(symbol);
    if (inst == nullptr) {
        // 시장에 존재하지 않는 종목
        return false; 
    }
    // 3. 현재가 조회
    double currentPrice = inst->getPrice();
    bool success = false;

    // 4. Account에 매수/매도 요청
    if (isBuy) {
        // 매수: Account::buy(심볼, 수량, 가격, 마켓포인터)
        success = acc->buy(symbol, qty, currentPrice, mkt);
    } else {
        // 매도: Account::sell(심볼, 수량, 가격)
        success = acc->sell(symbol, qty, currentPrice);
    }

    // 5. 거래 성공 시 기록 저장
    if (success) {
        // 현재 시간 가져오기 (TimeUtil 사용)
        // 예: "14:30:00"
        string timestamp = TimeUtil::now(); 

        // Order 객체 생성 (Order.h의 생성자 순서: symbol, isBuy, qty, price, date)
        Order newOrder(symbol, isBuy, qty, currentPrice, timestamp);
        
        // 기록 벡터에 저장
        orderHistory.push_back(newOrder);
        
        // cout << "Order:  " << (isBuy ? "Buy " : "Sell ") 
        //           << symbol << " " << qty << " filled at " << currentPrice << endl;
    }

    return success;
}

// 주문 내역 조회
const std::vector<Order>& OrderManager::getOrderHistory() const {
    return orderHistory;
}