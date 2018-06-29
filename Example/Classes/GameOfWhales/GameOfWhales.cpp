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

#include "GameOfWhales.h"
#include <vector>



#if GAMEOFWHALES_IOS == 1//IOS Include
#warning "GameOfWhales IOS"
#include "GameOfWhalesIOS.h"


#elif GAMEOFWHALES_ANDROID == 1//ANDROID Include
#include "GameOfWhalesAndroid.h"


#else//NOT IMPLEMENTED Include
#warning "GameOfWhales Native"
//#include "GameOfWhalesNative.h"


#endif

namespace gameofwhales {
    
    const char* version     = "2.0.14";
    const char* platform    = GAMEOFWHALES_PLATFORM;
    
    const char* PROVIDER_APN    = "apn";
    const char* PROVIDER_GCM    = "gcm";
    const char* PROVIDER_FCM    = "fcm";
    
    const char* VERIFY_STATE_LEGAL      = "legal";
    const char* VERIFY_STATE_ILLEGAL    = "illegal";
    const char* VERIFY_STATE_UNDEFINED  = "undefined";
    
    const char* STORE_APPSTORE = "AppleAppStore";
    const char* STORE_SAMSUNG = "SamsungApps";
    const char* STORE_GOOGLEPLAY = "GooglePlay";
    const char* STORE_AMAZON = "";
    
    typedef std::map<std::string,const SpecialOffer *> tactiveoffers;
    tactiveoffers active_offers;
    
    typedef std::vector<gameofwhales::Listener*> tlisteners;
    tlisteners listeners;
   
    const SpecialOffer * getSpecialOffer(const char* product)
    {
        auto it = active_offers.find(product);
        if(it != active_offers.end())
        {
            return it->second;
        }
        
        
        return nullptr;
    }
    
    void removeSpecialOffer(const SpecialOffer * offer)
    {
        if (!offer)
            return;
        
        tactiveoffers::iterator it;
        for (it = active_offers.begin(); it != active_offers.end(); ++it )
            if (it->second == offer)
            {
                break;
            }
        
        if (it!=active_offers.end())
        {
            active_offers.erase(it);
        }
        
        delete offer;
    }
    
    
    gameofwhales::PURCHASE_STATE getState(const char* state_s)
    {
        return gameofwhales::PURCHASE_STATE::LEGAL;
    }
    
    void addListener(gameofwhales::Listener* listener)
    {
        listeners.push_back(listener);
    }
    
    void removeListener(gameofwhales::Listener* listener)
    {
        tlisteners::iterator it = std::find(listeners.begin(), listeners.end(), listener);
        
        if (it != listeners.end())
        {
            listeners.erase(it);
        }
    }
    
    std::string buildReceipt(const char* store, const char* transactioID,  const char* payload)
    {
        gameofwhales::json::JSON receiptJSON;
        receiptJSON["transactioID"] = transactioID;
        receiptJSON["Store"] = store;
        receiptJSON["Payload"] = payload;
        return receiptJSON.dump();
    }
    
    namespace internal
    {
        void notify_onPushDelivered(const SpecialOffer * so, const char* camp, const char* title, const char* message)
        {
            for (auto it = listeners.begin(); it != listeners.end(); ++it)
            {
                (*it)->onPushDelivered(so, camp, title, message);
            }
        }
        
        void notify_specialOfferAppeared(const SpecialOffer* offer)
        {
            if (!offer)
                return;

            const SpecialOffer * active_offer = getSpecialOffer(offer->product.c_str());
            if (active_offer)
            {
                removeSpecialOffer(active_offer);
            }
            
            active_offers[offer->product] = offer;
            
            for (auto it = listeners.begin(); it != listeners.end(); ++it)
            {
                (*it)->specialOfferAppeared(*offer);
            }
        }
        
        void notify_specialOfferDisappeared(const std::string& product)
        {
            const SpecialOffer * offer = getSpecialOffer(product.c_str());
            
            for (auto it = listeners.begin(); it != listeners.end(); ++it)
            {
                (*it)->specialOfferDisappeared(*offer);
            }
            
            removeSpecialOffer(offer);
        }
        
        void notify_onPurchaseVerified(const char* transactionID, gameofwhales::PURCHASE_STATE state)
        {
            for (auto it = listeners.begin(); it != listeners.end(); ++it)
            {
                (*it)->onPurchaseVerified(transactionID, state);
            }
        }
    }
    
    
    void initialize(const char* gameID, bool debugLog)
    {
        gameofwhales::native::_initialize(gameID, version, platform, debugLog);
    }
    
    
    void inAppPurchased(const char* sku, float price, const char* currency, const char* transactionID, const char* receipt)
    {
        gameofwhales::native::_inAppPurchased(sku, price, currency, transactionID, receipt);
    }
    
    
    void updateToken(const char* token, const char* provider)
    {
        gameofwhales::native::_updateToken(token, provider);
    }
    
    
    void pushReacted(const char* camp)
    {
        gameofwhales::native::_pushReacted(camp);
    }
    
    
    void converting(const char* json_resources, const char *place)
    {
        gameofwhales::native::_converting(json_resources, place);
    }
    
    
    void profile(const char* json_changed_params)
    {
        gameofwhales::native::_profile(json_changed_params);
    }
    
    
    void consume(const char* currency, int number, const char* sink, int amount, const char* place)
    {
        gameofwhales::native::_consume(currency, number, sink, amount, place);
    }
    
    
    void acquire(const char* currency, int amount, const char* source, int number, const char * place)
    {
        gameofwhales::native::_acquire(currency, amount, source, number, place);
    }
    
    
    void reportError(const char* message, const char* stacktrace)
    {
        gameofwhales::native::_reportError(message, stacktrace);
    }
}
