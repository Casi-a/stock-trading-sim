# OOP Term Project - B팀
# 주식 거래 시뮬레이터

## 📋 프로젝트 개요
객체지향프로그래밍 과목 텀 프로젝트로 개발한 주식 거래 시뮬레이션 프로그램입니다. B팀은 A팀이 작성한 설계 명세서를 기반으로 구현합니다.

**제출 마감:** 2025년 12월 9일
**팀장:** 신바다
**팀 구성:** B팀 (김다인, 김진광, 신바다, 이호준, 임찬형, 정승아)

## 🎯 주요 기능
- **다양한 자산 거래:** 현물 주식, 선물 계약, 커스텀 ETF
- **실시간 가격 시뮬레이션:** 랜덤 가격 변동 및 고가/저가/변동률 추적
- **커스텀 ETF 생성:** 사용자 정의 포트폴리오 묶음 기능
- **리스크 관리 시스템:**
  - 손실 80% 발생 시 선물 자동 청산
  - 만기일 지난 선물 자동 청산
- **대화형 콘솔 UI:** 거래, 보유 종목 조회, 시장 분석을 위한 다중 화면

## 🏗️ 아키텍처

### 3계층 설계
1. **도메인 계층** - 거래 비즈니스 로직
2. **UI 계층** - 콘솔 기반 사용자 인터페이스
3. **유틸리티 계층** - 시간 및 난수 생성 헬퍼 함수

### 핵심 클래스

#### 도메인 계층
- `Instrument` - 모든 거래 가능 자산의 추상 베이스 클래스
- `SpotStock` - 일반 주식 구현
- `FutureContract` - 레버리지와 만기일이 있는 선물
- `CustomETF` - 사용자 정의 포트폴리오 묶음
- `Market` - 모든 종목 관리 및 가격 업데이트
- `Account` - 현금 잔고 및 포지션 관리
- `Position` - 특정 종목의 보유 내역
- `Order` - 체결된 거래 기록
- `OrderManager` - 매수/매도 주문 처리
- `RiskManager` - 강제 청산 및 만기 처리

#### UI 계층
- `Screen` - 모든 화면의 추상 베이스 클래스
- `MainScreen` - 메인 메뉴 (상승률 상위 종목 및 잔고 표시)
- `AllStocksScreen` - 전체 시장 종목 리스트
- `HoldingsScreen` - 사용자 보유 포지션
- `TradeScreen` - 매수/매도 인터페이스
- `CustomETFScreen` - ETF 생성 인터페이스
- `Application` - 메인 애플리케이션 컨트롤러

#### 유틸리티 계층
- `TimeUtil` - 현재 시간 포맷팅
- `RandomUtil` - 가격 변동률 생성

## 👥 팀원별 담당

| 팀원 | 담당 부분 |
|------|----------|
| 김다인 | Application, Market, Instrument, SpotStock, FutureContract, TimeUtil, RandomUtil |
| 김진광 | Account, OrderManager, RiskManager, Position, Order, CustomETF |
| 신바다 | AllStocksScreen, Screen 베이스 클래스 |
| 이호준 | MainScreen, HoldingsScreen |
| 임찬형 | CustomETFScreen |
| 정승아 | TradeScreen |

## 📁 프로젝트 구조

```
stock-trading-sim/
├── docs/                    # 문서 및 명세서
│   ├── diagrams/           # A팀 UML/시퀀스 다이어그램 (7개)
│   │   ├── 01_uml_class.md
│   │   ├── 02_sequence_overall.md
│   │   ├── 03_sequence_menu1.md
│   │   ├── 04_sequence_menu2.md
│   │   ├── 05_sequence_menu3.md
│   │   ├── 06_sequence_menu4.md
│   │   └── 07_sequence_menu5.md
│   ├── Team A 클래스 기획서.pdf
│   ├── Tema A UML 다이어그램 및 시퀀스 다이어그램.pdf
│   └── Term Project.pdf
├── include/                 # 헤더 파일 (개발용)
│   ├── core/               # 핵심 비즈니스 로직
│   │   ├── Account.h / Application.h / Market.h
│   │   ├── OrderManager.h / RiskManager.h
│   ├── model/              # 데이터 모델
│   │   ├── Instrument.h / InstrumentType.h
│   │   ├── SpotStock.h / FutureContract.h / CustomETF.h
│   │   ├── Position.h / Order.h
│   ├── ui/                 # 사용자 인터페이스
│   │   ├── Screen.h (베이스 클래스)
│   │   ├── MainScreen.h / AllStocksScreen.h / HoldingsScreen.h
│   │   ├── TradeScreen.h / CustomETFScreen.h
│   └── util/               # 유틸리티
│       ├── TimeUtil.h / RandomUtil.h
├── src/                     # 구현 파일 (개발용)
│   ├── main.cpp            # 모듈화 버전 메인
│   ├── core/               # 핵심 비즈니스 로직 구현
│   ├── model/              # 데이터 모델 구현
│   ├── ui/                 # UI 구현
│   └── util/               # 유틸리티 구현
├── A_TEAM_SPEC_REVIEW.md   # A팀 설계 분석 문서
├── main.cpp                # 최종 제출용 단일 파일 (1114줄)
└── readme.md
```

