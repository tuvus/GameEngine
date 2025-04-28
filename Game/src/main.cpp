#include <memory>

#include "Application.h"
#include "ApplicationFactory.h"
#include "CardGame.h"

class CardGameFactory : public ApplicationFactory
{
  public:
    string Get_Name() override { return "NewCardGame"; }

    function<void(std::chrono::milliseconds, Application&)> Create_Update_Function() override
    {
        return [](std::chrono::milliseconds delta_time, Application& application) {};
    }
};

int main()
{
    unique_ptr<ApplicationFactory> factory = make_unique<CardGameFactory>();
    Create_Application(move(factory), true);
}
