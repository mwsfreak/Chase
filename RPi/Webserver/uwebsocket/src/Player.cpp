#include "Player.h"

void Player::newTime(int time)
{
    if (time > 0) {
        sum_ += time;
        timeCounter_++;
        avgTime_ = (sum_/timeCounter_)/100.0;     //Calculate avgTime in seconds
    }
}

int Player::addPenalty(void)
{
    penalty_++;
    return penalty_;
}    

int Player::getPenalty(void)
{
    return penalty_;
}

float Player::getAvgTime(void)
{
    return avgTime_;
}

void Player::newGame(string name) {
    name_ = name;
    penalty_ = 0;   // Reset values when new name is applied
    avgTime_ = 0;   //
    sum_ = 0;       //
    
}

string Player::getName() {
    return name_;
}
