cmake_minimum_required(VERSION 3.18)
project(chatroom-client VERSION 0.1.0 LANGUAGES C CXX)

#  导入 FetchContent 模块来引入依赖
include(FetchContent)

#  声明 Boost.Asio 依赖
FetchContent_Declare(
    BoostAsio
    GIT_REPOSITORY https://github.com/boostorg/asio.git
    GIT_TAG boost-1.87.0
)

#  声明 fmt, spdlog 需要 fmt 作为前置依赖
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.1.4
)

#  声明 spdlog 依赖
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

#  下载并编译所有依赖
FetchContent_MakeAvailable(BoostAsio fmt spdlog nlohmann_json)

add_compile_options(-std=c++23)
add_executable(chatroom-client main.cpp)

# std::print() Windows 下依赖头文件
# target_link_libraries(chatroom-client PRIVATE stdc++exp)

# 引入 Boost
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(chatroom-client ${Boost_LIBRARIES})

# 引入 spdlog
target_link_libraries(chatroom-client PRIVATE spdlog::spdlog_header_only)

# Boost.asio Windows 下依赖库
target_link_libraries(chatroom-client PRIVATE ws2_32)

# spdlog 依赖库 fmt
target_link_libraries(chatroom-client PRIVATE fmt::fmt-header-only)

# nlohmann json
target_link_libraries(chatroom-client PRIVATE nlohmann_json::nlohmann_json)