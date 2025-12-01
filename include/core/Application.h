#pragma once
#include "Market.h"
#include "Account.h"
#include "OrderManager.h"
#include "RiskManager.h"
#include "Screen.h"

// 메인 애플리케이션 클래스
class Application {
private:
    Market market;              // 시장 데이터 관리
    Account account;            // 사용자 계좌 관리
    OrderManager orderManager;  // 매수/매도 주문 관리
    RiskManager riskManager;    // 리스크 관리
    Screen* currentScreen;      // 현재 화면
    bool running;               // 메인 루프 실행 여부

    // 초기 종목 데이터 로드
    void initInstruments();

public:
    // 생성자
    Application();

    // 소멸자
    ~Application();

    // Getter
    Market& getMarket();
    Account& getAccount();
    OrderManager& getOrderManager();

    // 메인 루프
    void run();

    // 화면 전환
    void changeScreen(Screen* next);

    // 프로그램 종료
    void exit();
};
