cmake_minimum_required(VERSION 3.18)

#  导出编译指令供 clangd 使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#  设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

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

#  声明 nlohmann_json 依赖，作为数据文件存储格式
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

#  声明 ImGui 依赖，作为前端绘制窗口
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8-docking
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

#  下载并编译所有依赖
FetchContent_MakeAvailable(BoostAsio fmt spdlog nlohmann_json ImGui glfw)

#  创建可执行文件
add_executable(${PROJECT_NAME}
    main.cpp

    #  ImGui 源文件
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    #  使用 glfw 与 opengl 作为 ImGui 的后端
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

    #  ImGui::InputText 对 std::string 支持
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

#  启用警告
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -pedantic
)

#  包含 ImGui 头文件
target_include_directories(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

#  链接 backends 的依赖库 glfw 与 opengl
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw

    #  链接 spdlog
    spdlog::spdlog_header_only
)

if (${CMAKE_SYSTEM} MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(${CMAKE_SYSTEM} MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif ()