#include "MainScreen.h"
#include "HoldingsScreen.h"
#include "TradeScreen.h"
#include "CustomETFScreen.h"
#include "AllStocksScreen.h"
#include "Application.h"
#include "Market.h"
#include "Account.h"
#include "Instrument.h"
#include "TimeUtil.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void MainScreen::show() {
    // 1. 화면 초기화
    clear();

    // 2. 도메인 객체 참조
    Market& market = app->getMarket();
    Account& account = app->getAccount();

    // 3. 상단 UI: 시간 및 잔고 출력
    cout << "## [현재 시간] " << TimeUtil::now() << endl;

    double equity = account.getTotalEquity();
    cout << "==== Stock Market ==== 현재 잔고: " << fixed << setprecision(0) << equity << endl;

    // 4. 중단 UI: 변동률 TOP 5 출력
    cout << "--------------------------------------------------------" << endl;
    cout << left << setw(10) << "종목명"
         << right << setw(10) << "현재가"
         << setw(10) << "고가"
         << setw(10) << "저가"
         << setw(12) << "변동률" << endl;

    vector<Instrument*> topMovers = market.getTopMovers(5);

    for (const auto& inst : topMovers) {
        double change = inst->getChangeRate();
        string sign = (change >= 0) ? "+" : "";

        cout << left << setw(10) << inst->getSymbol()
             << right << setw(10) << fixed << setprecision(2) << inst->getPrice()
             << setw(10) << inst->getHigh()
             << setw(10) << inst->getLow()
             << setw(11) << sign << change << "%" << endl;
    }

    // 5. 하단 UI: 메인 메뉴 출력
    cout << "--------------------------------" << endl;
    cout << "1. 전체 종목 조회" << endl;
    cout << "2. 보유 종목 조회" << endl;
    cout << "3. 매수" << endl;
    cout << "4. 매도" << endl;
    cout << "5. ETF 커스텀" << endl;
    cout << "6. 종료" << endl;
    cout << "---" << endl;
    cout << "선택: ";

    // 6. 입력 처리
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    // 7. 화면 전환 분기
    switch (choice) {
    case 1:
        app->changeScreen(new AllStocksScreen(app));
        break;
    case 2:
        app->changeScreen(new HoldingsScreen(app));
        break;
    case 3:
        app->changeScreen(new TradeScreen(app, true));  // 매수
        break;
    case 4:
        app->changeScreen(new TradeScreen(app, false)); // 매도
        break;
    case 5:
        app->changeScreen(new CustomETFScreen(app));
        break;
    case 6:
        app->exit();
        break;
    default:
        cout << "잘못된 선택입니다." << endl;
        break;
    }
}
