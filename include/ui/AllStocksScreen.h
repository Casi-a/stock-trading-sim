#pragma once
#include "Screen.h"

/**
 * Class: AllStocksScreen
 * Market에 존재하는 모든 종목(Instrument)의 목록을 조회하는 화면입니다.
 * - 종목코드, 종목명, 현재가, (선택: 등락률) 등을 표 형태로 출력합니다.
 */
class AllStocksScreen : public Screen {
public:
    AllStocksScreen(Application* app) : Screen(app) {}

    virtual void show() override;
};