#pragma once
#include <functional>

#include "ApplicationFactory.h"
#include "ApplicationWindow.h"

class TestWindow : public ApplicationWindow {
private:
    function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function;

public:
    TestWindow(Application& application,
        function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function): ApplicationWindow(application),
        render_function(render_function) {}

    void Render(std::chrono::milliseconds delta_time) override {
        render_function(delta_time, *this);
    }
};

class TestFactory : public ApplicationFactory {
private:
    function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function;

public:
    TestFactory(function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function): render_function(
        render_function) {}

    string Get_Name() override { return "TestGame"; }

    ApplicationWindow* Create_Window(Application& application) override {
        return new TestWindow(application, render_function);
    }

    ~TestFactory() override = default;
};

inline ApplicationFactory* CreateApplicationTestFactory(function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function) {
    return new TestFactory(render_function);
}
