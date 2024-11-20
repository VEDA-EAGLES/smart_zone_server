# Makefile for Linux

CXX = g++                  # C++ 컴파일러
CXXFLAGS = -std=c++17       # C++17 표준 사용
LIBS = -lboost_system -lboost_thread -pthread  # 링크할 라이브러리
SRC = testServer.cpp        # 소스 파일
OBJ = testServer.o          # 오브젝트 파일
EXE = testServer            # 실행 파일 이름

# 기본 목표 (실행 파일 생성)
all: $(EXE)

# 실행 파일 생성
$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $(EXE) $(LIBS)

# 소스 파일에서 오브젝트 파일 생성
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)

# 클린 목표 (컴파일된 파일 제거)
clean:
	rm -f $(OBJ) $(EXE)

# 오브젝트 파일만 삭제하는 목표
clean-obj:
	rm -f $(OBJ)

# 실행 (make run으로 실행할 수 있도록 설정)
run: $(EXE)
	./$(EXE)

.PHONY: clean run clean-obj
