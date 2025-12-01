# UML 클래스 다이어그램 명세 (A팀)

**출처:** A팀 설계 문서
**작성일:** 2025년 11월
**정리일:** 2025년 12월 1일

---

## 1. Application (메인 컨트롤러)

Application 클래스는 전체 시스템의 진입점이자 중앙 컨트롤러다.

**private 멤버:**
- `Market& market`
- `Account& account`
- `OrderManager& orderManager`
- `RiskManager& riskManager`
- `MainScreen* mainScreen`
- `Screen* currentScreen`

**public 메서드:**
- `Application()` - 기본 생성자
- `void run()` - 메인 루프 실행
- `Market& getMarket()` - Market 참조 반환
- `Account& getAccount()` - Account 참조 반환
- `OrderManager& getOrderManager()` - OrderManager 참조 반환
- `void changeScreen(Screen* next)` - 화면 전환
- `void exit()` - 프로그램 종료

---

## 2. Screen (인터페이스)

Screen은 abstract 인터페이스로, 모든 화면 클래스의 부모다.

**pure virtual 메서드:**
- `void display(Application* app)` - 화면 출력 (순수 가상)
- `void exit()` - 화면 종료 (순수 가상)

---

## 3. Screen 구현 클래스들

### 3.1 MainScreen
- **상속:** Screen
- **private 멤버:** `Application* app`
- **public 메서드:**
  - `void display(Application* app) override`
  - `void exit() override`

### 3.2 AllStocksScreen
- **상속:** Screen
- **private 멤버:** `Application* app`
- **public 메서드:**
  - `void display(Application* app) override`
  - `void exit() override`

### 3.3 HoldingsScreen
- **상속:** Screen
- **private 멤버:** `Application* app`
- **public 메서드:**
  - `void display(Application* app) override`
  - `void exit() override`

### 3.4 TradeScreen
- **상속:** Screen
- **private 멤버:**
  - `Application* app`
  - `bool isBuy` - true: 매수, false: 매도
- **public 메서드:**
  - `void display(Application* app) override`
  - `void exit() override`

### 3.5 CustomETFScreen
- **상속:** Screen
- **private 멤버:** `Application* app`
- **public 메서드:**
  - `void display(Application* app) override`
  - `void exit() override`

---

## 4. Market (시장 데이터 관리)

Market 클래스는 모든 금융상품을 관리한다.

**private 멤버:**
- `vector<Instrument*> instruments`

**public 메서드:**
- `vector<Instrument*> getAll() const` - 모든 종목 반환
- `Instrument* findBySymbol(string symbol)` - 심볼로 종목 검색
- `void addInstrument(Instrument* inst)` - 상품 추가
- `void simulatePriceChange()` - 가격 시뮬레이션

---

## 5. Account (계좌 관리)

Account 클래스는 사용자 계좌와 보유 포지션을 관리한다.

**private 멤버:**
- `double balance` - 현금 잔고
- `vector<Position*> positions` - 보유 포지션 목록
- `double initialBalance` - 초기 자산

**public 메서드:**
- `double getBalance() const` - 잔고 조회
- `void deposit(double amount)` - 입금
- `bool withdraw(double amount)` - 출금 (잔액 부족시 false)
- `void addPosition(Position* pos)` - 포지션 추가
- `vector<Position*> getPositions() const` - 포지션 목록 반환
- `vector<Position*> getPositionsByType(InstrumentType type) const` - 타입별 포지션 조회
- `double getInitialBalance() const` - 초기 자산 조회

---

## 6. OrderManager (주문 처리)

OrderManager 클래스는 매수/매도 주문을 처리하고 거래 내역을 관리한다.

**private 멤버:**
- `Account& account`
- `Market& market`
- `vector<Order> history` - 거래 내역

**public 메서드:**
- `OrderManager(Account& acc, Market& mk)` - 생성자
- `bool placeMarketOrder(string symbol, InstrumentType type, int quantity, OrderSide side)` - 주문 실행
- `vector<Order> getHistory() const` - 거래 내역 조회

---

## 7. RiskManager (리스크 관리)

RiskManager 클래스는 선물 강제청산과 만기일 체크를 담당한다.

**private 멤버:**
- `Account& account`
- `Market& market`
- `OrderManager& orderManager`
- `double maxLossRatio` - 최대 손실률

**public 메서드:**
- `RiskManager(Account& acc, Market& mk, OrderManager& om, double maxLossRatio)` - 생성자
- `void checkForcedLiquidation()` - 강제청산 체크 (초기 자산 대비 80% 손실시 선물 강제청산)
- `void checkFuturesExpiry(string today)` - 만기일 체크 (만기일 지난 선물 강제청산)

