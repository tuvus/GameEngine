#pragma once

// Needed to include glad manually
// GLFW seems to try to auto-include glad which causes problems
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Application.h"

class ApplicationWindow {
private:
    Application& application;
    GLFWwindow* window;

public:
    ApplicationWindow(Application& application);
    void Start_Application_Window();
    void Render_Update(std::chrono::milliseconds deltaTime);
    /**
     * Handles the main game logic and is called every frame.
     * @param deltaTime The game time of this frame in milliseconds
     */
    virtual void Render(std::chrono::milliseconds deltaTime) = 0;
    Application& Get_Application() const;
    GLFWwindow& Get_Window() const;
    virtual ~ApplicationWindow();
};
