#pragma once
#include "Screen.h"
#include <vector>
#include <string>

// 전방 선언
class Position;

// 보유 종목 조회 화면
class HoldingsScreen : public Screen {
private:
    // 포지션 테이블 출력 헬퍼 함수
    void printPositionTable(const std::string& title, const std::vector<Position*>& positions);

public:
    HoldingsScreen(Application* app) : Screen(app) {}

    virtual void show() override;
};
