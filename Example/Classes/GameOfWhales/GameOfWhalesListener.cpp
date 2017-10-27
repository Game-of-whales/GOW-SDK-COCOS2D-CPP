#include "GameOfWhalesListener.h"
#include <time.h>


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
