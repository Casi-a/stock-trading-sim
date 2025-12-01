#pragma once

// 전방 선언
class Application;

// 모든 화면의 추상 베이스 클래스
class Screen {
protected:
    Application* app;  // Application 객체 포인터

    // 화면 클리어 (선택적 사용)
    void clear();

public:
    // 생성자
    Screen(Application* application);

    // 가상 소멸자
    virtual ~Screen() = default;

    // 순수 가상 함수: 각 화면마다 구현 필요
    virtual void show() = 0;
};
