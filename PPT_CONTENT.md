# B팀 발표 PPT 내용

**과목:** 객체지향프로그래밍
**발표일:** 2025년 12월 9일
**팀:** B팀 (팀장: 신바다)

---

## 슬라이드 1. 표지

**주식 거래 시뮬레이터**
Stock Trading Simulator

- 과목: 객체지향프로그래밍
- B팀: 김다인, 김진광, 신바다(팀장), 이호준, 임찬형, 정승아
- 발표일: 2025년 12월 9일

---

## 슬라이드 2. 프로젝트 개요 + 고유 기능

### 프로젝트 개요
- A팀이 설계한 명세서를 기반으로 B팀이 구현
- 콘솔 기반 주식 거래 시뮬레이션 프로그램

### 주요 기능
1. **다양한 자산 거래**: 현물 주식, 선물 계약, 커스텀 ETF
2. **실시간 가격 시뮬레이션**: 랜덤 가격 변동, 고가/저가/변동률 추적
3. **리스크 관리 시스템**: 80% 손실 시 선물 강제청산, 만기일 자동청산

### 고유 기능 (A팀 설계)
- **커스텀 ETF**: 사용자가 직접 종목과 비율을 설정하여 포트폴리오를 묶음 단위로 거래

---

## 슬라이드 3. 클래스 다이어그램

### 3계층 아키텍처

```
┌─────────────────────────────────────────────────────────┐
│                      UI 계층                             │
│  Screen(추상) ← MainScreen, AllStocksScreen,            │
│                 HoldingsScreen, TradeScreen,            │
│                 CustomETFScreen                         │
├─────────────────────────────────────────────────────────┤
│                    Domain 계층                           │
│  Instrument(추상) ← SpotStock, FutureContract, CustomETF│
│  Market, Account, Position, Order                       │
│  OrderManager, RiskManager                              │
├─────────────────────────────────────────────────────────┤
│                   Utility 계층                           │
│  TimeUtil, RandomUtil                                   │
└─────────────────────────────────────────────────────────┘
```

### 핵심 관계
- **상속**: Instrument ← SpotStock, FutureContract, CustomETF
- **상속**: Screen ← 5개 화면 클래스
- **컴포지션**: Market → Instrument*, Account → Position*

---

## 슬라이드 4. 클래스 상세 설명 (1) - Domain 계층

### Instrument (추상 클래스)
- 모든 금융상품의 베이스 클래스
- 필드: symbol, name, currentPrice, highPrice, lowPrice, changeRate, type
- 메서드: updatePrice(), recalcPrice() (순수 가상)

### SpotStock (현물 주식)
- Instrument 상속, 추가 필드 없음

### FutureContract (선물)
- Instrument 상속
- 추가 필드: underlying(기초자산), leverage(레버리지), expiryDate(만기일)
- 메서드: isExpired(), recalcPrice()

### CustomETF (커스텀 ETF)
- Instrument 상속
- 추가 필드: components (종목 + 비율 리스트)
- 메서드: addComponent(), recalcPrice()

---

## 슬라이드 5. 클래스 상세 설명 (2) - Core & UI 계층

### Market
- 모든 종목 관리 (현물/선물/ETF 통합 리스트)
- 메서드: addInstrument(), findBySymbol(), tick(), getTopMovers()

### Account
- 현금 잔고 + 보유 포지션 관리
- 메서드: buy(), sell(), getTotalEquity(), getPositionsByType()

### OrderManager
- 매수/매도 주문 처리, 거래 내역 저장

### RiskManager
- 80% 손실 시 선물 강제청산
- 만기일 지난 선물 자동청산

### Screen (추상 클래스)
- 모든 화면의 베이스, show() 순수 가상 함수
- 5개 화면: MainScreen, AllStocksScreen, HoldingsScreen, TradeScreen, CustomETFScreen

---

## 슬라이드 6. A팀 설계 요약 (1) - 전체 흐름

