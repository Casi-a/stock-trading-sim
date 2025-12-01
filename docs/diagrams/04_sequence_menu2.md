# 메뉴2: 보유 종목 조회 시퀀스 다이어그램 (A팀)

**출처:** A팀 설계 문서
**작성일:** 2025년 11월
**정리일:** 2025년 12월 1일

---

## 참여 객체

- User
- MainScreen
- Application
- HoldingsScreen
- Account

---

## 흐름

### 1. 메뉴 선택
```
User → 메인 메뉴에서 2(보유 종목 조회) 선택
```

### 2. 화면 전환
```
MainScreen → Application.changeScreen(HoldingsScreen)
  ↓
Application → HoldingsScreen 생성 및 설정
```

### 3. 현물 포지션 조회
```
HoldingsScreen → Account.getPositionsByType(Spot)
  ← "현물 포지션 목록" 반환
```

### 4. 선물 포지션 조회
```
HoldingsScreen → Account.getPositionsByType(Future)
  ← "선물 포지션 목록" 반환
```

### 5. ETF 포지션 조회
```
HoldingsScreen → Account.getPositionsByType(CustomETF)
  ← "ETF 포지션 목록" 반환
```

### 6. 보유 종목 출력
```
HoldingsScreen → User에게 현물/선물/ETF 보유 종목 리스트 출력
  [현물 섹션]
  - 종목1: 수량, 평단가, 현재가, 손익률
  - 종목2: ...

  [선물 섹션]
  - 선물1: 수량, 평단가, 현재가, 손익률

  [커스텀 ETF 섹션]
  - ETF1: 수량, 평단가, 현재가, 손익률
```

### 7. 뒤로가기
```
User → 0(뒤로가기) 입력
  ↓
HoldingsScreen → Application.changeScreen(MainScreen)
```

---

## 비고

- **현물, 선물, ETF 각각 따로 섹션 나눠서 출력**
- 각 포지션은 종목명, 수량, 평균 매입가, 현재 평가액, 손익률 등을 표시

---

## 상세 설명

1. 사용자가 메인 화면에서 "2. 보유 종목 조회"를 선택
2. MainScreen이 Application의 changeScreen()을 호출하여 HoldingsScreen으로 전환
3. HoldingsScreen은 Account에서 타입별로 포지션을 조회:
   - InstrumentType::Spot (현물)
   - InstrumentType::Future (선물)
   - InstrumentType::ETF (커스텀 ETF)
4. 각 타입별로 섹션을 나눠서 포지션 정보 출력
5. 사용자가 0을 입력하면 MainScreen으로 돌아감

---

**문서 끝**
