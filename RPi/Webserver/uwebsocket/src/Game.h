#pragma once

#include "Player.h"
#include "../src/json.hpp"
<<<<<<< HEAD
#include string
using json = nlohmann::json
=======
#include <string>
using json = nlohmann::json;
>>>>>>> 983af4cdbb7f732ae60eedcbd6c6ee0fd0572f08

class Game {
public:
    //Game(int penalty, int gameMode, string playerNames[]);
    void startGame();
    void updateGame(int penaltyPlayer, int timePlayer, int time);
    void stopGame();

    void to_json(json& j, const Game& g);
    void from_json(const json& j, Game& g);

private:
    int gameState_;
    int maxPenalty_;
    int penaltyCount_;
    int gameMode_;
    Player players_[8];
};