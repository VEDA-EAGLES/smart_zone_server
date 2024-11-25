cmake_minimum_required(VERSION 3.10)
project(smart_zone_server)

# include 디렉토리 추가
# include_directories(include ${GST_INCLUDE_DIRS} ${GSTRTSP_INCLUDE_DIRS})
include_directories(include ${SQLite3_INCLUDE_DIRS})

# 소스 파일 목록
set(SOURCE_FILES
    main.cpp
    source/httpServer.cpp
    source/mainDB.cpp
)

# 실행 파일 생성
# find_package(PkgConfig REQUIRED)
find_package(SQLite3 REQUIRED)
# pkg_check_modules(GST REQUIRED gstreamer-1.0)
# pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)

add_executable(smart_zone_server ${SOURCE_FILES})

# 라이브러리 링크
target_link_libraries(smart_zone_server 
    SQLite::SQLite3
    # ${GST_LIBRARIES} 
    # ${GSTRTSP_LIBRARIES} 
)

# C++11 표준 사용
target_compile_features(smart_zone_server PRIVATE cxx_std_11)