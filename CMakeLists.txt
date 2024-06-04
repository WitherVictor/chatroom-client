cmake_minimum_required(VERSION 3.0.0)
project(chatroom-client VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)

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

# print 依赖库
target_link_libraries(chatroom-client PRIVATE stdc++exp)