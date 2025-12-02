// CustomETFScreen.cpp
// 메뉴 5: 사용자 커스텀 ETF 구성/수정 화면
// - 사용자에게 종목 심볼과 비중(%)을 입력받아서
//   UI 레벨에서 필요한 모든 정보를 수집하는 역할
// - 실제 CustomETF 객체 생성 및 Market 등록은
//   도메인 계층(Application/Service)에서 이어서 처리

#include "CustomETFScreen.h"

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <cmath>

// 이 파일에서 사용하는 함수/타입을
// (namespace {}) 안에 넣어서 링크 충돌을 방지
namespace {

    // 정수 입력 유틸
    int readInt(const std::string& prompt) {
        using namespace std;

        while (true) {
            cout << prompt;

            int value;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }

            // 잘못 입력된 경우 처리
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[오류] 정수를 입력하세요.\n";
        }
    }

    // 실수 입력 유틸 (비중 입력에 사용)
    double readDouble(const std::string& prompt) {
        using namespace std;

        while (true) {
            cout << prompt;

            double value;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[오류] 숫자를 입력하세요.\n";
        }
    }

    // 한 종목의 구성 정보
    struct ComponentInput {
        std::string symbol;   // 종목 심볼
        double weight;        // 비중(%)
    };
}

// show() 구현
void CustomETFScreen::show() {
    using namespace std;

    cout << "====================[ 커스텀 ETF 구성/수정 ]====================\n\n";

    // 1. ETF 기본 정보 입력 --------------------------------------------------
    string etfSymbol;
    cout << "생성할 커스텀 ETF의 심볼을 입력하세요 (예: CUST1): ";
    cin >> etfSymbol;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string etfName;
    cout << "커스텀 ETF의 이름을 입력하세요: ";
    getline(cin, etfName);

    cout << "\n--- 구성 종목 정보 입력 ---\n";

    // 2. 구성 종목 개수 입력 -------------------------------------------------
    int count = 0;
    while (count <= 0) {
        count = readInt("구성할 종목 개수를 입력하세요 (1 이상): ");
        if (count <= 0) {
            cout << "[오류] 최소 1개 이상의 종목이 필요합니다.\n";
        }
    }

    vector<ComponentInput> components;
    components.reserve(static_cast<size_t>(count));

    double totalWeight = 0.0;

    // 3. 각 종목 심볼 + 비중(%) 입력 -----------------------------------------
    for (int i = 0; i < count; ++i) {
        cout << "\n[" << (i + 1) << " / " << count << "] 번째 종목\n";

        ComponentInput comp;

        cout << "종목 심볼을 입력하세요 (예: AAPL, TSLA 등): ";
        cin >> comp.symbol;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 비중(%) 입력
        comp.weight = -1.0;
        while (comp.weight <= 0.0) {
            comp.weight = readDouble("비중(%)을 입력하세요 (0보다 큰 값): ");
            if (comp.weight <= 0.0) {
                cout << "[오류] 비중은 0보다 커야 합니다.\n";
            }
        }

        totalWeight += comp.weight;
        components.push_back(comp);
    }

    // 4. 입력 결과 요약 출력 --------------------------------------------------
    cout << "\n====================[ 커스텀 ETF 요약 ]====================\n";
    cout << "심볼 : " << etfSymbol << "\n";
    cout << "이름 : " << etfName  << "\n\n";

    cout << "구성 종목 리스트 (입력한 비중 기준)\n";
    cout << "---------------------------------------------\n";
    cout << "  심볼\t비중(%)\n";
    cout << "---------------------------------------------\n";

    for (const auto& comp : components) {
        cout << "  " << comp.symbol << "\t" << comp.weight << "%\n";
    }

    cout << "---------------------------------------------\n";
    cout << "비중 합계 : " << totalWeight << "%\n";

    // 비중 합계가 100%인지 검증
    if (std::fabs(totalWeight - 100.0) > 1e-6) {
        cout << "\n[경고] 비중 합계가 100%가 아닙니다.\n";
        cout << "  - 현재 합계: " << totalWeight << "%\n";
    }

    // 5. 도메인 계층 연동 설명 ------------------------------------------------
    // 실제 CustomETF 객체 생성 및 Market 등록은
    // Application / Domain / Service 계층에서 아래 순서로 구현:
    //
    // 1) Market* market = app->getMarket();
    // 2) CustomETF custom(etfSymbol, etfName);
    // 3) for (모든 components):
    //        Instrument* inst = market->findBySymbol(comp.symbol);
    //        custom.addComponent(inst, comp.weight / 100.0);
    // 4) custom.recalcPrice();
    // 5) market->addInstrument(custom);

    cout << "\n※ 참고: 실제 CustomETF 생성/등록은 도메인 계층에서 처리해야 합니다.\n\n";

    // 화면 유지
    cout << "엔터를 누르면 이전 화면으로 돌아갑니다...";
    cin.get();
}

#include "MainScreen.h"
#include "Application.h"
