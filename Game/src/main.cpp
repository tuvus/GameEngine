#include <memory>

#include "Application.h"
#include "ApplicationFactory.h"
#include "CardGame.h"
#include <raylib.h>

class CardGameFactory : public ApplicationFactory
{
  public:
    std::unique_ptr<Application> Create_Application(bool client) override
    {
        auto g = std::make_unique<Card_Game>(client);
        g->init_game();
        return g;
    }
};

int main()
{
    auto factory = make_unique<CardGameFactory>();
    Create_Application(std::move(factory), true);
}
