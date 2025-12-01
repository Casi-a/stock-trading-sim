# A팀 설계 명세 분석

**분석 일시:** 2025년 12월 1일
**분석자:** B팀 (팀장: 신바다)

---

## 1. 설계 개요

A팀은 주식 거래 시뮬레이터를 3계층 아키텍처로 설계:
- **Domain Layer**: Market, Account, OrderManager, RiskManager, Instrument 계층
- **UI Layer**: Screen 상속 구조 (MainScreen, TradeScreen, HoldingsScreen, AllStocksScreen, CustomETFScreen)
- **Utility Layer**: TimeUtil, RandomUtil

주요 기능:
- 현물/선물/커스텀 ETF 거래
- 자동 시장가 변동 (tick)
- 선물 만기 처리
- 80% 손실 시 강제청산

---

## 2. 장점

### 2.1 명확한 계층 분리
- Domain/UI/Utility 3계층 구조로 책임 분리 명확
- Screen 추상 클래스로 화면 전환 통일

### 2.2 객체지향 원칙 준수
- Instrument 상속 구조: SpotStock, FutureContract, CustomETF
- 다형성 활용: InstrumentType enum으로 타입 구분
- 컴포지션: CustomETF가 Component 구조로 다른 Instrument 참조

### 2.3 리스크 관리 체계
- RiskManager가 독립적으로 손실률 계산
- 80% 손실 시 자동 청산 로직 분리
- 선물 만기일 자동 처리

### 2.4 확장성
- Screen 상속으로 새 화면 추가 용이
- Instrument 상속으로 새 금융상품 추가 가능

---

## 3. 단점 및 문제점

### 3.1 클래스 다이어그램과 시퀀스 다이어그램 불일치

**핵심 문제**: 클래스 다이어그램(설계도)에는 없는데, 시퀀스 다이어그램(실제 작동)에는 있는 메서드들이 다수 존재

| 클래스 | 누락 메서드 | 발견 위치 | 영향 |
|--------|------------|-----------|------|
| Market | `initInstruments()` | 02_sequence_overall.md | 시장 초기화 불가 |
| Market | `tick()` | 02_sequence_overall.md | 가격 변동 불가 |
| Market | `getTopMovers()` | 03_sequence_menu1.md | 급등/급락 조회 불가 |
| Market | `checkExpiries()` | 02_sequence_overall.md | 만기 처리 불가 |
| Account | `getPositionsByType()` | 04_sequence_menu2.md | 타입별 조회 불가 |
| Account | `checkForceClose()` | 02_sequence_overall.md | 강제청산 체크 불가 |
| CustomETF | `addComponent()` | 07_sequence_menu5.md | ETF 구성 불가 |
| CustomETF | `recalcPrice()` | 07_sequence_menu5.md | ETF 가격 계산 불가 |
| Application | `changeScreen()` | 모든 시퀀스 | 화면 전환 불가 |

→ **총 9개 메서드**가 클래스 다이어그램에 명시되지 않아 구현 시 혼란 발생

### 3.2 FutureContract 클래스 설계 불완전

시퀀스 다이어그램(05, 06)에서 선물 거래를 다루지만, FutureContract 클래스에 필수 필드 누락:
- **누락**: `leverage` (레버리지 배율), `underlying` (기초자산)
- **현재**: `expiryDate`만 존재
- **문제**: 레버리지 없으면 선물의 핵심 특성 구현 불가

### 3.3 계층 분리 원칙 위반

`02_sequence_overall.md` 51번 라인:
```
MainScreen → Market.tick()
```
- UI 계층(MainScreen)이 Domain 계층(Market) 직접 호출
- 올바른 설계: `MainScreen → Application → Market`

### 3.4 메모리 관리 정책 불명확

- `Instrument*`, `Screen*` 포인터 사용하지만 소유권/해제 명시 없음
- Market의 instruments 벡터와 CustomETF의 Component 포인터 라이프사이클 불명확

### 3.5 Position 클래스 상세 로직 부재

- 물타기 시 평균 매입가 재계산 로직 없음
- 실현손익/평가손익 구분 없음

---

## 4. B팀 개선사항

### 4.1 누락된 9개 메서드 전부 구현 완료
- **김다인**: Market 클래스에 `initInstruments()`, `tick()`, `getTopMovers()`, `checkExpiries()` 구현
- **김진광**: Account에 `getPositionsByType()`, `checkForceClose()` 구현 / CustomETF에 `addComponent()`, `recalcPrice()` 구현
- **신바다**: Application에 `changeScreen()` 구현 예정

### 4.2 FutureContract 필드 보완
```cpp
class FutureContract : public Instrument {
    double leverage;        // 레버리지 배율 추가
    string underlying;      // 기초자산 정보 추가
    Date expiryDate;
};
```

### 4.3 계층 분리 수정
MainScreen이 Market 직접 호출하지 않고 Application 경유:
```cpp
// Before: MainScreen → Market.tick()
// After:  MainScreen → Application.updateMarket() → Market.tick()
```

### 4.4 메모리 관리 명확화
- Screen은 Application이 소유 및 해제
- Instrument는 Market이 소유 및 해제
- Position은 Account가 소유 및 해제

### 4.5 Position 평단가 계산 로직 구현
```cpp
void Position::addQuantity(int qty, double price) {
    avgPrice = (avgPrice * quantity + price * qty) / (quantity + qty);
    quantity += qty;
}
```

---

## 5. 최종 평가

**총평**: A팀 설계는 3계층 아키텍처와 객체지향 원칙을 잘 따르고 있으나, **클래스 다이어그램과 시퀀스 다이어그램 간 불일치**가 가장 큰 문제. 핵심 메서드 9개 누락, FutureContract 필드 누락, 계층 분리 위반 등 구현 시 혼란 요소 다수.

B팀은 A팀 설계 의도를 유지하면서 누락된 부분을 보완하고, 메모리 관리 및 평단가 계산 로직을 명확히 구현하여 안정적인 시뮬레이터 완성.

---

**문서 끝**
