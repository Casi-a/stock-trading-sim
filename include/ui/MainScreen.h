#pragma once
#include "Screen.h"

class MainScreen : public Screen {
public:
    MainScreen(Application* app) : Screen(app) {}
    
    // [Logic] 메인 메뉴 출력 및 화면 전환 분기 처리
    virtual void show() override;
};