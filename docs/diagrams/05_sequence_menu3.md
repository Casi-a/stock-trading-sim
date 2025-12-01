# 메뉴3: 매수 시퀀스 다이어그램 (A팀)

**출처:** A팀 설계 문서
**작성일:** 2025년 11월
**정리일:** 2025년 12월 1일

---

## 참여 객체

- User
- MainScreen
- Application
- TradeScreen
- OrderManager
- Market
- Account

---

## 흐름

### 1. 매수 메뉴 선택
```
User → 메인 메뉴에서 3(매수) 선택
```

### 2. 화면 전환
```
MainScreen → Application.changeScreen(TradeScreen, isBuy = true)
  ↓
Application → TradeScreen 생성 및 설정 (매수 모드)
```

### 3. 거래 유형 선택
```
TradeScreen → User에게 "거래유형 선택 (1.현물 / 2.선물 / 3.ETF)" 출력
  ↓
User → 2(선물) 선택
```

### 4. 종목 정보 입력
```
TradeScreen → User에게 "선물 종목 코드와 수량 입력" 출력
  ↓
User → symbol = "NQFUT", qty = 3 입력
```

### 5. OrderManager 조회
```
TradeScreen → Application.getOrderManager()
  ← OrderManager 참조 반환
```

### 6. 시장가 매수 주문
```
TradeScreen → OrderManager.placeMarketOrder("NQFUT", Future, 3, Buy)
  ↓
OrderManager → Market.findBySymbol("NQFUT")
    ← inst 또는 null 반환
```

### 7. 주문 처리 분기

#### alt [조회 성공]

**7-1. Account 매수 호출**
```
OrderManager → Account.buy(NQFUT, 3, currentPrice)
  ← "매수 성공 여부 반환 (true/false)"
```

**7-2-1. 매수 성공**
```
alt [매수 성공]:
  OrderManager → TradeScreen에게 true 반환
    ↓
  TradeScreen → User에게 "선물 매수 성공! 잔고/보유 수량 갱신" 출력
```

**7-2-2. 매수 실패**
```
alt [매수 실패]:
  OrderManager → TradeScreen에게 false 반환
    ↓
  TradeScreen → User에게 "매수 실패 (잔고 부족 등)" 출력
```

#### alt [조회 실패]

**7-3. 종목 조회 실패**
```
OrderManager → TradeScreen에게 false (조회 실패) 반환
  ↓
TradeScreen → User에게 "주문 실패(등록되지 않은 정보)" 출력
```

### 8. 메인 화면으로 복귀
```
TradeScreen → Application.changeScreen(MainScreen)
```

---

## 비고

- **거래 유형 선택**: 현물/선물/ETF 중 선택
- **매수 프로세스**:
  1. 종목 코드 및 수량 입력
  2. Market에서 종목 조회
  3. Account 잔고 확인
  4. 매수 성공 시 Position 업데이트 및 잔고 차감
- **현물, 선물, 커스텀 ETF에 따라 메서드명 변동 가능**

---

## 상세 설명

### 매수 성공 조건
1. Market에 종목이 존재해야 함
2. Account 잔고가 충분해야 함 (가격 × 수량 <= 잔고)

### 매수 성공 시 처리
1. Account의 cashBalance 차감
2. Position 추가 또는 수량 증가 (물타기 시 평단가 재계산)
3. Order 기록 저장

### 매수 실패 사유
- 종목 미등록 (Market에 없음)
- 잔고 부족

---

**문서 끝**
