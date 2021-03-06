# GameOfWhales Cocos2D SDK

Supported platforms: **IOS**

Supported lanuages: **C++**

Requirements: **Cocos2D-X: 3.x+**, **iOS 7+**
    
    
# Changelog
 
### 2.0.14 (Jun 29, 2018)

ADDED

* _Custom data_ is supported for special offers.
* The information about a device's locale is sent to **Game of Whales**.

MODIFIED

* Push notification about special offer comes at the same time with the special offer (new parameter ``offer`` was added):

```cpp
void onPushDelivered(const gameofwhales::SpecialOffer * so, const char* camp, const char* title, const char* message)
```

* Static methods were added instead of ``shared`` methods.
    

# Implementation Guide

## SETUP

Download the latest sdk version from our server:

[<img src=https://github.com/Game-of-whales/GOW-SDK-UNITY/wiki/img/download.png>](https://github.com/Game-of-whales/GOW-SDK-COCOS2D-CPP/releases/download/v2.0.14/gow-cocos-sdk-2.0.14.zip)

### C++ Installation

Copy files

1) from ``[GOW_COCOS_SDK]/Classes/`` to ``[YOUR_PROJECT_FOLDLER]/Classes/``
2) from ``[GOW_COCOS_SDK]/proj.ios_mac/`` to ``[YOUR_PROJECT_FOLDLER]/proj.ios_mac/``


### Configure XCODE

Add ``[YOUR_PROJECT_FOLDLER]/Classes/`` to your project:

<img src=http://www.gameofwhales.com/sites/default/files/documentation/cocos_AddFilesToProject.png>

Add ``[YOUR_PROJECT_FOLDLER]/proj.ios_mac/GameOfWhales.framework`` to your project:

<img src=http://www.gameofwhales.com/sites/default/files/documentation/cocos_LinkBinaryWithLibrary.png>

In ``Info.plist`` set ``App Transport Security Settings/Allow Arbitrary Loads`` to **YES**:

<img src=http://www.gameofwhales.com/sites/default/files/documentation/cocos_AppTransportSecuritySettings.png>



# Code integration

**C++**

