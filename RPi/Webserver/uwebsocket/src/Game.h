#pragma once

#include "Player.h"
#include "../src/json.hpp"
#include <string>

using namespace std;
using json = nlohmann::json;

class Game {
public:
    uint8_t updateGame(int8_t penaltyPlayer, int8_t timePlayer, uint16_t time);
    friend void to_json(json& j, const Game& g);
    friend void from_json(const json& j, Game& g);

private:
    uint8_t gameState_;
    uint8_t maxPenalty_;
    uint8_t penaltyCount_;
    uint8_t gameMode_;
    Player players_[8];
};