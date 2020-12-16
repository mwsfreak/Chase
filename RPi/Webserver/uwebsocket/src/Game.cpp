/*    
* Title:        Game.cpp
* Description:  Game class source file
* Authors:      Allan Lassesen, Magnus Bisgård Franks
* 
* This code is inspired from
* - Guides, documentation and examples from the json library, provided from:
*   https://github.com/nlohmann/json/
*/

#include "Game.h"

/* Function to update Game according to data received from newly ended turn */
uint8_t Game::updateGame(int8_t penaltyPlayer, int8_t timePlayer, uint16_t time) {
    if (gameState_ == 1) {
        uint8_t newPenalty = 0;

        /* Add penalty point to player, and increment penalty sum */
        if ((penaltyPlayer >= 0) && (penaltyPlayer <= 7)) {     
            newPenalty = players_[penaltyPlayer].addPenalty();  
            penaltyCount_++;
        }

        /* Register players time and calc. average time */
        if ((timePlayer >= 0) && (timePlayer <= 7)) {           
            players_[timePlayer].newTime(time);                 
        }

        /* End game if conditions are met */
        if (  ((gameMode_ == 1) && (newPenalty >= maxPenalty_)) ||
              ((gameMode_ == 2) && (penaltyCount_ >= maxPenalty_)) ) {
            gameState_ = 2;     //End game  
        }
    }

    return gameState_;
}

uint8_t Game::getGameState() const {
    return gameState_;
}


/* Methods to convert Game to and from JSON using nlohmann/json library
   These enable using the overloaded assignment operator for the JSON library 
   Source: https://github.com/nlohmann/json/#arbitrary-types-conversions */
void to_json(json& j, const Game& g) {
    json temp;    //Create temporary json object

    /* Assign game status to JSON object */
    temp["gameStatus"] = g.gameState_;    

    /* If game is running or just ended, add rest of game data */
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

    j = temp;   //Assign temporary json object to calling object
}

void from_json(const json& j, Game& g) {
    /* Check if each field exists in json object. If true, assign to game attributes */
    if (j.contains("gameStatus")) {
        j.at("gameStatus").get_to(g.gameState_);
    }

    if (j.contains("maxPenalty")) {
        j.at("maxPenalty").get_to(g.maxPenalty_);
    }

    if (j.contains("gameMode")) {
        j.at("gameMode").get_to(g.gameMode_);
    }

    /* If array of players is present in json object, reset old players data and set new name */
    if (j.contains("players")) {
        for (size_t i = 0; i < 8; i++) {
            g.players_[i].resetPlayer(j.at("players").at(i).at("name"));
        }
    }
}