### 프로그램 실행 흐름

```
main() 시작
    ↓
Application 생성 → Market 초기화 (50개 종목)
    ↓
┌─────── 메인 루프 ───────┐
│  1. Market.tick()       │  ← 가격 변동
│  2. RiskManager 체크    │  ← 강제청산/만기청산
│  3. Screen.show()       │  ← 화면 출력 및 입력
└─────────────────────────┘
```

### 초기 설정
- 초기 자본금: 1,000,000원
- 현물 주식: 50개 하드코딩
- 선물: 5개 (AAPL, TSLA, MSFT, NVDA, AMZN)
- 선물 만기일: 2025-12-31

---

## 슬라이드 7. A팀 설계 요약 (2) - 메인 화면 & 메뉴

### 메인 화면 출력
```
## [현재 시각] 14:32:12
==== Stock Market ==== 현재 잔고: 1,000,000
종목명    현재가    고가      저가      변동률
AAPL      186.20    221.12    121.33    +10.91%
TSLA      226.82    331.11    13.12     +8.72%
...
--------------------------------
1. 전체 종목 조회
2. 보유 종목 조회
3. 매수
4. 매도
5. ETF 커스텀
6. 종료
```

---

## 슬라이드 8. A팀 설계 요약 (3) - 주요 기능

### 메뉴 1: 전체 종목 조회
- Market.getAll() → 50개 종목 리스트 출력

### 메뉴 2: 보유 종목 조회
- 현물/선물/ETF 타입별로 섹션 분리 출력
- 각 포지션의 수량, 평단가, 현재가, 손익률 표시

### 메뉴 3-4: 매수/매도
- 거래 유형 선택 (현물/선물/ETF)
- 종목 코드, 수량 입력 → 주문 실행

### 메뉴 5: ETF 커스텀
- 사용자가 종목과 비율 입력
- CustomETF 생성 → Market에 등록

### 메뉴 6: 종료

---

## 슬라이드 9. 프로그램 데모

### 실행 방법
```bash
g++ -std=c++11 main.cpp -o stock_trading_sim
./stock_trading_sim
```

### 데모 시나리오
1. 메인 화면 확인 (상승률 상위 5개 종목)
2. 전체 종목 조회
3. 현물 매수 (예: AAPL 10주)
4. 보유 종목 조회
5. 커스텀 ETF 생성
6. ETF 매수
7. 손익 확인

---

## 슬라이드 10. A팀 설계 리뷰 - 좋았던 점

### 1. 다형성을 활용한 확장성 있는 설계
- 주식, 선물, ETF를 **Instrument 하나로 통합 관리**
- Market, Account에서 종류에 구애받지 않고 하나의 리스트로 처리

### 2. FutureContract의 유연한 기초자산 설계
- 기초자산 타입을 SpotStock이 아닌 **Instrument로 정의**
- "선물지수를 따르는 ETF", "ETF에 대한 선물" 등 확장 가능

### 3. 명확한 클래스 분리
- Screen 추상 클래스를 상속받아 다양한 화면 구현
- Application이 현재 화면을 관리하는 구조로 화면 전환 로직이 깔끔

### 4. 시퀀스 다이어그램 제공
- "기능이 있다"가 아니라 **기능이 어떤 순서로 동작하는지** 그림으로 제공
- 전체 시스템 흐름을 빠르게 파악 가능, 인터랙션이 직관적

---

## 슬라이드 11. A팀 설계 리뷰 - 아쉬웠던 점

### 1. 클래스 다이어그램과 시퀀스 다이어그램 불일치
- 시퀀스에는 있는데 클래스 다이어그램에 없는 메서드 **8개 발견**

| 클래스 | 누락 메서드 |
|--------|------------|
| Market | tick(), getTopMovers(), initInstruments() |
| Account | getPositionsByType() |
| CustomETF | addComponent(), recalcPrice() |
| Application | changeScreen() |
| FutureContract | isExpired() |

