#include "Screen.h"
#include <iostream>

// 생성자
Screen::Screen(Application* application) : app(application) {
}

// 화면 클리어 (간단한 줄바꿈으로 구현)
void Screen::clear() {
    // 여러 줄 출력으로 화면 정리 효과
    for (int i = 0; i < 2; i++) {
        std::cout << std::endl;
    }
}
