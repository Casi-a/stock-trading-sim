#pragma once
#include "../core/Application.h"
#include <cstdlib>

/**
 * Class: Screen
 * 모든 화면의 부모 클래스입니다.
 */
class Screen {
protected:
    Application* app; // 앱 기능 사용을 위한 포인터

public:
    Screen(Application* application) : app(application) {}
    virtual ~Screen() {}

    /**
     * [Abstract Function]
     * 화면을 그리고 입력을 처리하는 로직을 구현해야 합니다.
     */
    virtual void show() = 0;
    
    void clear() { 
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};