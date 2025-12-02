#include "HoldingsScreen.h"
#include "MainScreen.h"
#include "Application.h"
#include "Account.h"
#include "Position.h"
#include "Instrument.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

// 포지션 테이블 출력 (private 헬퍼 함수)
void HoldingsScreen::printPositionTable(const string& title, const vector<Position*>& positions) {
    cout << endl << title << endl;

    if (positions.empty()) {
        cout << "  (보유 종목 없음)" << endl;
        return;
    }

    // 테이블 헤더
    cout << left << setw(10) << "Symbol"
         << right << setw(8) << "Qty"
         << setw(12) << "Avg Price"
         << setw(12) << "Cur Price"
         << setw(12) << "P&L(%)" << endl;

    // 데이터 출력
    for (const auto& pos : positions) {
        auto inst = pos->getInstrument();

        double currentPrice = inst->getPrice();
        double avgPrice = pos->getAvgPrice();

        // 손익률 계산
        double profitRate = 0.0;
        if (avgPrice != 0) {
            profitRate = ((currentPrice - avgPrice) / avgPrice) * 100.0;
        }

        cout << left << setw(10) << inst->getSymbol()
             << right << setw(8) << pos->getQuantity()
             << setw(12) << fixed << setprecision(2) << avgPrice
             << setw(12) << currentPrice
             << setw(11) << profitRate << "%" << endl;
    }
}

// 메인 로직
void HoldingsScreen::show() {
    clear();

    cout << "========== 보유 종목(Holdings) ==========" << endl;

    Account& account = app->getAccount();

    // 1. 현물(Spot) 포지션
    vector<Position*> spotPositions = account.getPositionsByType(InstrumentType::Spot);
    printPositionTable("[현물 주식]", spotPositions);

    // 2. 선물(Future) 포지션
    vector<Position*> futurePositions = account.getPositionsByType(InstrumentType::Future);
    printPositionTable("[선물]", futurePositions);

    // 3. ETF 포지션
    vector<Position*> etfPositions = account.getPositionsByType(InstrumentType::ETF);
    printPositionTable("[커스텀 ETF]", etfPositions);

    // 4. 뒤로가기
    cout << "\n-----------------------------------------" << endl;
    cout << "0. 뒤로가기" << endl;
    cout << "선택: ";

    int choice;
    cin >> choice;

    if (choice == 0) {
        app->changeScreen(new MainScreen(app));
    }
}
