#include "Game.h"

Game::Game(int Penalty, string Pl1, string Pl2, string Pl3, string Pl4 = "unknown", string Pl5 = "unknown", string Pl6 = "unknown", string Pl7 = "unknown", string Pl8 = "unknown") 
: maxPenalty{Penalty}, players[0]{Pl1}, Player[1]{Pl2},  Player[2]{Pl3},  Player[3]{Pl4},  Player[4]{Pl5},  Player[5]{Pl6},  Player[6]{Pl7},  Player[7]{Pl8}
{
    for(size_t i=0 ; i < 8 ; i++)
    {
        players[i].index = i;
    }
}

Game::updateGame(int penltyPlayer, int timePlayer, int time)
{
    players[penltyPlayer].setPenalty();
    players[timePlayer].setNewTime(time);

    totalPenalty++;

    gameState = totalPenalty < maxPenalty ? 1 : 2;
}

Game::~Game(),
{
}