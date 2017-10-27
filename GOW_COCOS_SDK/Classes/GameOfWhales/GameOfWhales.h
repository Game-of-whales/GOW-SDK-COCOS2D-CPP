
#ifndef __GAME_OF_WHALES__HEADER__
#define __GAME_OF_WHALES__HEADER__

#include "GameOfWhalesPlatform.h"
#include "GameOfWhalesListener.h"
#include "GameOfWhalesJSON.hpp"

#include <vector>
#include <string>

namespace gameofwhales {
    
    typedef std::vector<std::string, int> converting_type;
    typedef std::vector<std::string, int> profile_changes_type;
    
    extern const char* version;
    extern const char* platfrom;
    
    extern const char* PROVIDER_APN;
    extern const char* PROVIDER_GCM;
    extern const char* PROVIDER_FCM;
    
    extern const char* STORE_APPSTORE;
    extern const char* STORE_SAMSUNG;
    extern const char* STORE_GOOGLEPLAY;
    extern const char* STORE_AMAZON;
    
    
    void initialize(const char* gameID, bool debugLog);
    void inAppPurchased(const char* sku, float price, const char* currency, const char* transactionID, const char* receipt);
    void updateToken(const char* token, const char* provider);
    void pushReacted(const char* camp);
    void converting(const char* json_resources, const char *place);
    void profile(const char* json_changed_params);
    void consume(const char* currency, int number, const char* sink, int amount, const char* place);
    void acquire(const char* currency, int amount, const char* source, int number, const char* place);
    void reportError(const char* message, const char* stacktrace);
    
    void addListener(gameofwhales::Listener*);
    void removeListener(gameofwhales::Listener*);
    
    
    const SpecialOffer * getSpecialOffer(const char* product);
    
    std::string buildReceipt(const char* store, const char* transactioID,  const char* payload);
    
    namespace internal
    {
        void notify_onPushDelivered(const char* camp, const char* title, const char* message);
        void notify_specialOfferAppeared(const SpecialOffer * offer);
        void notify_specialOfferDisappeared(const std::string& product);
        void notify_onPurchaseVerified(const char* transactionID, gameofwhales::PURCHASE_STATE state);
    }
}












#endif//__GAME_OF_WHALES__HEADER__
