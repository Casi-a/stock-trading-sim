#pragma once
#include "Screen.h"

class TradeScreen : public Screen {
private:
    bool isBuyMode; // true: 매수, false: 매도

public:
    TradeScreen(Application* app, bool isBuy) : Screen(app), isBuyMode(isBuy) {}

    // [Logic] 종목코드, 수량 입력받아 OrderManager::executeOrder 호출
    virtual void show() override;
};