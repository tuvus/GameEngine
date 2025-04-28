#pragma once

#include <chrono>
#include <functional>
#include <string>

class Application;

class ApplicationFactory
{
  public:
    ApplicationFactory() = default;
    virtual std::string Get_Name() = 0;
    virtual std::function<void(std::chrono::milliseconds, Application&)>
    Create_Update_Function() = 0;
    virtual ~ApplicationFactory() = default;
};
