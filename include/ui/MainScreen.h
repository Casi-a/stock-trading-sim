#pragma once
#include "Screen.h"

// 메인 메뉴 화면
class MainScreen : public Screen {
public:
    MainScreen(Application* app) : Screen(app) {}

    // 메인 메뉴 출력 및 화면 전환 입력 처리
    virtual void show() override;
};
