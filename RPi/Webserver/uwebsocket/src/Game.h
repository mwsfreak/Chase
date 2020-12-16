/*    
* Title:        Game.h
* Description:  Game class header file
* Authors:      Allan Lassesen, Magnus Bisgård Franks
* 
* This code is inspired from
* - Guides, documentation and examples from the json library, provided from:
*   https://github.com/nlohmann/json/
*/

#pragma once

#include "Player.h"
#include "../src/json.hpp"
#include <string>

using namespace std;
using json = nlohmann::json;

class Game {
public:
    uint8_t updateGame(int8_t penaltyPlayer, int8_t timePlayer, uint16_t time);
    uint8_t getGameState() const;
    
    friend void to_json(json& j, const Game& g);    //Conversion from Game to Json object
    friend void from_json(const json& j, Game& g);  //Conversion from Json to Game object

private:
    uint8_t gameState_;     //Game State: 0 = Scoreboard, 1 = Running, 2 = Stopped 
    uint8_t maxPenalty_;    // Game ends when maxPenalty is reached, according to gameMode
    uint8_t penaltyCount_;  // Current sum of penalty points
    uint8_t gameMode_;      // Game ends when: 1 = First player reaches maxPenalty, 2 = penaltycount reaches maxPenalty 
    Player players_[8];     // Array of players (index 0-7)
};