#pragma once
#include "Application.h"

using namespace std;

class Card_Game : public Application {
public:
    string Get_Name() override;
    void Start_Application(bool server) override;
    void Update_Application(chrono::milliseconds deltaTime) override;
    void End_Application() override;
    ~Card_Game() override;
};
