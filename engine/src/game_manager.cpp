#include "game_manager.h"

using namespace std;

Game_Manager::Game_Manager(Application& application, Network& network,
                           unordered_map<Client_ID, Player_ID>* client_player_ids,
                           Player_ID player_id)
    : application(application), network(network), player_id(player_id) {
    player_steps = unordered_map<Player_ID, long>();

    for (const auto& client_player_id : *client_player_ids) {
        client_id_to_player_id.emplace(client_player_id.first, client_player_id.second);
        player_id_to_client_id.emplace(client_player_id.second, client_player_id.first);
    }

    network.bind_rpc("stepupdate", [this](const long new_step) {
        On_Receive_Step_Update(new_step);
        return RPC_Manager::VALID_CALL_ON_CLIENTS;
    });
    if (network.Is_Server())
        network.bind_rpc("minstepupdate", [this](const long player_id, const long min_step) {
            On_Receive_Player_Step_Update(player_id, min_step);
            return RPC_Manager::VALID;
        });
    if (!network.Is_Server()) {
        network.call_rpc(false, "minstepupdate", player_id, step);
    }
}

Game_Manager::~Game_Manager() {
}

void Game_Manager::On_Receive_Step_Update(long new_step) {
    max_step = max(max_step, new_step);
}

void Game_Manager::On_Receive_Player_Step_Update(Player_ID player_id, long player_min_step) {
    player_steps[player_id] = max(player_steps[player_id], player_min_step);
    min_step = step;
    for (auto const& [id, player_step] : player_steps) {
        min_step = min(min_step, player_step);
    }
}

void Game_Manager::Update() {
    // If there are players still loading in the game then we will wait for them to load
    if (min_step == -1)
        return;

    // If we are the server and the players have reached a close enough step
    // We can continue on and procede with the next step
    if (network.Is_Server() && step == max_step && min_step > step - 3) {
        max_step++;
    }
    if (step < max_step) {

        step++;
        if (network.Is_Server()) {
            network.call_rpc(false, "stepupdate", step);
        } else {
            network.call_rpc(false, "minstepupdate", player_id, step);
        }
    }
}

long Game_Manager::Get_New_Id() {
    return next_id++;
}

long Game_Manager::Get_Current_Step() const {
    return step;
}

Client_ID Game_Manager::Get_Client_ID(Player_ID player_id) {
    return player_id_to_client_id[player_id];
}
