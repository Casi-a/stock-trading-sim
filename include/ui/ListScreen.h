#pragma once
#include "Screen.h"

/**
 * Class: ListScreen
 * '전체 시세 조회'와 '내 보유 잔고 조회'를 통합한 화면입니다.
 */
class ListScreen : public Screen {
public:
    enum Mode { ALL_MARKET, MY_HOLDINGS }; // Mode에 따라 로직 변경

private:
    Mode mode;

public:
    ListScreen(Application* app, Mode m) : Screen(app), mode(m) {}

    // mode에 따라 Market 목록 또는 Account 포지션 목록 출력
    virtual void show() override;
};