// ============================================================================
// Stock Trading Simulator - Single File Version
// B팀 구현 (2025년 12월)
// 팀원: 김다인, 김진광, 신바다(팀장), 이호준, 임찬형, 정승아
// ============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>
#include <utility>

using namespace std;

// ============================================================================
// 1. ENUMS & TYPE DEFINITIONS
// ============================================================================

// 금융 상품 타입 열거형
enum class InstrumentType {
    Spot,    // 현물 주식
    Future,  // 선물 계약
    ETF      // 커스텀 ETF
};

// ============================================================================
// 2. FORWARD DECLARATIONS
// ============================================================================

class Instrument;
class Market;
class Account;
class Position;
class OrderManager;
class Application;
class Screen;

// ============================================================================
// 3. UTILITY CLASSES
// ============================================================================

// 시간 및 날짜 관련 유틸리티 클래스
class TimeUtil {
public:
    // 현재 시스템 시간 반환 (HH:MM:SS)
    static string now() {
        using namespace chrono;
        auto tp = system_clock::now();
        time_t t = system_clock::to_time_t(tp);
        tm local{};
#ifdef _WIN32
        localtime_s(&local, &t);
#else
        local = *localtime(&t);
#endif
        char buf[12];
        strftime(buf, sizeof(buf), "%H:%M:%S", &local);
        return string(buf);
    }

    // 오늘 날짜 반환 (YY-MM-DD)
    static string todayDate() {
        using namespace chrono;
        auto tp = system_clock::now();
        time_t t = system_clock::to_time_t(tp);
        tm local{};
#ifdef _WIN32
        localtime_s(&local, &t);
#else
        local = *localtime(&t);
#endif
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &local);
        return string(buf);
    }
};

// 난수 생성 유틸리티 클래스
class RandomUtil {
public:
    // 지정된 범위 내의 랜덤 실수 반환
    static double randomRate(double min, double max) {
        static mt19937 rng(static_cast<unsigned>(
            chrono::high_resolution_clock::now().time_since_epoch().count()));
        uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }
};

// ============================================================================
// 4. MODEL CLASSES - DEFINITIONS
// ============================================================================

// 모든 금융 상품의 공통 속성과 기능을 정의하는 추상 베이스 클래스
class Instrument {
protected:
    string symbol;      // 종목 코드
    string name;        // 종목 이름
    double currentPrice;     // 현재가
    double highPrice;        // 고가
    double lowPrice;         // 저가
    double changeRate;       // 변동률 (%)
    InstrumentType type;     // 종목 타입

public:
    Instrument(const string& sym, const string& nm, double price, InstrumentType t)
        : symbol(sym), name(nm), currentPrice(price),
          highPrice(price), lowPrice(price), changeRate(0.0), type(t) {}

    virtual ~Instrument() = default;

    const string& getSymbol() const { return symbol; }
    const string& getName() const { return name; }
    double getPrice() const { return currentPrice; }
    double getHigh() const { return highPrice; }
    double getLow() const { return lowPrice; }
    double getChangeRate() const { return changeRate; }
    InstrumentType getType() const { return type; }

    virtual void updatePrice(double newPrice) {
        const double oldPrice = currentPrice;
        if (newPrice > highPrice) highPrice = newPrice;
        if (newPrice < lowPrice) lowPrice = newPrice;
        currentPrice = newPrice;
        if (fabs(oldPrice) > 1e-9) {
            changeRate = (newPrice - oldPrice) / oldPrice * 100.0;
        } else {
            changeRate = 0.0;
        }
    }

    virtual void recalcPrice() = 0;
};

// 현물 주식 클래스
class SpotStock : public Instrument {
public:
    SpotStock(const string& sym, const string& nm, double price)
        : Instrument(sym, nm, price, InstrumentType::Spot) {}

    void recalcPrice() override {}
};

// 선물 계약 클래스
class FutureContract : public Instrument {
private:
    Instrument* underlying;
    double leverage;
    string expiryDate;

public:
    FutureContract(const string& sym, const string& nm, Instrument* underlyingAsset,
                   double leverageFactor, const string& expiry)
        : Instrument(sym, nm,
                     underlyingAsset ? underlyingAsset->getPrice() * leverageFactor : 0.0,
                     InstrumentType::Future),
          underlying(underlyingAsset), leverage(leverageFactor), expiryDate(expiry) {}