## 🚀 구현 기능

### 1. 거래 기능
- 시장가 주문 실행 (매수/매도)
- 현물, 선물, ETF 지원
- 자동 포지션 관리
- 주문 내역 추적

### 2. 커스텀 ETF 시스템
- 여러 주식으로 커스텀 포트폴리오 정의
- 각 구성 요소별 비율(%) 설정
- 구성 요소 기반 자동 가격 계산
- ETF를 단일 단위로 거래

### 3. 리스크 관리
- **선물 청산:** 계좌 자산이 초기 자본의 20% 이하로 떨어지면 자동 청산
- **만기 처리:** 만기일 지난 선물 계약 자동 청산
- 시장 틱마다 지속적인 리스크 모니터링

### 4. 시장 시뮬레이션
- 약 50개의 사전 로드된 주식 심볼
- 매 틱마다 랜덤 가격 변동
- 고가/저가 추적
- 변동률 계산
- 상승률 상위 5개 종목 표시

## 🎮 사용자 인터페이스 흐름

### 메인 화면
- 현재 시각 표시
- 계좌 잔고
- 상승률 상위 5개 종목
- 메뉴 옵션:
  1. 전체 종목 조회
  2. 보유 종목 조회
  3. 매수
  4. 매도
  5. ETF 커스텀
  6. 종료

### 거래 흐름
1. 종목 타입 선택 (현물/선물/ETF)
2. 심볼 입력
3. 수량 입력
4. 주문 확인
5. 계좌 자동 업데이트

## 💻 기술 상세

### 초기 설정
- 초기 현금 잔고: 1,000,000원
- 약 50개의 하드코딩된 주식 심볼
- 선물 만기일: 2025-12-31
- 모든 종목은 Market의 통합 리스트에 저장

### 주요 디자인 패턴
- **상속:** Instrument 베이스 클래스와 SpotStock, FutureContract, CustomETF 파생 클래스
- **컴포지션:** Account는 Positions를 포함, Market은 Instruments를 포함
- **다형성:** 가상 함수 show()를 사용하는 Screen 기반 UI 시스템

## 📝 제출 요구사항

### 제출물
1. **코드:** 단일 C++ 소스 파일 (main.cpp)
2. **리포트:** 인쇄본 포함 내용:
   - B팀의 원본 명세서
   - A팀 설계 구현 리뷰
   - 발표 슬라이드
3. **발표:** 수업 중 데모 및 토론

### 평가 기준 (총 25점)
- 설계 품질 (5점)
- 구현 및 리뷰 (5점)
- 리포트 (6점)
- 발표 (7점)
- 동료 평가 (2점)

## 🔄 개발 워크플로우

1. **Phase 1:** A팀이 설계 명세서 작성 ✅
2. **Phase 2:** B팀이 명세서 기반으로 구현 ✅
   - 김다인: 7개 클래스 완료 ✅
   - 김진광: 6개 클래스 완료 ✅
   - 이호준: 2개 화면 완료 ✅
   - 정승아: 1개 화면 완료 ✅
   - 임찬형: 1개 화면 완료 ✅
   - 신바다: Screen.h, AllStocksScreen 완료 ✅
3. **Phase 3:** 코드 통합 및 헤더 분리 완료 ✅
   - 총 40개 파일 (헤더 20개 + 구현 19개 + main 1개)
   - A팀 명세 누락 메서드 9개 구현 완료
   - 인코딩 문제 수정 완료
   - CustomETFScreen 완전 구현 완료
4. **Phase 4:** 단일 main.cpp 병합 완료 ✅
   - 총 1114줄 단일 파일
   - 컴파일 테스트 완료
   - 모든 기능 작동 확인
5. **Phase 5:** 프로젝트 구조 정리 완료 ✅ (12/2)
   - include/의 .cpp 파일들을 src/로 이동
   - 중복 파일 17개 정리
   - 파일명 통일 (CustomETFScreen)
   - 불필요한 CMakeLists.txt 제거
6. **최종:** 제출 및 발표 준비 중 🔄 (12/9)

## 📚 참고자료
- 과목명: 객체지향프로그래밍
- 설계: A팀
- 구현: B팀
- 명세서 교환: A → B → C → D → E → F → A
- **A팀 설계 분석:** [A_TEAM_SPEC_REVIEW.md](./A_TEAM_SPEC_REVIEW.md)
- **UML/시퀀스 다이어그램:** [docs/diagrams/](./docs/diagrams/)

## 🛠️ 빌드 방법

단일 파일 C++ 프로젝트이므로 다음과 같이 컴파일:
```bash
g++ -std=c++11 main.cpp -o stock_trading_sim
./stock_trading_sim
```

---
**참고:** 본 구현은 A팀의 명세서를 최대한 따르되, 필요한 수정 및 개선사항은 최종 리포트에 문서화했습니다.
