#include "Application.h"

#include <chrono>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

using namespace std;

void Create_Application(Application& application, bool server) {
    cout << "Creating application: " + application.Get_Name() << endl << endl;
    application.Start_Application(server);
}

Application::Application() = default;

void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) {
    // Temporary here
}

void Application::Start_Application(bool server) {
    cout << "Starting application: " + Get_Name() << endl;

    SteamNetworkingIPAddr addrServer;
    addrServer.Clear();
    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg)) {
        cerr << "GameNetworkingSockets initialization failed " << err_msg << endl;
        exit(-1);
    }

    connection_api = SteamNetworkingSockets();
    char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
    addrServer.ToString(szAddr, sizeof(szAddr), true);
    SteamNetworkingConfigValue_t config_options;
    config_options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)OnConnectionStatusChanged);
    if (server) {
        listen_socket = connection_api->CreateListenSocketIP(addrServer, 1, &config_options);
        if (listen_socket == k_HSteamListenSocket_Invalid) cerr << "Failed to setup listener on port " << 0 << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid) cerr << "Failed to setup listener on port " << 0 << endl;
    } else {
        remote_host_connection = connection_api->ConnectByIPAddress(addrServer, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid) cerr << "Failed to connect to the host" << endl;
    }
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
    Application_Loop();
}


void Application::Application_Loop() {
    chrono::time_point<chrono::system_clock> frame_end_time = chrono::system_clock::now();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Start Menu");

        chrono::time_point<chrono::system_clock> frame_start_time = chrono::system_clock::now();
        Update_Application(chrono::duration_cast<std::chrono::milliseconds>(frame_start_time - frame_end_time));
        this_thread::sleep_until(frame_start_time + 16ms);
        frame_end_time = chrono::system_clock::now();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    End_Application();
}

void Application::End_Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    GameNetworkingSockets_Kill();
}

Application::~Application() = default;

GLFWwindow& Application::Get_Window() const {
    return *window;
}
