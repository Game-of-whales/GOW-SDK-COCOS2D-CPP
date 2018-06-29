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

#include "GameOfWhalesListener.h"
#include <ctime>


namespace gameofwhales {
    
    
    long SpecialOffer::getLeftTime() const
    {
        std::time_t t = std::time(nullptr);
        return (finishedAt - t);
    }
    
    bool SpecialOffer::isExpiried() const
    {
        return getLeftTime() <= 0;
    }
    
    
    bool SpecialOffer::hasPriceFactor() const
    {
        return priceFactor >= 1.0001f || priceFactor <= 0.999f;
    }
    
    
    bool SpecialOffer::hasCountFactor() const
    {
        return countFactor >= 1.0001f || countFactor <= 0.999f;

    }
}
