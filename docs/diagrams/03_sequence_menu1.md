# 메뉴1: 전체 종목 조회 시퀀스 다이어그램 (A팀)

**출처:** A팀 설계 문서
**작성일:** 2025년 11월
**정리일:** 2025년 12월 1일

---

## 참여 객체

- User
- MainScreen
- Application
- AllStocksScreen
- Market

---

## 흐름

### 1. 메뉴 선택
```
User → 메인 메뉴에서 1(전체 종목 조회) 선택
```

### 2. 화면 전환
```
MainScreen → Application.changeScreen(AllStocksScreen)
  ↓
Application → AllStocksScreen 생성 및 설정
```

### 3. 전체 종목 조회
```
AllStocksScreen → Market.getAll()
  ← "전체 종목 리스트" 반환
```

### 4. 종목 리스트 출력
```
AllStocksScreen → User에게 전체 종목 리스트 출력
```

### 5. 뒤로가기
```
User → 0(뒤로가기) 입력
  ↓
AllStocksScreen → Application.changeScreen(MainScreen)
```

---

## 비고

- **주식 종목은 약 50개 정도 하드코딩으로 저장**

---

## 상세 설명

1. 사용자가 메인 화면에서 "1. 전체 종목 조회"를 선택
2. MainScreen이 Application의 changeScreen() 메서드를 호출하여 AllStocksScreen으로 전환
3. AllStocksScreen은 Market에서 모든 종목 데이터(getAll())를 가져옴
4. 약 50개의 종목 정보를 화면에 출력
5. 사용자가 0을 입력하면 MainScreen으로 다시 돌아감

---

**문서 끝**
