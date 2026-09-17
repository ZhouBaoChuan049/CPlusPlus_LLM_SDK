cmake_minimum_required(VERSION 3.10)

project(TestDeepSeek)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

add_executable(TestDeepSeek
    DeepseekTest.cc
    ../../Src/Util/LogModule.cc
    ../../Src/DeepSeekProvider.cc
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)

# 项目头文件
include_directories(../../Include)



set(OPENSSL3_ROOT "$ENV{HOME}/local/openssl-3.0.18")

include_directories(
    ${OPENSSL3_ROOT}/include
)

target_compile_definitions(
    TestDeepSeek
    PRIVATE
    CPPHTTPLIB_OPENSSL_SUPPORT
)

set_target_properties(TestDeepSeek PROPERTIES
    BUILD_RPATH "${OPENSSL3_ROOT}/lib64"
)


target_link_libraries(TestDeepSeek
    jsoncpp
    fmt
    spdlog
    gtest
    sqlite3
    Threads::Threads

    ${OPENSSL3_ROOT}/lib64/libssl.so
    ${OPENSSL3_ROOT}/lib64/libcrypto.so
)


