#include "AllStocksScreen.h"
#include "MainScreen.h"
#include "Application.h"
#include "Market.h"
#include "Instrument.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void AllStocksScreen::show() {
    clear();

    cout << "========== 전체 종목 조회(All Stocks) ==========" << endl;

    Market& market = app->getMarket();
    const vector<Instrument*>& allInstruments = market.getAll();

    if (allInstruments.empty()) {
        cout << "등록된 종목이 없습니다." << endl;
    } else {
        // 테이블 헤더
        cout << left << setw(12) << "종목 코드"
             << setw(30) << "종목 이름"
             << right << setw(12) << "현재가"
             << setw(12) << "고가"
             << setw(12) << "저가"
             << setw(12) << "변동률" << endl;
        cout << string(90, '-') << endl;

        // 모든 종목 출력
        for (const auto& inst : allInstruments) {
            double change = inst->getChangeRate();
            string sign = (change >= 0) ? "+" : "";

            cout << left << setw(12) << inst->getSymbol()
                 << setw(30) << inst->getName()
                 << right << setw(12) << fixed << setprecision(2) << inst->getPrice()
                 << setw(12) << inst->getHigh()
                 << setw(12) << inst->getLow()
                 << setw(11) << sign << change << "%" << endl;
        }
    }

    // 뒤로가기
    cout << "\n-----------------------------------------" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";

    int choice;
    cin >> choice;

    if (choice == 0) {
        app->changeScreen(new MainScreen(app));
    }
}
