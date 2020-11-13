#include "Player.h"

void Player::newTime(int time)
{
    if (time > 0) {
        sum_ += time;
<<<<<<< HEAD
        timecounter_++;
=======
        timeCounter_++;
>>>>>>> 983af4cdbb7f732ae60eedcbd6c6ee0fd0572f08
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
