#!/bin/bash

# 빌드 디렉토리 생성
mkdir -p build
cd build

# CMake로 Makefile 생성
cmake ..

# Make로 빌드
make

# 프로그램 실행
./dbServer

