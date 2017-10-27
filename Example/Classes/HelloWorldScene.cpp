#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOfWhales/GameOfWhales.h"




#define COCOS2D_DEBUG 1
#define DEFAULT_FONT "fonts/Marker Felt.ttf"




Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    gameofwhales::initialize("YOUR_GAMEKEY", true);
    gameofwhales::addListener(this);
    
    _playerData.load();
    
    sdkbox::IAP::init();
    sdkbox::IAP::setDebug(true);
    sdkbox::IAP::setListener(this);
    sdkbox::IAP::refresh();
    
    CCLOG("HelloWorld init");
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float scale = Director::getInstance()->getOpenGLView()->getViewPortRect().getMaxY() /  Director::getInstance()->getWinSize().height;
    if (scale > 2.0f)
        scale /= 1.5f;
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto button_left = MenuItemImage::create("button-left.png", "button-left.png", CC_CALLBACK_1(HelloWorld::menuLeftCallback, this));
    button_left->setPosition(Vec2(origin.x + button_left->getContentSize().width/2 , origin.y + visibleSize.height / 2));
    
    auto button_right = MenuItemImage::create("button-right.png", "button-right.png", CC_CALLBACK_1(HelloWorld::menuRightCallback, this));
    button_right->setPosition(Vec2(origin.x + visibleSize.width - button_right->getContentSize().width/2, origin.y + visibleSize.height / 2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->addChild(button_left);
    menu->setPosition(Vec2::ZERO);
    menu->addChild(button_right);
    this->addChild(menu, 1);
    
    
    /*auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    this->addChild(label, 1);*/
    
    auto btn_messagebox = MenuItemImage::create("msgbox.png", "msgbox.png", CC_CALLBACK_1(HelloWorld::menuMessageCallback, this));
    btn_messagebox->setPosition(Vec2::ZERO);
    btn_messagebox->setScale(scale);
    
    _messageTitle = Label::createWithTTF("Title", DEFAULT_FONT, 8);
    _messageTitle->setAlignment(cocos2d::TextHAlignment::CENTER);
    _messageTitle->setColor(Color3B::BLACK);
    btn_messagebox->addChild(_messageTitle);
    _messageTitle->setPosition(Vec2(btn_messagebox->getContentSize().width / 2.5, btn_messagebox->getContentSize().height / 1.4));
    
    _messageMessage = Label::createWithTTF("Message", DEFAULT_FONT, 6);
    _messageMessage->setAlignment(cocos2d::TextHAlignment::CENTER);
    _messageMessage->setColor(Color3B::BLACK);
    btn_messagebox->addChild(_messageMessage);
    _messageMessage->setPosition(Vec2(btn_messagebox->getContentSize().width / 2, btn_messagebox->getContentSize().height / 2));
    
    
    _message_holder = Menu::create(btn_messagebox, NULL);
    _message_holder->setVisible(false);
    this->addChild(_message_holder, 5);
    
    
    ///Create PANEL PRODUCT 5
    auto product_5 = MenuItemImage::create("panel.png", "panel.png", CC_CALLBACK_1(HelloWorld::menuProduct5Callback, this));
    product_5->setPosition(Vec2(-product_5->getContentSize().width * 1.05,0));
    Label * product_5_label = Label::createWithTTF("Product 5", DEFAULT_FONT, 14);
    product_5_label->setAlignment(cocos2d::TextHAlignment::CENTER);
    product_5_label->setPosition(Vec2(product_5->getContentSize().width / 2, product_5->getContentSize().height / 2));
    auto product_5_text = MenuItemLabel::create(product_5_label, nullptr);
    BankButton product_5_data;
    product_5_data.label = product_5_text;
    product_5_data.coins = 500;
    _bankLabels["product_5"] = product_5_data;
    product_5->addChild(product_5_text);
    
    
    ///Create PANEL PRODUCT 10
    auto product_10 = MenuItemImage::create("panel.png", "panel.png", CC_CALLBACK_1(HelloWorld::menuProduct10Callback, this));
    product_10->setPosition(Vec2(0,0));
    Label * product_10_label = Label::createWithTTF("Product 10", DEFAULT_FONT, 14);
    product_10_label->setAlignment(cocos2d::TextHAlignment::CENTER);
    product_10_label->setPosition(Vec2(product_10->getContentSize().width / 2, product_10->getContentSize().height / 2));
    auto product_10_text = MenuItemLabel::create(product_10_label, nullptr);
    BankButton product_10_data;
    product_10_data.label = product_10_text;
    product_10_data.coins = 1500;
    _bankLabels["product_10"] = product_10_data;
    product_10->addChild(product_10_text);
    
    auto product_20 = MenuItemImage::create("panel.png", "panel.png", CC_CALLBACK_1(HelloWorld::menuProduct20Callback, this));
    product_20->setPosition(Vec2(product_20->getContentSize().width * 1.05, 0));
    Label * product_20_label = Label::createWithTTF("Product 20", DEFAULT_FONT, 14);
    product_20_label->setAlignment(cocos2d::TextHAlignment::CENTER);
    product_20_label->setPosition(Vec2(product_20->getContentSize().width / 2, product_20->getContentSize().height / 2));
    auto product_20_text = MenuItemLabel::create(product_20_label, nullptr);
    BankButton product_20_data;
    product_20_data.label = product_20_text;
    product_20_data.coins = 5000;
     _bankLabels["product_20"] = product_20_data;
    product_20->addChild(product_20_text);
    
    _bank_holder = Menu::create(product_5, product_10, product_20, NULL);
    this->addChild(_bank_holder, 1);
    _pages.push_back(_bank_holder);
    
    ///Player Data
    Label * playerClass = Label::createWithTTF("Class: " + _playerData.getSpeciality(), DEFAULT_FONT, 16);
    playerClass->setAlignment(cocos2d::TextHAlignment::CENTER);
    playerClass->setPosition(Vec2(0, visibleSize.height/2 - playerClass->getContentSize().height));
    _playerClassLabel = MenuItemLabel::create(playerClass, nullptr);
    
    string gender = "Gender: ";
    if (_playerData.getGender())
        gender+="Man";
    else
        gender+="Woman";
    
    Label * playerGender = Label::createWithTTF(gender, DEFAULT_FONT, 16);
    playerGender->setAlignment(cocos2d::TextHAlignment::CENTER);
    playerGender->setPosition(Vec2(0, visibleSize.height/2 - playerGender->getContentSize().height*2));
    _playerGenderLabel = MenuItemLabel::create(playerGender, nullptr);
    
    char coins_c[250];
    sprintf(coins_c, "Coins: %d", _playerData.getCoins());
    Label * playerCoins = Label::createWithTTF(coins_c, DEFAULT_FONT, 16);
    playerCoins->setAlignment(cocos2d::TextHAlignment::CENTER);
    playerCoins->setPosition(Vec2(0, visibleSize.height/2 - playerCoins->getContentSize().height*3));
    _playerCoinsLabel = MenuItemLabel::create(playerCoins, nullptr);
    
    
    
    auto btn_buy_item1 = MenuItemImage::create("button.png", "button_pressed.png", CC_CALLBACK_1(HelloWorld::menuBuyItem1, this));
    btn_buy_item1->setPosition(Vec2(0, btn_buy_item1->getContentSize().height * 1.05));
    
    _buyItem1Label = Label::createWithTTF("Buy ITEM1 for 500", DEFAULT_FONT, 14);
    btn_buy_item1->addChild(_buyItem1Label);
    _buyItem1Label->setPosition(Vec2(btn_buy_item1->getContentSize().width/2, btn_buy_item1->getContentSize().height/2));
    _buyItem1Label->setAlignment(cocos2d::TextHAlignment::CENTER);
    
    
    auto btn_buy_item2 = MenuItemImage::create("button.png", "button_pressed.png", CC_CALLBACK_1(HelloWorld::menuBuyItem2, this));
    btn_buy_item2->setPosition(Vec2(0, -btn_buy_item2->getContentSize().height * 0.05));
    
    _buyItem2Label = Label::createWithTTF("Buy ITEM2 for 1500", DEFAULT_FONT, 14);
    btn_buy_item2->addChild(_buyItem2Label);
    _buyItem2Label->setPosition(Vec2(btn_buy_item1->getContentSize().width/2, btn_buy_item1->getContentSize().height/2));
    _buyItem2Label->setAlignment(cocos2d::TextHAlignment::CENTER);

    
    
    
    auto btn_next_location = MenuItemImage::create("button.png", "button_pressed.png", CC_CALLBACK_1(HelloWorld::menuNextLocation, this));
    btn_next_location->setPosition(Vec2(0, -btn_next_location->getContentSize().height * 1.15));
    
    _nextLocationLabel = Label::createWithTTF("NEXT LOCATION (A)", DEFAULT_FONT, 14);
    btn_next_location->addChild(_nextLocationLabel);
    _nextLocationLabel->setPosition(Vec2(btn_next_location->getContentSize().width/2, btn_next_location->getContentSize().height/2));
    _nextLocationLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
    
    auto btn_level_up = MenuItemImage::create("button.png", "button_pressed.png", CC_CALLBACK_1(HelloWorld::menuLevelUp, this));
    btn_level_up->setPosition(Vec2(0, -btn_level_up->getContentSize().height * 2.15));
    
    _levelupLabel = Label::createWithTTF("LEVEL UP (1)", DEFAULT_FONT, 14);
    btn_level_up->addChild(_levelupLabel);
    _levelupLabel->setPosition(Vec2(btn_level_up->getContentSize().width/2, btn_level_up->getContentSize().height/2));
    _levelupLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
    
    
    _player_holder = Menu::create(_playerClassLabel, _playerGenderLabel, _playerCoinsLabel, btn_buy_item1, btn_buy_item2, btn_next_location,btn_level_up, NULL);
    this->addChild(_player_holder);
    _pages.push_back(_player_holder);
    
    
    
    this->schedule(schedule_selector(HelloWorld::onUpdate), 1.0f);
    
    
    updatePages();
    
    return true;
}

string HelloWorld::formatedLeftTime(long left)
{
    char buffer [100];
    std::time_t t = left;
    struct tm * timeinfo = gmtime(&t);
    std::strftime(buffer, 100, "%X", timeinfo);
    return buffer;
}

void HelloWorld::onPushDelivered(const char* camp, const char* title, const char* message)
{
    _messageTitle->setString(title);
    _messageMessage->setString(message);
    
    gameofwhales::pushReacted(camp);
    
    _message_holder->setVisible(true);
}


void HelloWorld::menuMessageCallback(Ref* pSender)
{
     _message_holder->setVisible(false);
}

void HelloWorld::onUpdate(float)
{
    updatePlayerTexts(false);
}

void HelloWorld::updatePlayerTexts(bool sendInfo)
{
    char text[512];
    
    const gameofwhales::SpecialOffer * offer = gameofwhales::getSpecialOffer("item1");
   
    float price = item1_price;
    
    if (offer && offer->hasPriceFactor() && !offer->isExpiried())
    {
        price *= offer->priceFactor;
        sprintf(text, "BUY ITEM 1 FOR %.0f (-%d%%) \n left: %s", price, 100 - int(offer->priceFactor * 100.0f), formatedLeftTime(offer->getLeftTime()).c_str());
    }
    else
    {
         sprintf(text, "BUY ITEM 1 FOR %.0f", price);
    }
     _buyItem1Label->setString(text);
    
    
    
    price = item2_price;
    
    
    offer = gameofwhales::getSpecialOffer("item2");
    
    if (offer && offer->hasPriceFactor() && !offer->isExpiried())
    {
        price *= offer->priceFactor;
        sprintf(text, "BUY ITEM 2 FOR %.0f (-%d%%) \n left: %s", price, 100 - int(offer->priceFactor * 100.0f), formatedLeftTime(offer->getLeftTime()).c_str());
    }
    else
    {
         sprintf(text, "BUY ITEM 2 FOR %.0f", price);
    }
    
    
    _buyItem2Label->setString(text);
    
    
    sprintf(text, "Coins: %d", _playerData.getCoins());
    _playerCoinsLabel->setString(text);
    
    sprintf(text, "NEXT LOCATION (%s)", _playerData.getLocation());
    _nextLocationLabel->setString(text);
    
    sprintf(text, "NEXT LEVEL (%d)", _playerData.getLevel());
    _levelupLabel->setString(text);
    
    
    offer = gameofwhales::getSpecialOffer("item2");
    
    for (bankdata::iterator it=_bankLabels.begin(); it!=_bankLabels.end(); ++it)
    {
        const string &id = it->first;
        BankButton& btn = it->second;
        
        MenuItemLabel * text_item = btn.label;
        
        
        
        offer = gameofwhales::getSpecialOffer(id.c_str());
        bool useOffer = offer && offer->hasCountFactor() && !offer->isExpiried();
        float coins = btn.coins;
        
        if (useOffer)
        {
            coins *= offer->countFactor;
        }
        
        char text_c[50];
        sprintf(text_c, "%d", (int) coins);
        
        string text = id + "\nCoins: " + text_c;
        
        if (useOffer)
        {
            sprintf(text_c, " (+ %d%%)", (int) (100.0f - offer->countFactor * 100.0f));
            text = text + text_c;
        }
        
        text = text + "\n\n for " + btn.price.c_str();
        
        if (text_item)
            text_item->setString(text);
    }
    
    
    //Update GameOfWhales User Profile
    if (sendInfo)
    {
        //gameofwhales::profile_changes_type profile;
        gameofwhales::json::JSON data;
        data["coins"] = _playerData.getCoins();
        data["gender"] = _playerData.getGender();
        data["location"] = _playerData.getLocation();
        data["class"] = _playerData.getSpeciality();
        gameofwhales::profile(data.dump().c_str());
    }
    
    
    
}

void HelloWorld::menuBuyItem1(Ref* pSender)
{
    if (_playerData.getCoins() >= ITEM1_COST)
    {
        _playerData.addCoins(-ITEM1_COST);
        updatePlayerTexts(true);
        gameofwhales::consume("coin", ITEM1_COST, "item1", 1, "shop");
    }
}

void HelloWorld::menuBuyItem2(Ref* pSender)
{
    if (_playerData.getCoins() >= ITEM2_COST)
    {
        _playerData.addCoins(-ITEM2_COST);
        updatePlayerTexts(true);
        gameofwhales::consume("coin", ITEM2_COST, "item2", 1, "shop");
    }
}

void HelloWorld::menuNextLocation(Ref* pSender)
{
    _playerData.nextLocation();
    updatePlayerTexts(true);
}

void HelloWorld::menuLevelUp(Ref* pSender)
{
    _playerData.nextLevel();
    updatePlayerTexts(true);
}

void HelloWorld::menuProduct5Callback(Ref* pSender)
{
    CCLOG("HelloWorld::menuProduct5Callback");
    sdkbox::IAP::purchase("product_5");
}

void HelloWorld::menuProduct10Callback(Ref* pSender)
{
    CCLOG("HelloWorld::menuProduct10Callback");
    sdkbox::IAP::purchase("product_10");
}


void HelloWorld::menuProduct20Callback(Ref* pSender)
{
    CCLOG("HelloWorld::menuProduct20Callback");
    sdkbox::IAP::purchase("product_20");
}

void HelloWorld::menuRightCallback(Ref* pSender)
{
    _currentPage++;
    if (_pages.size() <= _currentPage)
        _currentPage = 0;
    
    updatePages();
}


void HelloWorld::menuLeftCallback(Ref* pSender)
{
    _currentPage--;
    if (_currentPage < 0)
        _currentPage = (int)(_pages.size() - 1);
    
    updatePages();
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

void HelloWorld::updatePages()
{
    int current = 0;
    for (auto it = _pages.begin(); it != _pages.end(); ++it)
    {
        (*it)->setVisible(current == _currentPage);
        current ++;
    }
}


void HelloWorld::onInitialized(bool ok)
{
    CCLOG("IAP HelloWorld::onInitialized");
};

void HelloWorld::onSuccess(sdkbox::Product const& p)
{
    int coins = 0;
    if (p.id == "product_5")
    {
        coins = 1000;
        //_playerData.addCoins(1000);
    }
    
    if (p.id == "product_10")
    {
        coins = 5000;
        //_playerData.addCoins(5000);
    }
    
    if (p.id == "product_20")
    {
        coins = 10000;
        //_playerData.addCoins(10000);
    }
    
    _playerData.addCoins(coins);
    
    const char * currency = "coins";
    int value = coins;
    const char * itemName = p.id.c_str();
    int amount = 1;
    const char * place = "bank";
    gameofwhales::acquire(currency, value, itemName, amount, place);
    
    std::string receiptJSON = gameofwhales::buildReceipt(gameofwhales::STORE_APPSTORE, p.transactionID.c_str(), p.receiptCipheredPayload.c_str());

    CCLOG("IAP HelloWorld::onSuccess receipt_orig: %s", p.receipt.c_str());
    CCLOG("IAP HelloWorld::onSuccess %s %f %s %s %s ", p.id.c_str(), p.priceValue, p.currencyCode.c_str(), p.transactionID.c_str(), receiptJSON.c_str());
    
    const char * sku = p.id.c_str();
    float price = p.priceValue;
    const char * currencyCode = p.currencyCode.c_str();
    const char * transactionID = p.transactionID.c_str();
    gameofwhales::inAppPurchased(sku, price, currencyCode, transactionID, receiptJSON.c_str());
};

void HelloWorld::onFailure(sdkbox::Product const& p, const std::string &msg)
{
     CCLOG("IAP HelloWorld::onFailure %s : %s", p.description.c_str(), msg.c_str());
};


void HelloWorld::onCanceled(sdkbox::Product const& p)
{
    CCLOG("IAP HelloWorld::onCanceled %s", p.description.c_str());
};


void HelloWorld::onRestored(sdkbox::Product const& p)
{
    CCLOG("IAP HelloWorld::onRestored %s", p.description.c_str());
};

void HelloWorld::onProductRequestSuccess(std::vector<sdkbox::Product> const &products)
{
    CCLOG("IAP HelloWorld::onProductRequestSuccess");
    
    for (auto it = products.begin(); it != products.end(); ++it)
    {
        const sdkbox::Product &p = (*it);
        BankButton& item = _bankLabels[p.id];
        item.price = p.price;
        CCLOG("IAP HelloWorld::onProductRequestSuccess, product: %s", (*it).id.c_str());
    }
    
    updatePlayerTexts(false);
};

void HelloWorld::onProductRequestFailure(const std::string &msg)
{
    CCLOG("IAP HelloWorld::onProductRequestFailure: %s", msg.c_str());

};

void HelloWorld::onRestoreComplete(bool ok, const std::string &msg)
{
    CCLOG("IAP HelloWorld::onRestoreComplete: ok:%d  msg:%s", ok, msg.c_str());
};
