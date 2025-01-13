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
};

int main() {
    auto* factory = new CardGameFactory();
    Create_Application(*factory, true);
    delete factory;
}
