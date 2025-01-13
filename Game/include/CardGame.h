#pragma once
#include "Application.h"
#include "ApplicationWindow.h"

using namespace std;

class Card_Game : public ApplicationWindow {
public:
    Card_Game(Application& application);
    void Render(std::chrono::milliseconds deltaTime) override;
    ~Card_Game() override;
};
