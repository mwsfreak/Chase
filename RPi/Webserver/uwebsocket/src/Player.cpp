/*    
* Title:        Player.cpp
* Description:  Player class source file
* Authors:      Allan Lassesen, Magnus Bisgård Franks
*/

#include "Player.h"

/* Calculate a players average time */
void Player::newTime(int time) {
    if (time > 0) {
        sum_ += time;
        timeCounter_++;
        avgTime_ = (sum_/timeCounter_)/100.0;     //Calculate avgTime in seconds
    }
}

/* Add penalty point to a player */
int Player::addPenalty(void) {
    penalty_++;
    return penalty_;
}    

/* Get a players penalty points */
int Player::getPenalty(void) const {
    return penalty_;
}

/* Get a players average time */
float Player::getAvgTime(void) const {
    return avgTime_;
}

/* Reset a players data, and set a new name for the player */
void Player::resetPlayer(string name) {
    name_ = name;
    penalty_ = 0;
    avgTime_ = 0;
    sum_ = 0;
}

/* Get a players name */
string Player::getName() const {
    return name_;
}
