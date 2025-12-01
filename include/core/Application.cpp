#include "Application.h"
#include "MainScreen.h"
#include "SpotStock.h"
#include "FutureContract.h"
#include "TimeUtil.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

// 초기 종목 데이터 로드
void Application::initInstruments() {
    // Spot 주식 약 50개 하드코딩
    struct Seed { const char* sym; const char* name; double price; };

    std::vector<Seed> seeds = {
        {"AAPL", "Apple Inc.", 186.20},
        {"TSLA", "Tesla Inc.", 226.82},
        {"MSFT", "Microsoft Corp.", 413.71},
        {"NVDA", "NVIDIA Corp.", 129.32},
        {"AMZN", "Amazon.com Inc.", 170.92},
        {"GOOGL", "Alphabet Inc.", 140.11},
        {"META", "Meta Platforms", 312.33},
        {"NFLX", "Netflix Inc.", 402.44},
        {"INTC", "Intel Corp.", 35.12},
        {"AMD",  "Advanced Micro Devices", 121.55},
        {"ORCL", "Oracle Corp.", 110.32},
        {"IBM",  "IBM Corp.", 152.11},
        {"SAP",  "SAP SE", 145.21},
        {"ADBE", "Adobe Inc.", 512.50},
        {"AVGO", "Broadcom Inc.", 1150.0},
        {"QCOM", "Qualcomm Inc.", 132.70},
        {"CSCO", "Cisco Systems", 54.20},
        {"TXN",  "Texas Instruments", 168.30},
        {"SONY", "Sony Group", 93.40},
        {"SHOP", "Shopify Inc.", 65.10},
        {"BABA", "Alibaba Group", 78.20},
        {"PDD",  "PDD Holdings", 128.40},
        {"JD",   "JD.com", 30.50},
        {"UBER", "Uber Technologies", 70.30},
        {"LYFT", "Lyft Inc.", 12.40},
        {"DIS",  "Walt Disney", 92.10},
        {"KO",   "Coca-Cola", 58.20},
        {"PEP",  "PepsiCo", 173.30},
        {"NKE",  "Nike Inc.", 96.90},
        {"SBUX","Starbucks Corp.", 95.20},
        {"MCD", "McDonald's", 280.10},
        {"V",   "Visa Inc.", 265.40},
        {"MA",  "Mastercard", 402.30},
        {"PYPL","PayPal", 61.70},
        {"SQ",  "Block Inc.", 72.20},
        {"CRM", "Salesforce", 248.30},
        {"INTU","Intuit Inc.", 620.40},
        {"NOW", "ServiceNow", 730.50},
        {"SNOW","Snowflake Inc.", 190.60},
        {"ZM",  "Zoom Video", 62.30},
        {"ABNB","Airbnb Inc.", 150.40},
        {"SPOT","Spotify", 210.20},
        {"ROKU","Roku Inc.", 70.80},
        {"PANW","Palo Alto Networks", 310.50},
        {"NET", "Cloudflare", 95.60},
        {"DOCU","DocuSign", 48.70},
        {"TWLO","Twilio", 60.40},
        {"PLTR","Palantir", 23.50},
        {"ROBLX","Roblox", 40.20},
        {"ARM", "Arm Holdings", 120.30}
    };

    std::map<std::string, Instrument*> spotMap;

    // 1) Spot 종목 등록
    for (const auto& s : seeds) {
        Instrument* spot = new SpotStock(s.sym, s.name, s.price);
        market.addInstrument(spot);
        spotMap[s.sym] = spot;
    }

    // 2) 주요 종목 선물 (레버리지 10배, 만기 2025-12-31)
    std::vector<std::string> futSyms = {"AAPL", "TSLA", "MSFT", "NVDA", "AMZN"};
    for (const auto& sym : futSyms) {
        auto it = spotMap.find(sym);
        if (it != spotMap.end()) {
            std::string fsym = sym + std::string("F");
            std::string fname = it->second->getName() + " Future";
            Instrument* fut = new FutureContract(
                fsym, fname, it->second, 10.0, "2025-12-31");
            market.addInstrument(fut);
        }
    }
}

// 생성자
Application::Application()
    : market(),
      account(1000000.0),  // 초기 자산 100만
      orderManager(),
      riskManager(),
      currentScreen(nullptr),
      running(true)
{
    initInstruments();
    currentScreen = new MainScreen(this);
}

// 소멸자
Application::~Application() {
    delete currentScreen;
}

// Getter
Market& Application::getMarket() {
    return market;
}

Account& Application::getAccount() {
    return account;
}

OrderManager& Application::getOrderManager() {
    return orderManager;
}

// 메인 루프: 프로그램이 실행되는 동안 계속 반복
void Application::run() {
    while (running) {
        // 시장 데이터 1틱 업데이트
        market.tick();

        // 리스크 체크 (마진콜 & 선물 만기)
        riskManager.checkRisk(&account, &market, &orderManager);

        if (currentScreen) {
            currentScreen->show();
        } else {
            break;
        }
    }
}

// 화면 전환
void Application::changeScreen(Screen* next) {
    if (currentScreen != nullptr) {
        delete currentScreen;
    }
    currentScreen = next;
}

// 프로그램 종료
void Application::exit() {
    running = false;
}
