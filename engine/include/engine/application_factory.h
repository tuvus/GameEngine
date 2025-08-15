#pragma once

#include <memory>

class Application;

class ApplicationFactory {
  public:
    ApplicationFactory() = default;
    virtual ~ApplicationFactory() = default;

    virtual std::unique_ptr<Application> Create_Application(bool client) = 0;
};
