#pragma once

#include <string>
#include <stdint.h>
using namespace std;

class Player
{
public:
    //Player() = default;                 //Make compiler generate default constructor
    //Player(string name) : name_{name};  //Initialise name_ attribute
    void newTime(int time);
    int addPenalty();
    int getPenalty() const;
    float getAvgTime() const;
    void resetPlayer(string name);      // Changed to resetPlayer, and reset private data, when new name is set
    string getName() const;

private:
    string name_{"Unknown"};
    int penalty_{0};
    float avgTime_{0};
    uint32_t sum_{0};               //Big to avoid overflow
    int timeCounter_{0};
};