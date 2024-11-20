# 최소 요구 CMake 버전
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 버전
project(dbServer VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 빌드 타입 설정
set(CMAKE_BUILD_TYPE Debug)

# Boost 라이브러리 설정
find_package(Boost REQUIRED COMPONENTS system)

# 헤더 파일 경로 포함
include_directories(${Boost_INCLUDE_DIRS} inc)

# 소스 파일 설정
set(SOURCES
    src/HttpServer.cpp
    src/RequestHandler.cpp
    main.cpp
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# Boost 라이브러리 링크
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# 빌드 결과물 디렉토리 설정 (현재 경로에 실행 파일 생성)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Clean 기능 추가 (빌드 디렉토리 제거)
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
    COMMENT "Removing all build files."
)
