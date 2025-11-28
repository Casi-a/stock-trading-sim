#pragma once
#include "Screen.h"

/**
 * Class: HoldingsScreen
 * 사용자의 계좌(Account)에 있는 보유 주식(Position) 현황을 보여주는 화면입니다.
 */
class HoldingsScreen : public Screen {
public:
    HoldingsScreen(Application* app) : Screen(app) {}

    virtual void show() override;
};