    Instrument* getUnderlying() const { return underlying; }
    double getLeverage() const { return leverage; }
    const string& getExpiryDate() const { return expiryDate; }

    void recalcPrice() override {
        if (!underlying) return;
        double newPrice = underlying->getPrice() * leverage;
        updatePrice(newPrice);
    }

    bool isExpired(const string& today) const {
        return today > expiryDate;
    }
};

// 커스텀 ETF 클래스
class CustomETF : public Instrument {
private:
    vector<pair<Instrument*, double>> components;

public:
    CustomETF(const string& sym, const string& nm)
        : Instrument(sym, nm, 0.0, InstrumentType::ETF) {}

    void addComponent(Instrument* inst, double ratio) {
        if (inst == nullptr || ratio <= 0.0) {
            cout << "[Error] 유효하지 않은 종목이거나 비중입니다." << endl;
            return;
        }

        double currentTotalRatio = 0.0;
        for (const auto& comp : components) {
            currentTotalRatio += comp.second;
        }

        if (currentTotalRatio + ratio > 1.0) {
            cout << "[Error] ETF 구성 비중의 합은 100%(1.0)를 초과할 수 없습니다." << endl;
            return;
        }

        components.push_back(make_pair(inst, ratio));
        recalcPrice();
    }

    void recalcPrice() override {
        double newPrice = 0.0;
        for (const auto& comp : components) {
            Instrument* inst = comp.first;
            double ratio = comp.second;
            newPrice += inst->getPrice() * ratio;
        }
        Instrument::updatePrice(newPrice);
    }
};

// 특정 종목의 보유 내역을 나타내는 클래스
class Position {
private:
    Instrument* instrument;
    int quantity;
    double avgPrice;

public:
    Position(Instrument* inst, int qty, double price)
        : instrument(inst), quantity(qty), avgPrice(price) {}

    Instrument* getInstrument() const { return instrument; }
    int getQuantity() const { return quantity; }
    double getAvgPrice() const { return avgPrice; }

    void addQuantity(int qty, double currentPrice) {
        if (qty <= 0 || currentPrice < 0) return;
        double oldTotalCost = quantity * avgPrice;
        double newCost = qty * currentPrice;
        quantity += qty;
        avgPrice = (oldTotalCost + newCost) / quantity;
    }

    void removeQuantity(int qty) {
        if (qty <= 0) return;
        if (quantity < qty) {
            quantity = 0;
        } else {
            quantity -= qty;
        }
    }
};

// 거래 주문 기록 클래스
class Order {
private:
    string symbol;
    bool isBuy;
    int quantity;
    double price;
    string date;

public:
    Order(string sym, bool buy, int qty, double p, string d)
        : symbol(sym), isBuy(buy), quantity(qty), price(p), date(d) {}

    const string& getSymbol() const { return symbol; }
    bool getIsBuy() const { return isBuy; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    const string& getDate() const { return date; }
};

// ============================================================================
// 5. CORE CLASSES - DEFINITIONS
// ============================================================================

// 주식 시장 시뮬레이터 클래스
class Market {
private:
    vector<Instrument*> instruments;

    bool checkSymbol(const string& symbol) const {
        for (Instrument* inst : instruments) {
            if (inst->getSymbol() == symbol) return true;
        }
        return false;
    }

public:
    Market() = default;

    ~Market() {
        for (Instrument* inst : instruments) {
            delete inst;
        }
    }

    void addInstrument(Instrument* inst) {
        if (inst == nullptr) return;
        instruments.push_back(inst);
    }

    const vector<Instrument*>& getAll() const {
        return instruments;
    }

    Instrument* findBySymbol(const string& symbol) const {
        for (Instrument* inst : instruments) {
            if (inst->getSymbol() == symbol) return inst;
        }
        return nullptr;
    }

    void tick() {
        // Spot 가격 랜덤 변동
        for (Instrument* inst : instruments) {
            if (inst->getType() == InstrumentType::Spot) {
                double rate = RandomUtil::randomRate(-0.03, 0.03);
                double newPrice = inst->getPrice() * (1.0 + rate);
                if (newPrice < 0.01) newPrice = 0.01;
                inst->updatePrice(newPrice);
            }
        }

        // 파생상품 가격 재계산
        for (Instrument* inst : instruments) {
            if (inst->getType() != InstrumentType::Spot) {
                inst->recalcPrice();
            }
        }
    }

