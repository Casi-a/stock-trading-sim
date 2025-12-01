# 전체 흐름 시퀀스 다이어그램 (A팀)

**출처:** A팀 설계 문서
**작성일:** 2025년 11월
**정리일:** 2025년 12월 1일

---

## 참여 객체

- User
- Application
- MainScreen
- Market
- Account
- RiskManager
- TimeUtil
- OrderManager

---

## 전체 흐름

프로그램 초기화 → 메인 루프 (1틱/1분마다 반복)

---

## 1단계: 프로그램 초기화

```
User → 프로그램 실행 (main())
  ↓
Application 생성
  ↓
Application → Market.initInstruments()
  ← "종목 초기화 완료"
  ↓
Application → MainScreen 생성 및 설정
```

---

## 2단계: 메인 루프 (1회 또는 1틱/1분마다 반복)

### loop 시작

**2-1. 시간 및 잔고 조회**
```
MainScreen → TimeUtil.now()
  ← "현재 시간 문자열"

MainScreen → Account.getBalance()
  ← "현재 잔고"
```

**2-2. 시장 가격 갱신**
```
MainScreen → Market.tick()
  Market 내부:
    - Spot 가격 랜덤 갱신
    - Future/ETF는 recalcPrice() 실행
```

---

## 3단계: 선물 만기 체크

**3-1. 오늘 날짜 조회**
```
MainScreen → TimeUtil.todayDate()
  ← today ("YYYY-MM-DD")
```

**3-2. 만기 체크 호출**
```
MainScreen → RiskManager.checkFuturesExpiry(today)
```

### alt [만기 도래한 선물 포지션 존재]

**3-3. 포지션 조회**
```
RiskManager → Account.getPositions()
  ← "전체 포지션 목록"
```

**3-4. 각 포지션 순회**
```
loop [각 포지션 반복]:

  alt [선물 포지션 && isExpired(today)==true]:

    RiskManager → Market.findBySymbol(futureSymbol)
      ← FutureContract 반환

    RiskManager → OrderManager.placeMarketOrder(futureSymbol, Future, quantity, Sell)

      OrderManager → Market.findBySymbol(futureSymbol)
        ← FutureContract 반환

      OrderManager → Account.sell(FutureContract, quantity, price)
        ← "청산 성공 여부" 반환

      OrderManager → "주문 기록 저장"

  alt [아직 만기 아님 or 선물이 아님]:
    - 아무 동작 없음
```

### alt [만기 도래한 선물 포지션 없음]
```
→ "만기 청산 대상 없음" 출력
```

---

## 4단계: 강제 청산 체크 (80% 손실)

**4-1. 강제 청산 체크 호출**
```
MainScreen → RiskManager.checkForcedLiquidation()
```

**4-2. 초기 자산 및 현재 자산 조회**
```
RiskManager → Account.getInitialEquity()
  ← initialEquity

RiskManager → Account.getEquity(Market)
  ← currentEquity
```

### alt [현재 자산 <= 초기 자산 * 0.2] (80% 이상 손실)

**4-3. 포지션 조회**
```
RiskManager → Account.getPositions()
  ← "전체 포지션 목록"
```

**4-4. 각 포지션 순회하며 선물만 강제 청산**
```
loop [각 포지션 반복]:

  alt [선물 포지션(Future) && 수량 > 0]:

    RiskManager → OrderManager.placeMarketOrder(symbol, Future, quantity, Sell)

      OrderManager → Market.findBySymbol(symbol)
        ← FutureContract 반환

      OrderManager → Account.sell(FutureContract, quantity, price)
        ← "청산 성공 여부" 반환

      OrderManager → "주문 기록 저장"

  alt [선물이 아닌 포지션]:
    - 무시
```

**4-5. 청산 발생 메시지**
```
MainScreen ← "선물 포지션 강제 청산 발생" 출력
```

### alt [손실이 허용 범위 이내]
```
→ "강제 청산 없음" 출력
```

---

## 5단계: 메인 화면 출력

**5-1. 상위 종목 조회**
```
MainScreen → Market.getTopMovers(5)
  ← "상승률 상위 5개 리스트"
```

**5-2. 화면 출력**
```
MainScreen → User에게 메인 화면 출력
  - 현재 시간
  - 잔고
  - 상위 5개 종목
  - 메뉴 (1~6)
```

**5-3. 사용자 입력 처리**
```
User → 메뉴 번호 입력

MainScreen → Application.changeScreen(...)
  (입력 값에 따라 화면 전환 요청)
```

### loop 끝 (다음 틱으로 반복)

---

## 추가로 발견된 클래스/메서드

**UML 클래스 다이어그램에 없었는데 시퀀스에서 발견된 것들:**

### TimeUtil 클래스
- `static string now()` - 현재 시각 반환 (HH:MM:SS)
- `static string todayDate()` - 오늘 날짜 반환 (YYYY-MM-DD)
- `today("YYYY-MM-DD")` - 날짜 문자열 (위와 동일)

### Market 클래스
- `void initInstruments()` - 종목 초기화
- `void tick()` - 가격 갱신 (simulatePriceChange와 동일한 듯)
- `vector<Instrument*> getTopMovers(int count)` - 상승률 상위 종목 조회

### Account 클래스
- `double getInitialEquity()` - 초기 자산
- `double getEquity(Market& market)` - 현재 자산 (포지션 평가액 포함)
- `void sell(...)` - 매도 처리

### Futures 클래스
- `bool isExpired(string today)` - 만기 여부 체크

---

## 주요 비즈니스 로직 흐름

1. **초기화**: Market 종목 50개 하드코딩 → MainScreen 생성
2. **매 틱마다**:
   - Market.tick() → 가격 변동
   - RiskManager.checkFuturesExpiry() → 만기 청산
   - RiskManager.checkForcedLiquidation() → 강제 청산
   - MainScreen.show() → 화면 출력 및 입력
3. **반복**

---

**문서 끝**
