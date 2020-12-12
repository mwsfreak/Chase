/*    
* Title:        Player.h
* Description:  Player class header file
* Authors:      Allan Lassesen, Magnus Bisgård Franks
*/

#pragma once

#include <string>
#include <stdint.h>
using namespace std;

class Player
{
public:
    void newTime(int time);
    int addPenalty();
    int getPenalty() const;
    float getAvgTime() const;
    void resetPlayer(string name);
    string getName() const;

private:
    string name_{"Unknown"};        //Player name
    int penalty_{0};                //Penalty points
    float avgTime_{0};              //Calculated average time in seconds
    uint32_t sum_{0};               //Sum of all player times in 1/100th seconds - 32 bytes to avoid overflow
    int timeCounter_{0};            //Number of times to calculate average from
};