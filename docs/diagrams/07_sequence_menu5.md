# 메뉴5: ETF 커스텀 시퀀스 다이어그램 (A팀)

**출처:** A팀 설계 문서
**작성일:** 2025년 11월
**정리일:** 2025년 12월 1일

---

## 참여 객체

- User
- MainScreen
- Application
- CustomETFScreen
- Market
- CustomETF

---

## 기능 설명

사용자가 직접 선정한 종목들로 비율(%)을 설정하여 묶은 뒤, 이 묶음을 하나의 종목처럼 취급하여 개별 주문의 번거로움 없이 묶음 단위로 일괄 매수/매도 및 수익률 관리를 하는 기능 구현.

---

## 흐름

### 1. ETF 커스텀 메뉴 선택
```
User → 메인 메뉴에서 5(ETF 커스텀) 선택
```

### 2. 화면 전환
```
MainScreen → Application.changeScreen(CustomETFScreen)
  ↓
Application → CustomETFScreen 생성 및 설정
```

### 3. ETF 기본 정보 입력
```
CustomETFScreen → User에게 "새 커스텀 ETF 심볼/이름 입력" 요청
  ↓
User → 심볼/이름 입력 (ex. MYETF, "My ETF")
```

### 4. 구성 종목 입력 루프

**loop [구성 종목 입력 반복]:**

#### 4-1. 구성할 종목 및 비율 입력
```
CustomETFScreen → User에게 "종목 코드와 비율(%) 입력 또는 종료 선택"
  ↓
User → 종목 코드 / 비율 입력 (ex. AAPL, 0.5)
```

#### 4-2. Market에서 종목 조회
```
CustomETFScreen → Market.findBySymbol("AAPL")
  ← AAPL_Instrument 반환
```

#### 4-3. ETF에 구성 요소 추가
```
CustomETFScreen → CustomETF.addComponent(AAPL, 0.5)
  ↓
CustomETF 내부:
  - Component(Instrument*, weight) 추가
  ↓
CustomETF → CustomETF.recalcPrice()
  (구성 종목 가중 평균으로 ETF 가격 계산)
```

**loop 종료 조건:** 사용자가 "종료" 선택

---

### 5. ETF를 Market에 등록
```
CustomETFScreen → Market.addInstrument(ETF)
  ↓
Market → "등록 완료"
  ↓
CustomETFScreen → User에게 "커스텀 ETF 생성 완료!" 메시지 출력
```

### 6. 메인 화면으로 복귀
```
CustomETFScreen → Application.changeScreen(MainScreen)
```

---

## 시퀀스 다이어그램 (이미지 기반)

```
User → MainScreen: 메뉴5(ETF 커스텀) 선택
MainScreen → Application: changeScreen(CustomETFScreen)
Application → CustomETFScreen: 생성 및 설정
CustomETFScreen → User: "새 커스텀 ETF 심볼/이름 입력"
User → CustomETFScreen: 심볼/이름 입력 (ex. MYETF, "My ETF")

[loop - 구성 종목 입력 반복]
  CustomETFScreen → User: "종목 코드 / 비율 입력 또는 종료 선택"
  User → CustomETFScreen: 종목 코드 / 비율 입력 (ex. AAPL, 0.5)
  CustomETFScreen → Market: findBySymbol("AAPL")
  Market → CustomETFScreen: AAPL_Instrument
  CustomETFScreen → CustomETF: addComponent(AAPL, 0.5)
  CustomETF → CustomETF: recalcPrice()
[loop end]

CustomETFScreen → Market: addInstrument(ETF)
Market → CustomETFScreen: 등록 완료
CustomETFScreen → User: "커스텀 ETF 생성 완료!" 출력
CustomETFScreen → Application: changeScreen(MainScreen)
```

---

## 비고

- **ETF 구성**:
  - 여러 종목을 선택하고 각 종목의 비율(weight) 설정
  - 비율 합계는 1.0 (100%)이 되어야 함
- **가격 계산**:
  - CustomETF의 가격 = Σ(구성 종목 가격 × 비율)
  - 구성 종목 가격 변동 시 자동으로 recalcPrice() 호출되어 ETF 가격 갱신
- **거래**:
  - 생성된 CustomETF는 Market의 instruments 리스트에 추가됨
  - 이후 일반 종목처럼 매수/매도 가능

---

## 상세 설명

### CustomETF 생성 프로세스

1. **심볼/이름 입력**: ETF의 고유 심볼과 이름 설정
2. **구성 종목 추가**:
   - 사용자가 종목 코드와 비율을 입력
   - Market에서 해당 종목 조회
   - CustomETF에 Component로 추가
3. **가격 재계산**:
   - 구성 종목들의 가격과 비율을 기반으로 ETF 가격 계산
4. **Market 등록**:
   - 완성된 CustomETF를 Market에 추가
   - 이후 일반 종목처럼 거래 가능

### 예시

```
ETF 이름: "Tech Portfolio"
구성:
  - AAPL: 50%
  - TSLA: 30%
  - NVDA: 20%

ETF 가격 = (AAPL 가격 × 0.5) + (TSLA 가격 × 0.3) + (NVDA 가격 × 0.2)
```

---

**문서 끝**
