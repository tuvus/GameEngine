#pragma once

#include "Application.h"

using namespace std;

class Card_Game : public Application
{
  public:
    Card_Game(Application& application);
    void Render(std::chrono::milliseconds deltaTime);
    ~Card_Game();
};
