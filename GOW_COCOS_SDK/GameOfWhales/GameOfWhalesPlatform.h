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

#ifndef __GAME_OF_WHALES__COCOS__HEADER__
#define __GAME_OF_WHALES__COCOS__HEADER__

/* Cocos specific defines */

#include "cocos2d.h"


#define GAMEOFWHALES_PLATFORM "cocos2dcpp"



//ANDROID
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define GAMEOFWHALES_ANDROID 1

//IOS
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #define GAMEOFWHALES_IOS 1

//UNSUPPORTED
#else
    #define GAMEOFWHALES_UNSUPPORTED 1

#endif



#endif//__GAME_OF_WHALES__COCOS__HEADER__
