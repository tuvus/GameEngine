#include "Game.h"

#include <chrono>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void Create_Game(Game& game) {
    cout << "Creating game: " + game.Get_Name() << endl << endl;
    game.Start_Game();
}

Game::Game() = default;

void Game::Start_Game() {
    cout << "Starting game: " + Get_Name() << endl;
    if (!glfwInit()) {
        cerr << "An error occurred with initializing glfw!" << endl;
        exit(-1);
    }

    // ensure OpenGL version matches GLSL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    auto errorCallBack = [](const int error, const char* description) {
        cerr << "glfw error " << error << " " << description;
    };
    glfwSetErrorCallback(errorCallBack);
    window = glfwCreateWindow(1280, 800, Get_Name().c_str(), nullptr, nullptr);
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
    Game_Loop();
}


void Game::Game_Loop() {
    chrono::time_point<chrono::system_clock> frameEndTime = chrono::system_clock::now();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Start Menu");

        chrono::time_point<chrono::system_clock> frameStartTime = chrono::system_clock::now();
        Update_Game(chrono::duration_cast<std::chrono::milliseconds>(frameStartTime - frameEndTime));
        this_thread::sleep_until(frameStartTime + 16ms);
        frameEndTime = chrono::system_clock::now();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    End_Game();
}

void Game::End_Game() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

Game::~Game() = default;

GLFWwindow& Game::GetWindow() const {
    return *window;
}
