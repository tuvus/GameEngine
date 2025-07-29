#pragma once
#include "ui/eui.h"
#include <chrono>

class Scene {
protected:
    Application& application;
    EUI_Element* root_elem;

public:
    Scene(Application& app) : application(app) {}
    virtual void Update_UI(std::chrono::milliseconds, EUI_Context) = 0;
    virtual void Update(std::chrono::milliseconds) = 0;
    EUI_Element* Get_Root() { return root_elem; }
};
