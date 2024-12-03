cmake_minimum_required(VERSION 3.10)
project(smart_zone_server)

# include 디렉토리 추가
include_directories(inc ${SQLite3_INCLUDE_DIRS})

# 소스 파일 목록
set(SOURCE_FILES
    main.cpp
    src/httpServer.cpp
    src/httpClient.cpp
    src/mainDB.cpp
)

# 실행 파일 생성
find_package(SQLite3 REQUIRED)

add_executable(smart_zone_server ${SOURCE_FILES})

# 라이브러리 링크
target_link_libraries(smart_zone_server 
    SQLite::SQLite3
)

# C++11 표준 사용
target_compile_features(smart_zone_server PRIVATE cxx_std_11)