    vector<Instrument*> getTopMovers(int n) const {
        vector<Instrument*> copy = instruments;
        sort(copy.begin(), copy.end(),
             [](const Instrument* a, const Instrument* b) {
                 return a->getChangeRate() > b->getChangeRate();
             });
        if (static_cast<int>(copy.size()) > n) copy.resize(n);
        return copy;
    }
};

// 사용자 계좌 관리 클래스
class Account {
private:
    double cashBalance;
    double initialEquity;
    vector<Position*> positions;

    Position* findPosition(string symbol) {
        for (Position* pos : positions) {
            if (pos->getInstrument()->getSymbol() == symbol) {
                return pos;
            }
        }
        return nullptr;
    }

public:
    Account(double startCash)
        : cashBalance(startCash), initialEquity(startCash) {}

    ~Account() {
        for (Position* pos : positions) {
            delete pos;
        }
        positions.clear();
    }

    bool buy(string symbol, int quantity, double price, Market* market) {
        if (quantity <= 0 || price <= 0) return false;
        double totalCost = price * quantity;

        if (cashBalance < totalCost) return false;

        Position* pos = findPosition(symbol);

        if (pos != nullptr) {
            pos->addQuantity(quantity, price);
        } else {
            Instrument* inst = market->findBySymbol(symbol);
            if (inst == nullptr) return false;

            Position* newPos = new Position(inst, quantity, price);
            positions.push_back(newPos);
        }

        cashBalance -= totalCost;
        return true;
    }

    bool sell(string symbol, int quantity, double price) {
        if (quantity <= 0 || price <= 0) return false;

        Position* pos = findPosition(symbol);

        if (pos == nullptr || pos->getQuantity() < quantity) {
            return false;
        }

        double totalSaleAmount = price * quantity;
        cashBalance += totalSaleAmount;

        pos->removeQuantity(quantity);

        if (pos->getQuantity() == 0) {
            for (auto it = positions.begin(); it != positions.end(); it++) {
                if (*it == pos) {
                    positions.erase(it);
                    delete pos;
                    break;
                }
            }
        }

        return true;
    }

    double getTotalEquity() const {
        double equity = cashBalance;
        for (const Position* pos : positions) {
            double marketValue = pos->getInstrument()->getPrice() * pos->getQuantity();
            equity += marketValue;
        }
        return equity;
    }

    double getCashBalance() const { return cashBalance; }
    double getInitialEquity() const { return initialEquity; }
    const vector<Position*>& getPositions() const { return positions; }

    vector<Position*> getPositionsByType(InstrumentType type) const {
        vector<Position*> result;
        for (Position* pos : positions) {
            if (pos->getInstrument()->getType() == type) {
                result.push_back(pos);
            }
        }
        return result;
    }
};

// 주문 관리 클래스
class OrderManager {
private:
    vector<Order> orderHistory;

public:
    OrderManager() = default;

    bool executeOrder(Account* acc, Market* mkt, string symbol, int qty, bool isBuy) {
        if (acc == nullptr || mkt == nullptr) return false;

        Instrument* inst = mkt->findBySymbol(symbol);
        if (inst == nullptr) return false;

        double currentPrice = inst->getPrice();
        bool success = false;

        if (isBuy) {
            success = acc->buy(symbol, qty, currentPrice, mkt);
        } else {
            success = acc->sell(symbol, qty, currentPrice);
        }

        if (success) {
            string timestamp = TimeUtil::now();
            Order newOrder(symbol, isBuy, qty, currentPrice, timestamp);
            orderHistory.push_back(newOrder);
        }

        return success;
    }

    const vector<Order>& getOrderHistory() const {
        return orderHistory;
    }
};

// 리스크 관리 클래스
class RiskManager {
public:
    RiskManager() = default;

