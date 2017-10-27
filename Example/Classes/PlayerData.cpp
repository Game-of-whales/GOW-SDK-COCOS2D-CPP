#include "PlayerData.h"



void PlayerData::load()
{
    
}


void PlayerData::save()
{
    
}

void PlayerData::addCoins(int coins)
{
    _coins += coins;
}

void PlayerData::nextLocation()
{
    _location ++;
    if (_location > 26)
        _location = 0;
    
    updateLocation();
}

void PlayerData::updateLocation()
{
    _locationC = (char)((int)('A') + _location);
}
