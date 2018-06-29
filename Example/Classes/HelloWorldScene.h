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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginIAP/PluginIAP.h"
#include <map>
#include <string>
#include <vector>

#include "PlayerData.h"


USING_NS_CC;

using namespace std;


struct BankButton
{
    MenuItemLabel * label;
    std::string     price;
    int             coins;
};

class HelloWorld : public cocos2d::Scene, public sdkbox::IAPListener, public gameofwhales::Listener
{
public:
    const float item1_price = 500;
    const float item2_price = 1500;
    
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuLeftCallback(Ref* pSender);
    void menuRightCallback(Ref* pSender);
    
    void menuProduct5Callback(Ref* pSender);
    void menuProduct10Callback(Ref* pSender);
    void menuProduct20Callback(Ref* pSender);
    void menuMessageCallback(Ref* pSender);
    
    void menuBuyItem1(Ref* pSender);
    void menuBuyItem2(Ref* pSender);
    void menuNextLocation(Ref* pSender);
    void menuLevelUp(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void onInitialized(bool ok) override;
    void onSuccess(sdkbox::Product const& p) override;
    void onFailure(sdkbox::Product const& p, const std::string &msg) override;
    void onCanceled(sdkbox::Product const& p) override;
    void onRestored(sdkbox::Product const& p) override;
    void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
    void onProductRequestFailure(const std::string &msg) override;
    void onRestoreComplete(bool ok, const std::string &msg) override;
    
    void onPushDelivered(const gameofwhales::SpecialOffer * so, const char* camp, const char* title, const char* message) override;
    void specialOfferAppeared(const gameofwhales::SpecialOffer& offer) override;
    void specialOfferDisappeared(const gameofwhales::SpecialOffer& offer) override;
private:
    string formatedLeftTime(long left);
    
    const int ITEM1_COST = 500;
    const int ITEM2_COST = 1500;
    
    PlayerData _playerData;
    
    Menu * _bank_holder = 0;
    Menu * _player_holder = 0;
    Menu * _message_holder = 0;
    
    MenuItemLabel * _playerClassLabel = 0;
    MenuItemLabel * _playerGenderLabel = 0;
    MenuItemLabel * _playerCoinsLabel = 0;
    
    Label * _nextLocationLabel = 0;
    Label * _levelupLabel = 0;
    Label * _messageTitle = 0;
    Label * _messageMessage = 0;
    Label * _buyItem1Label = 0;
    Label * _buyItem2Label = 0;
    
    MenuItemLabel * _btnNextLevel = 0;
    
    typedef map<string, BankButton>  bankdata;
    bankdata _bankLabels;
    vector<Menu*> _pages;
    int _currentPage = 1;
    
    void updatePages();
    void updatePlayerTexts(bool sendInfo);
    void onUpdate(float);
    
    string _camp;
};

#endif // __HELLOWORLD_SCENE_H__