    void checkRisk(Account* acc, Market* mkt, OrderManager* om) {
        if (acc == nullptr || mkt == nullptr || om == nullptr) return;

        // 1. 마진콜 체크
        double currentEquity = acc->getTotalEquity();
        double initialEquity = acc->getInitialEquity();

        if (currentEquity < initialEquity * 0.2) {
            cout << "\n자산이 20% 미만입니다. 강제 청산을 진행합니다.\n" << endl;

            vector<Position*> currentPositions = acc->getPositions();

            for (Position* pos : currentPositions) {
                Instrument* inst = pos->getInstrument();
                string symbol = inst->getSymbol();
                int qty = pos->getQuantity();

                bool result = om->executeOrder(acc, mkt, symbol, qty, false);

                if (result) {
                    cout << "[강제청산] " << symbol << " (" << qty << "주) 매도 완료." << endl;
                }
            }

            return;
        }

        // 2. 선물 만기 체크
        string today = TimeUtil::todayDate();

        vector<string> expiredSymbols;

        const vector<Position*>& positions = acc->getPositions();
        for (Position* pos : positions) {
            Instrument* inst = pos->getInstrument();

            FutureContract* future = dynamic_cast<FutureContract*>(inst);

            if (future != nullptr) {
                if (future->isExpired(today)) {
                    expiredSymbols.push_back(inst->getSymbol());
                }
            }
        }

        for (const string& symbol : expiredSymbols) {
            Position* pos = nullptr;
            for (Position* p : acc->getPositions()) {
                if (p->getInstrument()->getSymbol() == symbol) {
                    pos = p;
                    break;
                }
            }

            if (pos != nullptr) {
                int qty = pos->getQuantity();

                bool result = om->executeOrder(acc, mkt, symbol, qty, false);

                if (result) {
                    cout << "\n[알림] 선물 만기 도래: " << symbol << " (" << qty << "주) 자동 청산되었습니다." << endl;
                }
            }
        }
    }
};

// ============================================================================
// 6. UI CLASSES - DEFINITIONS
// ============================================================================

// 모든 화면의 추상 베이스 클래스
class Screen {
protected:
    Application* app;

    void clear() {
        for (int i = 0; i < 2; i++) {
            cout << endl;
        }
    }

public:
    Screen(Application* application) : app(application) {}
    virtual ~Screen() = default;
    virtual void show() = 0;
};

// 전방 선언된 Screen 타입들
class MainScreen;
class AllStocksScreen;
class HoldingsScreen;
class TradeScreen;
class CustomETFScreen;

// ============================================================================
// 7. APPLICATION CLASS
// ============================================================================

class Application {
private:
    Market market;
    Account account;
    OrderManager orderManager;
    RiskManager riskManager;
    Screen* currentScreen;
    bool running;

    void initInstruments() {
        struct Seed { const char* sym; const char* name; double price; };

        vector<Seed> seeds = {
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

        map<string, Instrument*> spotMap;

        for (const auto& s : seeds) {
            Instrument* spot = new SpotStock(s.sym, s.name, s.price);
            market.addInstrument(spot);
            spotMap[s.sym] = spot;
        }

        vector<string> futSyms = {"AAPL", "TSLA", "MSFT", "NVDA", "AMZN"};
        for (const auto& sym : futSyms) {
            auto it = spotMap.find(sym);
            if (it != spotMap.end()) {
                string fsym = sym + string("F");
                string fname = it->second->getName() + " Future";
                Instrument* fut = new FutureContract(
                    fsym, fname, it->second, 10.0, "2025-12-31");
                market.addInstrument(fut);
            }
        }
    }

public:
    Application();
    ~Application();

    Market& getMarket() { return market; }
    Account& getAccount() { return account; }
    OrderManager& getOrderManager() { return orderManager; }

    void run();
    void changeScreen(Screen* next);
    void exit() { running = false; }
};

// ============================================================================
// 8. SCREEN IMPLEMENTATIONS
// ============================================================================

// MainScreen
class MainScreen : public Screen {
public:
    MainScreen(Application* app) : Screen(app) {}

    void show() override;
};

// AllStocksScreen
class AllStocksScreen : public Screen {
public:
    AllStocksScreen(Application* app) : Screen(app) {}

