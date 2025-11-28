#pragma once
#include "Screen.h"

class CustomETFScreen : public Screen {
public:
    CustomETFScreen(Application* app) : Screen(app) {}

    // 사용자 입력을 받아 CustomETF 객체 생성 후 Market에 등록
    virtual void show() override;
};