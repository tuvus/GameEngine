#include "CardGame.h"
#include "Application.h"
#include "ApplicationFactory.h"

class CardGameFactory : public ApplicationFactory {
public:
    string Get_Name() override {
        return "NewCardGame";
    }

    ApplicationWindow* Create_Window(Application& application) override {
        return new Card_Game(application);
    }

    function<void(std::chrono::milliseconds, Application&)> Create_Update_Function() override {
        return [](std::chrono::milliseconds delta_time, Application& application) {};
    }
};

int main() {
    auto* factory = new CardGameFactory();
    Create_Application(*factory, true);
    delete factory;
}