    void show() override {
        clear();
        cout << "========== 전체 종목 조회(All Stocks) ==========" << endl;

        Market& market = app->getMarket();
        const vector<Instrument*>& allInstruments = market.getAll();

        if (allInstruments.empty()) {
            cout << "등록된 종목이 없습니다." << endl;
        } else {
            cout << left << setw(12) << "종목 코드"
                 << setw(30) << "종목 이름"
                 << right << setw(12) << "현재가"
                 << setw(12) << "고가"
                 << setw(12) << "저가"
                 << setw(12) << "변동률" << endl;
            cout << string(90, '-') << endl;

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

        cout << "\n-----------------------------------------" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            app->changeScreen(new MainScreen(app));
        }
    }
};

// HoldingsScreen
class HoldingsScreen : public Screen {
private:
    void printPositionTable(const string& title, const vector<Position*>& positions) {
        cout << endl << title << endl;

        if (positions.empty()) {
            cout << "  (보유 종목 없음)" << endl;
            return;
        }

        cout << left << setw(10) << "Symbol"
             << right << setw(8) << "Qty"
             << setw(12) << "Avg Price"
             << setw(12) << "Cur Price"
             << setw(12) << "P&L(%)" << endl;

        for (const auto& pos : positions) {
            auto inst = pos->getInstrument();
            double currentPrice = inst->getPrice();
            double avgPrice = pos->getAvgPrice();
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

public:
    HoldingsScreen(Application* app) : Screen(app) {}

    void show() override {
        clear();
        cout << "========== 보유 종목(Holdings) ==========" << endl;

        Account& account = app->getAccount();

        vector<Position*> spotPositions = account.getPositionsByType(InstrumentType::Spot);
        printPositionTable("[현물 주식]", spotPositions);

        vector<Position*> futurePositions = account.getPositionsByType(InstrumentType::Future);
        printPositionTable("[선물]", futurePositions);

        vector<Position*> etfPositions = account.getPositionsByType(InstrumentType::ETF);
        printPositionTable("[커스텀 ETF]", etfPositions);

        cout << "\n-----------------------------------------" << endl;
        cout << "0. 뒤로가기" << endl;
        cout << "선택: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            app->changeScreen(new MainScreen(app));
        }
    }
};

// TradeScreen
class TradeScreen : public Screen {
private:
    bool isBuy;

    template <typename T>
    T getValidInput(const string& prompt, T minVal, T maxVal = numeric_limits<T>::max()) {
        T value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                if (value >= minVal && value <= maxVal) {
                    return value;
                } else {
                    cout << "입력 범위를 벗어났습니다. 다시 입력해주세요.\n";
                }
            } else {
                cout << "잘못된 입력입니다. (숫자만 입력 가능)\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    string getValidSymbolInput(const string& prompt) {
        string symbol;
        cout << prompt;
        cin >> symbol;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return symbol;
    }

public:
    TradeScreen(Application* app, bool buyOrSell) : Screen(app), isBuy(buyOrSell) {}

    void show() override {
        cout << "\n======================================================\n";
        cout << "------------ " << (isBuy ? "매수(BUY)" : "매도(SELL)") << " 주문 화면 ------------\n";

        cout << "거래 유형을 선택하세요:\n";
        cout << "1. 현물 / 2. 선물 / 3. ETF 거래\n";
        int typeChoice = getValidInput<int>("선택 (1~3): ", 1, 3);

        string symbol = getValidSymbolInput("종목 코드 입력: ");
        int quantity = getValidInput<int>("수량 입력 (1 이상): ", 1);

        cout << "\n[주문 실행: " << (isBuy ? "매수" : "매도") << " " << symbol << " (" << quantity << "개)]\n";

        bool success = app->getOrderManager().executeOrder(
            &app->getAccount(), &app->getMarket(), symbol, quantity, isBuy);

        if (success) {
            cout << "주문 성공 [" << symbol << " " << quantity << "개 " << (isBuy ? "매수" : "매도") << " 체결]\n";
        } else {
            cout << "주문 실패\n (원인: 잔고 부족, 보유 수량 부족, 종목 미등록 등)\n";
        }

        cout << "\n------------------------------------------------------\n";
        cout << "메인 화면으로 돌아가려면 엔터를 누르세요...\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        app->changeScreen(new MainScreen(app));
    }
};

// CustomETFScreen
class CustomETFScreen : public Screen {
private:
    int readInt(const string& prompt) {
        while (true) {
            cout << prompt;
            int value;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[오류] 정수를 입력하세요.\n";
        }
    }

