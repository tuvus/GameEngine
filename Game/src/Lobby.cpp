
#include "Lobby.h"

#include <iostream>

Lobby::Lobby(Card_Game& g) : EUI_VBox() {
    pos = {0, 0};
    dim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    style.vertical_alignment = Alignment::Center;
    style.horizontal_alignment = Alignment::Center;
    gap = 20;
    auto* players_text = new EUI_Text("Players: 0");
    Add_Child(players_text);
    auto* leave_button = new EUI_Button("Leave Room", [this, &g]() {
        g.Close_Network();
        g.screen = MENU;
        g.set_ui_screen(MENU);
    });
    Add_Child(leave_button);
}
