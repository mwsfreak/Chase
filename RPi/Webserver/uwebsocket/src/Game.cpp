#include "Game.h"

uint8_t Game::updateGame(int8_t penaltyPlayer, int8_t timePlayer, uint16_t time) {
    uint8_t newPenalty = 0;

    if ((penaltyPlayer >= 0) && (penaltyPlayer <= 7)) {
        newPenalty = players_[penaltyPlayer].addPenalty();
        penaltyCount_++;
    }

    if ((timePlayer >= 0) && (timePlayer <= 7)) {
        players_[timePlayer].newTime(time);
    }

    if ( ((gameMode_ == 1) && (newPenalty >= maxPenalty_)) ||
         ((gameMode_ == 2) && (penaltyCount_ >= maxPenalty_)) ) {
        gameState_ = 2;     //End game
    }

    return gameState_;
}

/* Methods to convert Game to and from JSON using nlohmann/json library
   These enable using the overloaded assignment operator for the JSON library 
   Source: https://github.com/nlohmann/json/#arbitrary-types-conversions */
void to_json(json& j, const Game& g) {
    json temp;

    temp["gameStatus"] = g.gameState_;

    if (g.gameState_ != 0) {
        temp["maxPenalty"] = g.maxPenalty_;

        json playerArray = json::array();
        for (size_t i = 0; i < 8; i++) {
            playerArray.push_back( 
                { 
                    {"name", g.players_[i].getName()},
                    {"avgTime", g.players_[i].getAvgTime()},
                    {"penalty", g.players_[i].getPenalty()}
                } 
            );
        }
        temp["players"] = playerArray;
    }

    j = temp;
}

void from_json(const json& j, Game& g) {
    if (j.contains("gameStatus")) {
        j.at("gameStatus").get_to(g.gameState_);
    }

    if (j.contains("maxPenalty")) {
        j.at("maxPenalty").get_to(g.maxPenalty_);
    }

    if (j.contains("gameMode")) {
        j.at("gameMode").get_to(g.gameMode_);
    }

    if (j.contains("players")) {
        for (size_t i = 0; i < 8; i++) {
            g.players_[i].resetPlayer(j.at("players").at(i).at("name"));
        }
    }

    //Missing input validation
}