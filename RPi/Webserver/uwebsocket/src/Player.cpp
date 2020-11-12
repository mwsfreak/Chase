#include "Player.h"

void Player::newTime(int time)
{
    if (time > 0) {
        sum_ += time;
        timecounter_++;
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

void Player::setName(string name) {
    name_ = name;
}

string Player::getName() {
    return name_;
}
