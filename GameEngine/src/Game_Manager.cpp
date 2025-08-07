#include "Game_Manager.h"

using namespace std;

Game_Manager::Game_Manager(Application& application, Network& network)
    : application(application), network(network) {
    player_steps = unordered_map<long,long>();
    network.bind_rpc("stepupdate", [this](long new_step) { On_Recieve_Step_Update(new_step); });
    network.bind_rpc("minstepupdate", [this](long player_id, long min_step) {
        On_Recieve_Player_Step_Update(player_id, min_step);
    });
}

Game_Manager::~Game_Manager() {
}

void Game_Manager::On_Recieve_Step_Update(long new_step) {
    max_step = max(max_step, new_step);
}

void Game_Manager::On_Recieve_Player_Step_Update(long player_id, long min_step) {
    player_steps[player_id] = max(player_steps[player_id], min_step);
    min_step = step;
    for (auto const& [id, player_step] : player_steps) {
        min_step = min(min_step, player_step);
    }
}

void Game_Manager::Update() {
    // If we are the server and the players have reached a close enough step
    // We can continue on and procede with the next step
    if (network.Is_Server() && step == max_step && min_step > step - 3) {
        max_step++;
    }
    if (step < max_step) {

        step++;
        if (network.Is_Server()) {
            network.call_rpc("stepupdate", step);
        } else {
            network.call_rpc("minstepupdate",  step);
        }
    }
}

long Game_Manager::Get_New_Id() {
    return next_id++;
}
