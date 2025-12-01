#pragma once
#include "Screen.h"

// 전체 종목 조회 화면
// 담당: 신바다
class AllStocksScreen : public Screen {
public:
    AllStocksScreen(Application* app) : Screen(app) {}

    // 전체 종목 리스트 출력
    virtual void show() override;
};
