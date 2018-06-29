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

#ifndef __GAME_OF_WHALES__LISTENER__HEADER__
#define __GAME_OF_WHALES__LISTENER__HEADER__

#include <string>
#include <map>

namespace gameofwhales {
    
    enum PURCHASE_STATE
    {
        LEGAL,
        ILLEGAL,
        UNDEFINED
    };
    
    class SpecialOffer
    {
    public:
        std::string campaign;
        std::string product;
        float priceFactor = 1.0f;
        float countFactor = 1.0f;
        bool redeemable = false;
        long finishedAt = 0;
        std::string payload;
        std::map<std::string, std::string> customValues;
        
        bool isExpiried() const;
        bool hasPriceFactor() const;
        bool hasCountFactor() const;
        long getLeftTime() const;
    private:
    };
    
    class Listener
    {
    public:
        virtual void specialOfferAppeared(const SpecialOffer& offer) {}
        virtual void specialOfferDisappeared(const SpecialOffer& offer) {}
        virtual void onPushDelivered(const SpecialOffer * so, const char* camp, const char* title, const char* message) {}
        virtual void onPurchaseVerified(const char* transactionID, gameofwhales::PURCHASE_STATE state) {}
    };
    
    
    
    
    
}



#endif//__GAME_OF_WHALES__LISTENER__HEADER__
