#pragma once
#include <functional>
#include <string>

#include "ApplicationWindow.h"

class Application;

class ApplicationFactory {
public:
    ApplicationFactory() = default;
    virtual std::string Get_Name() = 0;
    virtual ApplicationWindow* Create_Window(Application&) = 0;
    virtual function<void(std::chrono::milliseconds, Application&)> Create_Update_Function() = 0;
    virtual ~ApplicationFactory() = default;
};
