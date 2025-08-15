#pragma once

#include <functional>

#include "application_factory.h"
#include "application_window.h"

class TestWindow : public ApplicationWindow {
  private:
    function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function;

  public:
    TestWindow(Application& application,
               function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function)
        : ApplicationWindow(application), render_function(render_function) {}

    void Render(std::chrono::milliseconds delta_time) override {
        render_function(delta_time, *this);
    }
};

class TestFactory : public ApplicationFactory {
  private:
    function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function;
    function<void(std::chrono::milliseconds, Application&)> update_function;

  public:
    TestFactory(function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function,
                function<void(std::chrono::milliseconds, Application&)> update_function)
        : render_function(render_function), update_function(update_function) {}

    string Get_Name() override { return "TestGame"; }

    unique_ptr<ApplicationWindow> Create_Window(Application& application) override {
        return make_unique<TestWindow>(application, render_function);
    }

    function<void(std::chrono::milliseconds, Application&)> Create_Update_Function() override {
        return update_function;
    }

    ~TestFactory() override = default;
};

inline unique_ptr<ApplicationFactory> CreateApplicationTestFactory(
    function<void(std::chrono::milliseconds, ApplicationWindow&)> render_function,
    function<void(std::chrono::milliseconds, Application&)> update_function) {
    return make_unique<TestFactory>(render_function, update_function);
}
