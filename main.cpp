//  ImGui 头文件
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//  GLFW 头文件
#include <GLFW/glfw3.h>

//  spdlog 头文件
#include "spdlog/spdlog.h"

static void glfw_error_callback(int error, const char* description) {
    spdlog::error("glfw error occurred! error code: {}, description: {}", error, description);
    std::terminate();
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);  // 设置 GLFW 错误回调函数
    if (!glfwInit()) {
        spdlog::error("glfw initialization failed!");
        return -1;
    }

    //  创建 GLFW 窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Demo", nullptr, nullptr);
    if (!window) {
        spdlog::error("glfw window creation failed!");
        return -1;
    }

    //  窗口上下文
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  //  设置垂直同步
    
    //  初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  //  启用键盘导航

    //  设置 ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    //  设置 ImGui 默认风格
    ImGui::StyleColorsDark();
    
    while (!glfwWindowShouldClose(window)) {
        //  发布事件
        glfwPollEvents();

        //  新帧开始
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Hello world window")) {
            ImGui::Text("Hello world!");
            ImGui::End();
        }

        // 渲染
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    //  资源清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}