### 2. FutureContract 필드 누락
- 클래스 다이어그램에 leverage, underlying 필드가 없었음

### 3. Account 클래스의 책임 과다
- Account가 buy/sell을 직접 수행하는 구조
- OrderManager가 매매 로직 담당, Account는 데이터 관리만 했으면 더 유연했을 것

### 4. 예외 처리 흐름 부재
- ETF 생성 시 비중 합계 100% 초과, 잘못된 심볼 입력 등의 예외 상황 흐름이 다이어그램에 없었음

### 5. 종목 간 상관관계 미반영
- RandomUtil이 모든 종목에 독립적인 난수만 생성
- 실제로는 나스닥 상승 시 기술주가 같이 오르는 등 상관관계가 있음

### 6. 촉박한 일정
- 설계 받고 1-2주 내 구현은 생각보다 많은 양이었음

---

## 슬라이드 12. A팀 설계 리뷰 - 수정/보완한 부분

### B팀이 추가 구현한 내용

#### 1. 누락된 8개 메서드 전부 구현
```cpp
// Market
void tick();
vector<Instrument*> getTopMovers(int n);
void initInstruments();

// Account
vector<Position*> getPositionsByType(InstrumentType type);

// CustomETF
void addComponent(Instrument* inst, double ratio);
void recalcPrice();

// Application
void changeScreen(Screen* next);

// FutureContract
bool isExpired(string today);
```

#### 2. FutureContract 필드 보완
```cpp
class FutureContract : public Instrument {
    Instrument* underlying;  // 기초자산 추가
    double leverage;         // 레버리지 추가
    string expiryDate;
};
```

#### 3. Component 구조체 간략화
- 설계에서는 Component 구조체를 별도로 정의
- B팀은 STL pair를 활용하여 간략하게 구현
```cpp
vector<pair<Instrument*, double>> components;
```

#### 4. Position 평단가 계산 로직 구현
```cpp
void Position::addQuantity(int qty, double price) {
    avgPrice = (avgPrice * quantity + price * qty) / (quantity + qty);
    quantity += qty;
}
```

---

## 슬라이드 13. 팀원들이 배운 점

### 1. OOP 개념의 실제 필요성 체감
- Instrument 추상 클래스를 상속해 현물/선물/ETF 구현
- updatePrice(), recalcPrice() 호출 시 각 타입에 맞는 동작 수행 → **다형성의 유용함 체감**

### 2. 디자인 패턴의 활용
- **Composite 패턴**: CustomETF가 다른 Instrument를 포함하면서 동시에 Instrument로 취급
- **State 패턴**: Application이 Screen 포인터로 화면 전환 → if-else 없이 상태 관리

### 3. 설계의 중요성
- "알아서 하세요"보다 "이 코드를 그대로 치세요" 수준으로 꼼꼼하게 다이어그램을 그려야 함
- 견고한 구조와 명확한 요구사항이 먼저 정의되지 않으면 프로젝트가 부실해짐

### 4. 협업을 위한 모듈화
- 헤더/소스 파일 분리, 인터페이스 통일로 개발 비용 최소화
- 도메인 기반 역할 분담으로 각 팀원이 본인 영역에 집중 가능

### 5. 디버깅 경험
- 선물 만기 체크에서 날짜 포맷 불일치로 매입 즉시 청산되는 버그 경험
- 사소한 부분의 정확성이 로직에 미치는 영향이 큼 → 데이터 타입/포맷 검증 습관

### 6. shared_ptr을 통한 안전한 참조 관리
- 동시에 참조해야 하는 데이터를 shared_ptr로 관리
- 참조가 끊길 때까지 객체가 생존하도록 설계

---

## 슬라이드 14. Q&A / 감사합니다

### B팀 팀원
김다인, 김진광, 신바다(팀장), 이호준, 임찬형, 정승아

### 감사합니다!

---

**문서 끝**
