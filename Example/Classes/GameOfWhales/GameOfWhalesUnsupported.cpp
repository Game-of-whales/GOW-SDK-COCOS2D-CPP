#include "GameOfWhalesUnsupported.h"
#if GAMEOFWHALES_UNSUPPORTED == 1


namespace gameofwhales {
    
    namespace native {
        
        void _initialize(const char* gameKey, const char* version, const char* platform, bool debug)
        {
            
        }
        
        void _inAppPurchased(const char* sku, float price, const char* currency, const char* transactionID, const char* receipt)
        {
            
        }
        
        
        void _updateToken(const char* token, const char* provider)
        {
            
        }
        
        
        void _pushReacted(const char* camp)
        {
            
        }
        
        
        void _converting(const char* resources, const char *place)
        {
            
        }
        
        
        void _profile(const char* parameters)
        {
            
        }
        
        
        void _consume(const char* currency, int number, const char* sink, int amount, const char* place)
        {
            
        }
        
        
        void _acquire(const char* currency, int amount, const char* source, int number, const char place)
        {
            
        }
        
        
        void _reportError(const char* message, const char* stacktrace)
        {
            
        }
        
        
    }
}
#endif