---

## 8. Instrument (추상 클래스 - 금융상품 기본)

Instrument는 abstract 클래스로 모든 금융상품의 부모다.

**private 멤버:**
- `string symbol` - 심볼
- `string name` - 이름
- `double currentPrice` - 현재가
- `double highPrice` - 고가
- `double lowPrice` - 저가
- `double changeRate` - 변동률
- `InstrumentType type` - 종목 타입

**public 메서드:**
- `Instrument(string sym, string nm, double price, InstrumentType t)` - 생성자
- `virtual ~Instrument()` - 가상 소멸자
- `string getSymbol() const`
- `string getName() const`
- `double getPrice() const`
- `double getHigh() const`
- `double getLow() const`
- `double getChangeRate() const`
- `InstrumentType getType() const`
- `void updatePrice(double newPrice)` - 가격 업데이트
- `virtual void recalcPrice()` - 가격 재계산 (순수 가상)

---

## 9. Stock (현물 주식)

Stock 클래스는 Instrument를 상속받는다.

**특징:**
- 추가 멤버 없음
- Instrument의 기능을 그대로 사용
- 생성자에서 InstrumentType을 Spot으로 설정

---

## 10. Futures (선물)

Futures 클래스는 Instrument를 상속받는다.

**private 멤버:**
- `string expiryDate` - 만기일 (기본값 "25-12-31")

**public 메서드:**
- `string getExpiryDate() const` - 만기일 조회

**특징:**
- 생성자에서 InstrumentType을 Future로 설정

---

## 11. CustomETF (사용자 정의 ETF)

CustomETF 클래스는 Instrument를 상속받는다.

**private 멤버:**
- `vector<Component> components` - 구성 종목 목록

**public 메서드:**
- `void addComponent(Instrument* inst, double weight)` - 구성요소 추가
- `void recalcPrice() override` - 가격 재계산 (구성 종목들의 가중 평균으로 ETF 가격 계산)

**특징:**
- 생성자에서 InstrumentType을 ETF로 설정

---

## 12. Component (ETF 구성요소)

Component 클래스는 CustomETF의 구성 종목과 비율을 나타낸다.

**public 멤버:**
- `Instrument* instrument` - 종목 포인터
- `double weight` - 비율 (0.0 ~ 1.0)

---

## 13. Position (보유 포지션)

Position 클래스는 보유 중인 금융상품 정보를 나타낸다.

**public 멤버:**
- `Instrument* instrument` - 종목 포인터
- `int quantity` - 보유 수량
- `double avgPrice` - 평균 매입가

---

## 14. Order (거래 내역)

Order 클래스는 거래 기록을 나타낸다.

**멤버:**
- `string symbol` - 종목 심볼
- `InstrumentType type` - 종목 타입
- `OrderSide side` - 매수/매도 구분
- `int quantity` - 수량
- `double price` - 가격
- `string timestamp` - 거래 시각

---

## 15. Enum 타입들

### InstrumentType
```cpp
enum class InstrumentType {
    Spot,    // 현물
    Future,  // 선물
    ETF      // 커스텀 ETF
};
```

### OrderSide
```cpp
enum class OrderSide {
    Buy,   // 매수
    Sell   // 매도
};
```

---

## 클래스 관계 요약

### Composition (소유 관계)
- Application → Market, Account, OrderManager, RiskManager
- OrderManager → Order 리스트
- Market → Instrument 포인터 리스트
- Account → Position 포인터 리스트
- CustomETF → Component 리스트

### Association (참조 관계)
- Application → Screen들
- OrderManager → Account, Market
- RiskManager → Account, Market, OrderManager
- Component → Instrument
- Position → Instrument

### Inheritance (상속 관계)
- Instrument ← Stock, Futures, CustomETF
- Screen ← MainScreen, AllStocksScreen, HoldingsScreen, TradeScreen, CustomETFScreen

---

## 핵심 비즈니스 로직

1. **선물(Futures)**
   - 만기일(expiryDate) 속성, 기본값 "25-12-31"
   - 초기 자산 대비 80% 손실시 RiskManager에 의해 강제청산
   - 만기일이 지나면 RiskManager에 의해 강제청산

2. **CustomETF**
   - 사용자가 선택한 종목들과 비율로 구성
   - 구성 종목 가격 변동시 recalcPrice()로 ETF 가격이 재계산됨

3. **Market**
   - 현물/선물/ETF 모두 동일한 instruments 리스트에 저장
   - 주식 종목은 약 50개를 하드코딩으로 초기화

---

**문서 끝**