    double readDouble(const string& prompt) {
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

public:
    CustomETFScreen(Application* app) : Screen(app) {}

    void show() override {
        cout << "====================[ 커스텀 ETF 구성/수정 ]====================\n\n";

        // 1. ETF 기본 정보 입력
        string etfSymbol;
        cout << "생성할 커스텀 ETF의 심볼을 입력하세요 (예: CUST1): ";
        cin >> etfSymbol;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string etfName;
        cout << "커스텀 ETF의 이름을 입력하세요: ";
        getline(cin, etfName);

        cout << "\n--- 구성 종목 정보 입력 ---\n";

        // 2. 구성 종목 개수 입력
        int count = 0;
        while (count <= 0) {
            count = readInt("구성할 종목 개수를 입력하세요 (1 이상): ");
            if (count <= 0) {
                cout << "[오류] 최소 1개 이상의 종목이 필요합니다.\n";
            }
        }

        // CustomETF 생성
        CustomETF* customETF = new CustomETF(etfSymbol, etfName);

        double totalWeight = 0.0;

        // 3. 각 종목 입력
        for (int i = 0; i < count; ++i) {
            cout << "\n[" << (i + 1) << " / " << count << "] 번째 종목\n";

            string symbol;
            cout << "종목 심볼을 입력하세요 (예: AAPL, TSLA 등): ";
            cin >> symbol;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            double weight = -1.0;
            while (weight <= 0.0) {
                weight = readDouble("비중(%)을 입력하세요 (0보다 큰 값): ");
                if (weight <= 0.0) {
                    cout << "[오류] 비중은 0보다 커야 합니다.\n";
                }
            }

            // Market에서 종목 찾기
            Instrument* inst = app->getMarket().findBySymbol(symbol);
            if (inst == nullptr) {
                cout << "[경고] " << symbol << "은(는) 존재하지 않는 종목입니다. 건너뜁니다.\n";
                continue;
            }

            // ETF에 추가 (비중은 %를 소수로 변환)
            customETF->addComponent(inst, weight / 100.0);
            totalWeight += weight;
        }

        // 4. 결과 요약
        cout << "\n====================[ 커스텀 ETF 요약 ]====================\n";
        cout << "심볼 : " << etfSymbol << "\n";
        cout << "이름 : " << etfName  << "\n";
        cout << "비중 합계 : " << totalWeight << "%\n";

        if (fabs(totalWeight - 100.0) > 1e-6) {
            cout << "[경고] 비중 합계가 100%가 아닙니다.\n";
        }

        // 5. Market에 등록
        app->getMarket().addInstrument(customETF);
        cout << "\n[완료] 커스텀 ETF가 생성되어 시장에 등록되었습니다!\n";

        cout << "\n엔터를 누르면 이전 화면으로 돌아갑니다...";
        cin.get();
        app->changeScreen(new MainScreen(app));
    }
};

// MainScreen 구현
void MainScreen::show() {
    clear();

    Market& market = app->getMarket();
    Account& account = app->getAccount();

    cout << "## [현재 시간] " << TimeUtil::now() << endl;

    double equity = account.getTotalEquity();
    cout << "==== Stock Market ==== 현재 잔고: " << fixed << setprecision(0) << equity << endl;

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

    cout << "--------------------------------" << endl;
    cout << "1. 전체 종목 조회" << endl;
    cout << "2. 보유 종목 조회" << endl;
    cout << "3. 매수" << endl;
    cout << "4. 매도" << endl;
    cout << "5. ETF 커스텀" << endl;
    cout << "6. 종료" << endl;
    cout << "---" << endl;
    cout << "선택: ";

    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    switch (choice) {
    case 1:
        app->changeScreen(new AllStocksScreen(app));
        break;
    case 2:
        app->changeScreen(new HoldingsScreen(app));
        break;
    case 3:
        app->changeScreen(new TradeScreen(app, true));
        break;
    case 4:
        app->changeScreen(new TradeScreen(app, false));
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

// ============================================================================
// 9. APPLICATION IMPLEMENTATION
// ============================================================================

Application::Application()
    : market(), account(1000000.0), orderManager(), riskManager(),
      currentScreen(nullptr), running(true)
{
    initInstruments();
    currentScreen = new MainScreen(this);
}

Application::~Application() {
    delete currentScreen;
}

void Application::run() {
    while (running) {
        market.tick();
        riskManager.checkRisk(&account, &market, &orderManager);

        if (currentScreen) {
            currentScreen->show();
        } else {
            break;
        }
    }
}

void Application::changeScreen(Screen* next) {
    if (currentScreen != nullptr) {
        delete currentScreen;
    }
    currentScreen = next;
}

// ============================================================================
// 10. MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== Stock Trading Simulator ===" << endl;
    cout << "프로그램을 시작합니다..." << endl << endl;

    Application app;
    app.run();

    cout << "\n프로그램을 종료합니다. 감사합니다!" << endl;
    return 0;
}
