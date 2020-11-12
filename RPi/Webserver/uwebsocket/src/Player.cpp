#include "Player.h"

Player::Player(string setName): penalty{0}, avgTime{0}, name{setName}{}

Player::setNewTime(int newTime)
{
    list<int>::iterator i;
    int sum;
    meassuredTime.push_back(newTime);
    for (i=meassuredTime.begin() ; i != meassuredTime.end() ; i++)
    {	
        sum += i; 		
    }
    this->avgTime = sum/ meassuredTime.size();
}

Player::setPenalty(void)
{
    this->penalty++;
}    

Player::getPenalty(void)
{
    return penalty;
}

Player::getAVGtime(void)
{
    return avgTime;
}