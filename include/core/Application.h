#pragma once

// 전방 선언 (Forward Declaration)으로 순환 참조 방지
class Screen;
class Market;
class Account;
class OrderManager;

/**
 * Class: Application
 * 프로그램의 메인 컨트롤러.
 */
class Application {
private:
    bool isRunning;          // 프로그램 실행 상태 플래그
    
    // Core Managers (소유권)
    Market* market;
    Account* account;
    OrderManager* orderManager;

    // Current View
    Screen* currentScreen;   // 현재 보여지는 화면

public:
    Application();
    ~Application();

    /**
     * 메인 루프
     * while(isRunning) 동안 시간흐름 -> 시세변동 -> 리스크체크 -> 화면출력 반복
     */
    void run();

    // 화면 교체
    void changeScreen(Screen* newScreen);
    
    void exit() { isRunning = false; }

    // Getters
    Market* getMarket() const { return market; }
    Account* getAccount() const { return account; }
    OrderManager* getOrderManager() const { return orderManager; }
};