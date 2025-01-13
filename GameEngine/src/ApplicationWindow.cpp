#include "ApplicationWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


ApplicationWindow::ApplicationWindow(Application& application): application(application), window(nullptr) {}

void ApplicationWindow::Start_Application_Window() {
    if (!glfwInit()) {
        cerr << "An error occurred with initializing glfw!" << endl;
        exit(-1);
    }

    // Ensure OpenGL version matches GLSL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto error_call_back = [](const int error, const char* description) {
        cerr << "glfw error " << error << " " << description;
    };
    glfwSetErrorCallback(error_call_back);
    window = glfwCreateWindow(1280, 800, application.Get_Name().c_str(), nullptr, nullptr);
    if (!window) {
        cerr << "Opening window failed" << endl;
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Loading glad failed" << endl;
        exit(-1);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void ApplicationWindow::Render_Update(const std::chrono::milliseconds deltaTime) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Start Menu");

    Render(deltaTime);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);

    if (glfwWindowShouldClose(window)) application.Close_Application();
}

Application& ApplicationWindow::Get_Application() const {
    return application;
}


GLFWwindow& ApplicationWindow::Get_Window() const {
    return *window;
}

ApplicationWindow::~ApplicationWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}