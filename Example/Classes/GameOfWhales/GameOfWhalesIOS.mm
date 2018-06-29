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
#include "GameOfWhalesIOS.h"
#if GAMEOFWHALES_IOS == 1

#include "GameOfWhales.h"
#include "GameOfWhalesListener.h"

#import <GameOfWhales/GameOfWhales.h>

@interface GameOfWhalesProxy : NSObject<GWDelegate>
@end

static GameOfWhalesProxy * gw_proxyInstance = nil;
static NSDictionary * gw_launchOptions = nil;

namespace gameofwhales {
    extern char* VERIFY_STATE_LEGAL;
    extern char* VERIFY_STATE_ILLEGAL;
    extern char* VERIFY_STATE_UNDEFINED;
}

using namespace gameofwhales;

bool GW_isPCharValid(const char* value)
{
    if (value == 0)
        return false;
    
    if (strlen(value) == 0)
        return false;
    
    return true;
}

#define GW_CHECK_CHAR(name, from) if (!GW_isPCharValid(name)) { \
    NSLog(@"GameOfWhales ERROR: Value '%@' from '%@' method is not valid", @#name, @#from); \
    return; }

@implementation GameOfWhalesProxy
+ (void) load
    {
        NSLog(@"GameOfWhalesProxy LOAD");
        gw_proxyInstance = [[GameOfWhalesProxy alloc] init];
    
        [[NSNotificationCenter defaultCenter] addObserver:gw_proxyInstance selector:@selector(ApplicationDidFinishLaunchingNotification:)
                                                 name:@"UIApplicationDidFinishLaunchingNotification" object:nil];
    
    
    
    //[[NSNotificationCenter defaultCenter] addObserver:gw_proxyInstance selector:
    // @selector(didRegisterForRemoteNotificationsWithDeviceToken:)
    //                                             name:kUnityDidRegisterForRemoteNotificationsWithDeviceToken object: nil];
    }

    -(id)init
    {
        if (self = [super init])
            {
        
            }
        return self;
    }

- (void) didRegisterForRemoteNotificationsWithDeviceToken:(NSNotification *)notification {
    @try {
        NSLog(@"GameOfWhalesProxy didRegisterForRemoteNotificationsWithDeviceToken");
    
        NSData * deviceToken = (NSData *)[notification userInfo];
        const char *data = (const char *) [deviceToken bytes];
        NSMutableString *token = [NSMutableString string];
    
        for (NSUInteger i = 0; i < [deviceToken length]; i++) {
            [token appendFormat:@"%02.2hhX", data[i]];
        }
    
        NSString * tokenResult = [token copy];
        NSLog(@"GameOfWhalesProxy IOS %@", tokenResult);
        [GW RegisterDeviceTokenWithString:tokenResult provider:GW_PROVIDER_APN];
    }
    @catch(NSException* e)
    {
        NSLog(@"GW:didRegisterForRemoteNotificationsWithDeviceToken %@", e.reason);
    }
}

- (void) ApplicationDidFinishLaunchingNotification:(NSNotification *)notification
{
    @try
    {
        NSLog(@"GameOfWhalesProxy ApplicationDidFinishLaunchingNotification");
        NSDictionary * launchOptions = [notification userInfo];
        gw_launchOptions = [[NSDictionary alloc] initWithDictionary:launchOptions];
    }
    @catch(NSException* e)
    {
        NSLog(@"GW:ApplicationDidFinishLaunchingNotification %@", e.reason);
    }
}