Initialize SDK with your [game key](https://www.gameofwhales.com/documentation/game).
```cpp
#include "GameOfWhales/GameOfWhales.h"
...
bool debugLogging = true;
gameofwhales::initialize("YOUR_GAME_KEY", debugLogging);
```


Add SDK's listener
```cpp
class HelloWorld : public gameofwhales::Listener

bool HelloWorld::init()
{
    gameofwhales::addListener(this);
```

Implement needed listener's methods 
```cpp
        void specialOfferAppeared(const SpecialOffer& offer) override;
        void specialOfferDisappeared(const SpecialOffer& offer) override;
        void onPushDelivered(const gameofwhales::SpecialOffer * so, const char* camp, const char* title, const char* message) override;
        void onPurchaseVerified(const char* transactionID, gameofwhales::PURCHASE_STATE state) override;
```

The purchase state can be:

* _VERIFY_STATE_LEGAL_ - a purchase is normal.
* _VERIFY_STATE_ILLEGAL_ - a purchase is a cheater's.
* _VERIFY_STATE_UNDEFINED_ - GOW server couldn't define the state of a purchase. 


## Game Events

Send information about IAPs, for example, by using **``BOXSDK``**:
```cpp
void HelloWorld::onSuccess(sdkbox::Product const& p)
{
    std::string receiptJSON = gameofwhales::buildReceipt(gameofwhales::STORE_APPSTORE, p.transactionID.c_str(), p.receiptCipheredPayload.c_str());
    const char * sku = p.id.c_str();
    float price = p.priceValue;
    const char * currencyCode = p.currencyCode.c_str();
    const char * transactionID = p.transactionID.c_str();
    gameofwhales::inAppPurchased(sku, price, currencyCode, transactionID, receiptJSON.c_str());
```

>Check that *Android Bundle Identifier* and *Android Public License Key* have been filled on [*Game Settings*](https://www.gameofwhales.com/documentation/game#game-settings) page before you will make a purchase.


Send user data, for example:
```cpp
    gameofwhales::json::JSON data;
    data["coins"] = _playerData.getCoins();
    data["gender"] = _playerData.getGender();
    data["location"] = _playerData.getLocation();
    data["class"] = _playerData.getSpeciality();
    gameofwhales::profile(data.dump().c_str());
```


>If you send more than 3000 properties, **Game of Whales** will sort all properties alphabetically and will save only the first 3000.

>If the length of a string-type property is more than 64 characters, **Game of Whales** will save only the first 64 characters.


Send information about user currency consuming, for example:
```cpp
    const char * currency = "coins";
    int itemCost = 1000;
    const char * itemName = "item1";
    int amount = 1;
    const char * place = "shop";
    gameofwhales::consume("currency, itemCost, itemName, amount, place);
```
It means that someone spent 1000 "coins" for 1 "item1" in "shop".

Send information about user currency obtainment, for example:
```cpp
    const char * currency = "coins";
    int value =  5000;
    const char * itemName = "yourgame.product.iap";
    int amount = 1;
    const char * place = "bank";
    gameofwhales::acquire(currency, value, itemName, amount, place);
```
It means that someone has acquired 5000 "coins" for 1 "yourgame.product.iap" in "bank".



## Push notifications

You need to do this chapter steps only if you want to use [push notifications](https://www.gameofwhales.com/documentation/push-notifications) in your app.

In order to request permission for push notifications, add code to your **``AppDelegate``** class, for example:
```objc
UNAuthorizationOptions authOptions =
    UNAuthorizationOptionAlert
    | UNAuthorizationOptionSound
    | UNAuthorizationOptionBadge;
    [[UNUserNotificationCenter currentNotificationCenter] requestAuthorizationWithOptions:authOptions completionHandler:^(BOOL granted, NSError * _Nullable error) {
        if(!error){
            dispatch_async(dispatch_get_main_queue(), ^{
                 [[UIApplication sharedApplication] registerForRemoteNotifications];
            }); 
        }
    }];
```

Show message to user when app in foreground and send "reacted" event 
```cpp
void onPushDelivered(const char* camp, const char* title, const char* message)
{
    //show message box and then call:
    gameofwhales::pushReacted(camp);
}
```

To register device token, add next code to your **``AppDelegate``** class
```objc
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    [[GW shared] registerDeviceTokenWithData: deviceToken provider:GW_PROVIDER_APN];
}
```

In order to provide information about received notifications, add next code to your **``AppDelegate``** class:
```objc
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))completionHandler
{
    [[GW shared] receivedRemoteNotification:userInfo withApplication:application fetchCompletionHandler:completionHandler];
}
```

In order to check notifications implementation send [a test notification](https://www.gameofwhales.com/documentation/how-send-test-push-notification).


## Special Offers

Before any product can be used in a special offer it has to be bought by someone after SDK has been implemented into the game. Please make sure your game has at least one purchase of the product that is going to be used in the special offer.
If you want to create a special offer for in game resource, please, make sure your game has at least one _converting_ event with the appropriate resource.


You need to do this chapter steps only if you want to use [special offers](https://www.gameofwhales.com/documentation/special-offers) from **Game of Whales** in your app.

To get information about special offers changes implement next listener's methods:
```cpp
    void specialOfferAppeared(const SpecialOffer& offer) override;
    void specialOfferDisappeared(const SpecialOffer& offer) override;
```

To receive information about special offer for product call **``gameofwhales::getSpecialOffer``** method:
```cpp
    //buying some product
    const char* productID = "item1";
    float price = 1000;
    float count = 10;
    const gameofwhales::SpecialOffer * offer = gameofwhales::getSpecialOffer(productID);
    if (offer && !offer->isExpiried())
    {
        if (offer->hasPriceFactor() )
        {
            //change price for product
            price *= offer->priceFactor;
        }
        
        if (offer->hasCountFactor())
        {
            count *= offer->countFactor;
        }
        
        ...
    }
    
```

## Special offers custom data

It's possible to pass [custom data](https://www.gameofwhales.com/documentation/custom-data) to special offers. In order to get the data in game's side, use ``customValues`` parameter of ``SpecialOffer`` class.

```cpp
    string str = offer.customValues["your_string"];
    int number = atoi(offer.customValues["your_number"].c_str()); 
    bool boolean = atoi(offer.customValues["your_bool"].c_str()); 
```


> You can find an example of using the SDK [here](https://github.com/Game-of-whales/GOW-SDK-COCOS2D-CPP/tree/master/Example).

Run your app. The information about it began to be collected and displayed on the [dashboard](https://gameofwhales.com/documentation/dashboard). In a few days, you will get data for analyzing.

This article includes the documentation for _Game of Whales Cocos2D SDK_. You can find information about other SDKs in [documentation about Game of Whales](https://www.gameofwhales.com/documentation/download-setup).
