#ifndef __GAME_OF_WHALES__COCOS__HEADER__
#define __GAME_OF_WHALES__COCOS__HEADER__

/* Cocos specific defines */

#include "cocos2d.h"


#define GAMEOFWHALES_PLATFORM "cocos2dcpp"


//IOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define GAMEOFWHALES_ANDROID 1

//ANDROID
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #define GAMEOFWHALES_IOS 1

#else//UNSUPPORTED
    #define GAMEOFWHALES_UNSUPPORTED 1
#endif



#endif//__GAME_OF_WHALES__COCOS__HEADER__
