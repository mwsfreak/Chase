#include "Game.h"

/*
Game::Game(int penalty, int gameMode, string playerNames[], int numOfPlayers) 
: {
    if ((numOfPlayers > 0) && (numOfPlayers <= 8) {     //Assign player names
        for (size_t i = 0; i < numOfPlayers; i++)
        {
            players_[i].setName(playerNames[i]);
        }
    }

    if (penalty <= 0) {
        maxPenalty_ = 10;
    } else {
        maxPenalty_ = penalty;
    }
    
    if (gameMode <= 0) {
        gameMode_ = 1;
    } else if (gameMode > 2) {
        gameMode_ = 2;
    } else {
        gameMode_ = gameMode_;
    }
}
*/

void Game::startGame() {
    //send startkommando på Uart
}

void Game::updateGame(int penaltyPlayer, int timePlayer, int time) {
    int returned = players_[penaltyPlayer].addPenalty();
    players_[timePlayer].newTime(time);

    penaltyCount_++;

    if ( ((gameMode_ == 1) && (returned > maxPenalty_)) ||
         ((gameMode_ == 2) && (penaltyCount_ > maxPenalty_)) ) {
        gameState_ = 2;     //End game
        this->stopGame();
    }
}

void Game::stopGame() {
    //send stopkommando på uart
}

/* Methods to convert Game to and from JSON using nlohmann/json library
   These enable using the overloaded assignment operator for the JSON library 
   Source: https://github.com/nlohmann/json/#arbitrary-types-conversions */
void Game::to_json(json& j, const Game& g) {
    j = json {"gameStatus", g.gameState_};

    if (gameState_ != 0) {
        j["maxPenalty"] = maxPenalty_;
        j["players"] = json::array();

<<<<<<< HEAD
        json::array() playerArray;
        for (size_t int = 0; i < 8; i++) {
=======
       json playerArray = json::array();
        for (size_t i = 0; i < 8; i++) {
>>>>>>> 983af4cdbb7f732ae60eedcbd6c6ee0fd0572f08
            playerArray += { 
                {"name", players_[i].getName()},
                {"avgTime", players_[i].getAvgTime()},
                {"penalty", players_[i].getPenalty()}
<<<<<<< HEAD
            }
        );
=======
            };
        }

>>>>>>> 983af4cdbb7f732ae60eedcbd6c6ee0fd0572f08
        j += playerArray;

    }
}

void Game::from_json(const json& j, Game& g) {
    
    if (j.at("gameStatus") == 1) {
<<<<<<< HEAD
        startGame()
=======
        startGame();
>>>>>>> 983af4cdbb7f732ae60eedcbd6c6ee0fd0572f08
    }
    
    j.at("maxPenalty").get_to(g.maxPenalty_);
    j.at("gameMode").get_to(g.gameMode_);
    
<<<<<<< HEAD
    for (size_t int = 0; i < 8; i++) {
        j.at("players").at(i).at("name").get_to(g.players_[i]);
    }
=======
    /*
    for (size_t i = 0; i < 8; i++) {
        j.at("players").at(i).at("name").get_to(players_[i]);
    }
    */

>>>>>>> 983af4cdbb7f732ae60eedcbd6c6ee0fd0572f08

    //Missing input validation
}