- (void)specialOfferAppeared:(GWSpecialOffer *)specialOffer
{
    @try {
        NSLog(@"GameOfWhalesProxy specialOfferAppeared");
     
        /*NSDictionary* dict = @{
            @"camp":specialOffer.campaign,
            @"product":specialOffer.product,
            @"countFactor": @(specialOffer.countFactor),
            @"priceFactor": @(specialOffer.priceFactor),
            @"finishedAt": @([specialOffer.finishedAt timeIntervalSince1970] * 1000),
            @"payload":specialOffer.payload};*/
        
        gameofwhales::SpecialOffer *offer_p = new SpecialOffer();
        offer_p->campaign = std::string([specialOffer.campaign UTF8String], [specialOffer.campaign lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        offer_p->product = std::string([specialOffer.product UTF8String], [specialOffer.product lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        offer_p->payload = std::string([specialOffer.payload UTF8String], [specialOffer.payload lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        offer_p->countFactor = specialOffer.countFactor;
        offer_p->priceFactor = specialOffer.priceFactor;
        offer_p->redeemable = specialOffer.redeemable;
        offer_p->finishedAt = [specialOffer.finishedAt timeIntervalSince1970];
        for (NSString *key in specialOffer.customValues) {
            
            NSObject * obj = [specialOffer.customValues objectForKey:key];
            NSString * value = [NSString stringWithFormat:@"%@", obj];
            offer_p->customValues[[key UTF8String]] = [value UTF8String];
        }
        
        
        //specialOffer.cus
        
        gameofwhales::internal::notify_specialOfferAppeared(offer_p);
        
        //[self unitySendMethod:@"Internal_OnSpecialOfferAppeared" param:dict];
    }
    @catch(NSException* e)
    {
        NSLog(@"GW:specialOfferAppeared %@", e.reason);
    }
}

- (void)specialOfferDisappeared:(GWSpecialOffer *)specialOffer
{
    @try {
        NSLog(@"GameOfWhalesProxy specialOfferDisappeared");
    
        std::string product = std::string([specialOffer.product UTF8String], [specialOffer.product lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
    
        gameofwhales::internal::notify_specialOfferDisappeared(product);
    }
    @catch(NSException* e)
    {
        NSLog(@"GW:specialOfferDisappeared %@", e.reason);
    }
}


- (void)onPushDelivered:(nullable GWSpecialOffer*)offer camp:(nonnull NSString *)camp title:(nonnull NSString*)title message:(nonnull NSString*)message
{
    @try {
        NSLog(@"GameOfWhalesProxy onPushDelivered");

        NSMutableDictionary * dict = [[NSMutableDictionary alloc] initWithCapacity:5];
        [dict setValue:camp forKey:@"camp"];
        [dict setValue:title forKey:@"title"];
        [dict setValue:message forKey:@"message"];
        //[self unitySendMethod:@"Internal_OnPushDelivered" param:dict];
        
        std::string camp_str = "";
        if (camp)
        {
            camp_str = std::string([camp UTF8String], [camp lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        }
        
        std::string title_str = std::string([title UTF8String], [title lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        std::string message_str = std::string([message UTF8String], [message lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        
        std::string offerProduct;
        const SpecialOffer * so = 0;
        if (offer != nil)
        {
            offerProduct = std::string([offer.product UTF8String], [offer.product lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
            
            so = gameofwhales::getSpecialOffer(offerProduct.c_str());
        }

        
        gameofwhales::internal::notify_onPushDelivered(so, camp_str.c_str(), title_str.c_str(), message_str.c_str());
    }
    @catch(NSException* e)
    {
        NSLog(@"GW:onPushDelivered %@", e.reason);
    }
}

- (void)onPurchaseVerified:(NSString *)transactionID state:(NSString *)state
{
    @try {
        NSLog(@"GameOfWhalesProxy onPurchaseVerified");
        
        //NSMutableDictionary * dict = [[NSMutableDictionary alloc] initWithCapacity:5];
        //[dict setValue:transactionID forKey:@"transactionID" ];
        //[dict setValue:state forKey:@"verifyState" ];
        
        std::string transactionID_str = std::string([transactionID UTF8String], [transactionID lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        std::string state_str = std::string([state UTF8String], [state lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        
        gameofwhales::PURCHASE_STATE state = gameofwhales::PURCHASE_STATE::UNDEFINED;
        if (state_str == VERIFY_STATE_ILLEGAL)
        {
            state = gameofwhales::PURCHASE_STATE::ILLEGAL;
        }
        
        if (state_str == VERIFY_STATE_UNDEFINED)
        {
            state = gameofwhales::PURCHASE_STATE::LEGAL;
        }
        
        gameofwhales::internal::notify_onPurchaseVerified(transactionID_str.c_str(), state);
    }
    @catch(NSException* e)
    {
        NSLog(@"GW:onPurchaseVerified %@", e.reason);
    }
}


@end






///C++ PART -----------------------------------
namespace gameofwhales {
    namespace native {
        
        void _initialize(const char* gameKey, const char* version, const char* platform, bool debug)
        {
            @try
            {
                GW_CHECK_CHAR(gameKey, initialize);
                GW_CHECK_CHAR(version, initialize);
                GW_CHECK_CHAR(platform, initialize);
            
                NSString *nsGameKey = [[NSString alloc] initWithUTF8String:gameKey];
                NSString *nsVersion = [[NSString alloc] initWithUTF8String:version];
                NSString *nsPlatform = [[NSString alloc] initWithUTF8String:platform];
            
                GW_PLATFORM = nsPlatform;
                GW_VERSION = nsVersion;
                [GW InitializeWithGameKey:nsGameKey :gw_launchOptions :debug];
                [GW AddDelegate:gw_proxyInstance];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_initialize %@", e.reason);
            }
        }
    
        
        void _inAppPurchased(const char* sku, float price, const char* currency, const char* transactionID, const char* receipt)
        {
            @try
            {
                GW_CHECK_CHAR(sku, inAppPurchased);
                GW_CHECK_CHAR(currency, inAppPurchased);
                GW_CHECK_CHAR(transactionID, inAppPurchased);
                GW_CHECK_CHAR(receipt, inAppPurchased);
                
                NSString * nsSku = [[NSString alloc] initWithUTF8String:sku];
                NSString * nsCurrency = [[NSString alloc] initWithUTF8String:currency];
                NSString * nsTransactionID = [[NSString alloc] initWithUTF8String:transactionID];
                NSString * nsReceipt = [[NSString alloc] initWithUTF8String:receipt];
                
                [GW InAppPurchased:nsSku :price :nsCurrency :nsTransactionID :nsReceipt];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_inAppPurchased %@", e.reason);
            }
        }
        
        
        void _updateToken(const char* token, const char* provider)
        {
            @try
            {
                GW_CHECK_CHAR(token, updateToken);
                GW_CHECK_CHAR(provider, updateToken);
        
                NSString *nsToken = [[NSString alloc] initWithUTF8String:token];
                NSString *nsProvider = [[NSString alloc] initWithUTF8String:provider];
                
                [GW RegisterDeviceTokenWithString:nsToken provider:nsProvider];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_updateToken %@", e.reason);
            }
        }
        
        
        void _pushReacted(const char* camp)
        {
            if (!camp)
                return;
            
            @try
            {
                NSString *nsCampID = [[NSString alloc] initWithUTF8String:camp];
                [GW ReactedRemoteNotificationWithCampaign:nsCampID];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_pushReacted %@", e.reason);
            }
        }
        
        
        
        void _converting(const char* json_resources, const char *place)
        {
            @try
            {
                GW_CHECK_CHAR(json_resources, converting);
                GW_CHECK_CHAR(place, converting);
                
                NSString *nsResources = [[NSString alloc] initWithUTF8String:json_resources];
                NSString *nsPlace = [[NSString alloc] initWithUTF8String:place];
                [GW ConvertingWithString:nsResources place:nsPlace];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_converting %@", e.reason);
            }
        }
        
        
        void _profile(const char* json_changed_params)
        {
            @try
            {
                GW_CHECK_CHAR(json_changed_params, profile);
                
                NSString *nsParams = [[NSString alloc] initWithUTF8String:json_changed_params];
                [GW ProfileWithString:nsParams];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_profile %@", e.reason);
            }
        }
        
        
        void _consume(const char* currency, int number, const char* sink, int amount, const char* place)
        {
            @try
            {
                GW_CHECK_CHAR(currency, consume);
                GW_CHECK_CHAR(sink, consume);
                GW_CHECK_CHAR(place, consume);

                NSString *nsCurrency = [[NSString alloc] initWithUTF8String:currency];
                NSString *nsSink = [[NSString alloc] initWithUTF8String:sink];
                NSString *nsPlace = [[NSString alloc] initWithUTF8String:place];
                
                NSNumber *nsNumber = [NSNumber numberWithInt:number];
                NSNumber *nsAmount = [NSNumber numberWithInt:amount];
                
                [GW ConsumeCurrency:nsCurrency number:nsNumber sink:nsSink amount:nsAmount place:nsPlace];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_consume %@", e.reason);
            }
        }
        
        
        void _acquire(const char* currency, int amount, const char* source, int number, const char* place)
        {
            @try
            {
                GW_CHECK_CHAR(currency, acquire);
                GW_CHECK_CHAR(source, acquire);
                GW_CHECK_CHAR(place, acquire);
                
                NSString *nsCurrency = [[NSString alloc] initWithUTF8String:currency];
                NSString *nsSource = [[NSString alloc] initWithUTF8String:source];
                NSString *nsPlace = [[NSString alloc] initWithUTF8String:place];
                
                NSNumber *nsAmount = [NSNumber numberWithInt:amount];
                NSNumber *nsNumber = [NSNumber numberWithInt:number];
                
                [GW AcquireCurrency:nsCurrency amount:nsAmount source:nsSource number:nsNumber place:nsPlace];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_acquire %@", e.reason);
            }
        }
        
        
        void _reportError(const char* message, const char* stacktrace)
        {
            @try
            {
                GW_CHECK_CHAR(message, reportError);
                GW_CHECK_CHAR(stacktrace, reportError);
                
                NSString * nsMessage = [[NSString alloc] initWithUTF8String:message];
                NSString * nsStacktrace = [[NSString alloc] initWithUTF8String:stacktrace];
                [GW ReportError:nsMessage :nsStacktrace];
            }
            @catch(NSException* e)
            {
                NSLog(@"GW:gameofwhales::native::_reportError %@", e.reason);
            }
        }
    
    
    }
}
#endif
