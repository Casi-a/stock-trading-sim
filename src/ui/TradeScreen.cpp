#include "TradeScreen.h"
#include "MainScreen.h"
#include "Application.h"
#include <iostream>
#include <limits>

using namespace std;

// 생성자
TradeScreen::TradeScreen(Application* app, bool buyOrSell)
    : Screen(app), isBuy(buyOrSell) {
}

// 템플릿 헬퍼 함수 구현
template <typename T>
T TradeScreen::getValidInput(const std::string& prompt, T minVal, T maxVal) {
    T value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            } else {
                std::cout << "입력 범위를 벗어났습니다. 다시 입력해주세요.\n";
            }
        } else {
            std::cout << "잘못된 입력입니다. (숫자만 입력 가능)\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// 심볼 입력 헬퍼 함수
std::string TradeScreen::getValidSymbolInput(const std::string& prompt) {
    std::string symbol;
    std::cout << prompt;
    std::cin >> symbol;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return symbol;
}

// 화면 출력 및 주문 실행 로직
void TradeScreen::show() {
    std::cout << "\n======================================================\n";
    std::cout << "------------ " << (isBuy ? "매수(BUY)" : "매도(SELL)") << " 주문 화면 ------------\n";

    // 1. 거래 유형 선택
    std::cout << "거래 유형을 선택하세요:\n";
    std::cout << "1. 현물 / 2. 선물 / 3. ETF 거래\n";
    int typeChoice = getValidInput<int>("선택 (1~3): ", 1, 3);

    // 2. 종목 코드 및 수량 입력
    std::string symbol = getValidSymbolInput("종목 코드 입력: ");
    int quantity = getValidInput<int>("수량 입력 (1 이상): ", 1);

    // 3. 주문 실행
    std::cout << "\n[주문 실행: " << (isBuy ? "매수" : "매도") << " " << symbol << " (" << quantity << "개)]\n";

    bool success = app->getOrderManager().executeOrder(
        &app->getAccount(),
        &app->getMarket(),
        symbol,
        quantity,
        isBuy
    );

    // 4. 결과 출력
    if (success) {
        std::cout << "주문 성공 [" << symbol << " " << quantity << "개 " << (isBuy ? "매수" : "매도") << " 체결]\n";
    } else {
        std::cout << "주문 실패\n";
        std::cout << " (원인: 잔고 부족, 보유 수량 부족, 종목 미등록 등)\n";
    }

    // 5. 메인 화면으로 돌아가기
    std::cout << "\n------------------------------------------------------\n";
    std::cout << "메인 화면으로 돌아가려면 엔터를 누르세요...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    app->changeScreen(new MainScreen(app));
}
