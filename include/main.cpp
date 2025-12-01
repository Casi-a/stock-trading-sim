#include "Application.h"
#include <iostream>

int main() {
    std::cout << "=== Stock Trading Simulator ===" << std::endl;
    std::cout << "프로그램을 시작합니다..." << std::endl << std::endl;

    Application app;
    app.run();

    std::cout << "\n프로그램을 종료합니다. 감사합니다!" << std::endl;
    return 0;
}
