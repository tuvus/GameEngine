#include <memory>

#include "CardGame.h"
#include "Application.h"
#include "ApplicationFactory.h"

class CardGameFactory : public ApplicationFactory {
public:
    string Get_Name() override {
        return "NewCardGame";
    }

    unique_ptr<ApplicationWindow> Create_Window(Application& application) override {
        return make_unique<Card_Game>(application);
    }

    function<void(std::chrono::milliseconds, Application&)> Create_Update_Function() override {
        return [](std::chrono::milliseconds delta_time, Application& application) {};
    }
};

int main() {
    unique_ptr<ApplicationFactory> factory = make_unique<CardGameFactory>();
    Create_Application(move(factory), true);
}
