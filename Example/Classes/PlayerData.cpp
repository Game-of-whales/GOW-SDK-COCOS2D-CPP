/*
 * Game Of Whales SDK
 *
 * https://www.gameofwhales.com/
 *
 * Copyright © 2018 GameOfWhales. All rights reserved.
 *
 * Licence: https://github.com/Game-of-whales/GOW-SDK-COCOS2D-CPP/blob/master/LICENSE
 *
 */

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
