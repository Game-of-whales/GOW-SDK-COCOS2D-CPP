#include "GameOfWhalesAndroid.h"
#include "GameOfWhalesJSON.h"
#include "GameOfWhalesListener.h"
#include "GameOfWhales.h"
#include <jni.h>
#include <android/log.h>
#include <jni.h>
#include <string>
#include "cocos2d.h"

#define  LOG_TAG    "androidnative"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define JNI_NOT_NULL(v) if (!v) { __android_log_print(ANDROID_LOG_DEBUG, "GOWAndroid", "JNI POINTER IS NULL! %d %s", __LINE__, __FILE__); throw notFound();}

namespace gameofwhales
{

    extern char* VERIFY_STATE_LEGAL;
    extern char* VERIFY_STATE_ILLEGAL;
    extern char* VERIFY_STATE_UNDEFINED;

    class notFound {};

    namespace jni
    {
        std::string getString(JNIEnv* env, jstring jvalue)
        {
            if (!env)
            {
                return "";
            }

            const char* cvalue = env->GetStringUTFChars(jvalue, NULL);
            std::string result = "";
            if (cvalue)
            {
                result = cvalue;
            }
            env->ReleaseStringUTFChars(jvalue, cvalue);
            return result;
        }
    }
}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_gameofwhales_cocos2dproxy_GameOfWhalesProxy_nativeOnSpecialOfferAppeared
            (JNIEnv* env, jclass clazz, jstring jjsonData)
    {
        std::string offerJsonStr = gameofwhales::jni::getString(env, jjsonData);
        gameofwhales::json::JSON offerJson = gameofwhales::json::JSON::Load(offerJsonStr);

        std::string camp = offerJson["camp"].ToString();
        float countFactor = offerJson["countFactor"].ToFloat();
        float priceFactor = offerJson["priceFactor"].ToFloat();
        long finishedAt = offerJson["finishedAt"].ToInt();
        std::string product = offerJson["product"].ToString();
        std::string payload = offerJson["payload"].ToString();

        gameofwhales::SpecialOffer * offer = new gameofwhales::SpecialOffer();
        offer->campaign = camp;
        offer->countFactor = countFactor;
        offer->priceFactor = priceFactor;
        offer->finishedAt = finishedAt;
        offer->product = product;
        offer->payload = payload;

        gameofwhales::internal::notify_specialOfferAppeared(offer);
    }

    JNIEXPORT void JNICALL Java_com_gameofwhales_cocos2dproxy_GameOfWhalesProxy_nativeOnSpecialOfferDisappeared
        (JNIEnv* env, jclass clazz, jstring jjsonData)
    {
        std::string campID;
        gameofwhales::internal::notify_specialOfferDisappeared(campID);
    }

    JNIEXPORT void JNICALL Java_com_gameofwhales_cocos2dproxy_GameOfWhalesProxy_nativeOnPushDelivered
            (JNIEnv* env, jclass clazz, jstring jcamp, jstring jtitle, jstring jmessage)
    {
        std::string camp = gameofwhales::jni::getString(env, jcamp);
        std::string title = gameofwhales::jni::getString(env, jtitle);
        std::string message = gameofwhales::jni::getString(env, jmessage);

        gameofwhales::internal::notify_onPushDelivered(camp.c_str(), title.c_str(), message.c_str());

    }

    JNIEXPORT void JNICALL Java_com_gameofwhales_cocos2dproxy_GameOfWhalesProxy_nativeOnPurchaseVerified
            (JNIEnv* env, jclass clazz, jstring jtransactionID, jstring jstate)
    {
        std::string transactionID = gameofwhales::jni::getString(env, jtransactionID);
        std::string state_str = gameofwhales::jni::getString(env, jstate);
        gameofwhales::PURCHASE_STATE state = gameofwhales::PURCHASE_STATE::UNDEFINED;

        if (state_str == gameofwhales::VERIFY_STATE_LEGAL)
        {
            state = gameofwhales::PURCHASE_STATE::LEGAL;
        }

        if (state_str == gameofwhales::VERIFY_STATE_ILLEGAL)
        {
            state = gameofwhales::PURCHASE_STATE::ILLEGAL;
        }

        gameofwhales::internal::notify_onPurchaseVerified(transactionID.c_str(), state);
    }


}

namespace gameofwhales {
    
    jclass _proxyClass = 0;
    jobject _proxyInstance = 0;
    
    
    
    namespace native {

        bool check()
        {
            if (_proxyClass == 0 || _proxyInstance == 0)
            {


                return false;
            }

            return true;
        }
        
        void _initialize(const char* gameKey, const char* version, const char* platform, bool debug)
        {
            try {
                LOGD("gameofwhales_initialize native _initialize");
                LOGD("gameofwhales_initialize gamekay: %s", gameKey);
                LOGD("gameofwhales_initialize version: %s", version);
                LOGD("gameofwhales_initialize platform: %s", platform);
                LOGD("gameofwhales_initialize debug: %d", debug);

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                _proxyClass = (jclass) env->NewGlobalRef(env->FindClass("com/gameofwhales/cocos2dproxy/GameOfWhalesProxy"));
                JNI_NOT_NULL(_proxyClass);

                jmethodID instancemethod = env->GetStaticMethodID(_proxyClass, "instance", "()Lcom/gameofwhales/cocos2dproxy/GameOfWhalesProxy;");
                JNI_NOT_NULL(instancemethod);

                _proxyInstance = (jobject) env->NewGlobalRef(env->CallStaticObjectMethod(_proxyClass, instancemethod));
                JNI_NOT_NULL(_proxyInstance);


                jstring jStore = env->NewStringUTF("");//TOODOO
                jstring jGameKey = env->NewStringUTF(gameKey);
                jstring jVersion = env->NewStringUTF(version);
                jstring jPlatform = env->NewStringUTF(platform);//Ljava/lang/String;
                jstring jAndroidProject = env->NewStringUTF("");
                jboolean jDebug = debug;

                jmethodID initmethod = env->GetStaticMethodID(_proxyClass, "initialize", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
                JNI_NOT_NULL(initmethod);

                env->CallStaticVoidMethod(_proxyClass, initmethod, jGameKey, jStore, jAndroidProject, jVersion, jDebug);
            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _initialize: failed");
            }
        }
        
        
        void _inAppPurchased(const char* sku, float price, const char* currency, const char* transactionID, const char* receipt)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _inAppPurchased");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jSku = env->NewStringUTF(sku);
                jfloat  jPrice = price;
                jstring jCurrency = env->NewStringUTF(currency);
                jstring jTransactionID = env->NewStringUTF(transactionID);
                jstring jReceipt = env->NewStringUTF(receipt);

                jmethodID _call = env->GetMethodID(_proxyClass, "inAppPurchased", "(Ljava/lang/String;FLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jSku, jPrice, jCurrency, jTransactionID, jReceipt);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _inAppPurchased: failed");
            }

        }
        
        
        void _updateToken(const char* token, const char* provider)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _updateToken");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jToken = env->NewStringUTF(token);
                jstring jProvider = env->NewStringUTF(provider);

                jmethodID _call = env->GetMethodID(_proxyClass, "updateToken", "(Ljava/lang/String;Ljava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jToken, jProvider);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _updateToken: failed");
            }
        }
        
        
        void _pushReacted(const char* camp)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _pushReacted");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jCamp = env->NewStringUTF(camp);

                jmethodID _call = env->GetMethodID(_proxyClass, "pushReacted", "(Ljava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jCamp);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _pushReacted: failed");
            }

        }
        
        
        void _converting(const char* json_resources, const char *place)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _converting");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jJsonResources = env->NewStringUTF(json_resources);
                jstring jPlace = env->NewStringUTF(place);

                jmethodID _call = env->GetMethodID(_proxyClass, "converting", "(Ljava/lang/String;Ljava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jJsonResources, jPlace);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _converting: failed");
            }
        }
        
        
        void _profile(const char* json_changed_params)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _profile");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jProfile = env->NewStringUTF(json_changed_params);

                jmethodID _call = env->GetMethodID(_proxyClass, "profile", "(Ljava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jProfile);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _profile: failed");
            }
        }
        
        
        void _consume(const char* currency, int number, const char* sink, int amount, const char* place)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _consume");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jCurrency = env->NewStringUTF(currency);
                jint  jNumber = number;
                jstring jSink = env->NewStringUTF(sink);
                jint jAmount = amount;
                jstring jPlace = env->NewStringUTF(place);

                jmethodID _call = env->GetMethodID(_proxyClass, "consume", "(Ljava/lang/String;ILjava/lang/String;ILjava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jCurrency, jNumber, jSink, jAmount, jPlace);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _consume: failed");
            }
        }
        
        
        void _acquire(const char* currency, int amount, const char* source, int number, const char* place)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _acquire");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jCurrency = env->NewStringUTF(currency);
                jint  jNumber = number;
                jstring jSource = env->NewStringUTF(source);
                jint jAmount = amount;
                jstring jPlace = env->NewStringUTF(place);

                jmethodID _call = env->GetMethodID(_proxyClass, "acquire", "(Ljava/lang/String;ILjava/lang/String;ILjava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jCurrency, jAmount, jSource, jNumber, jPlace);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _acquire: failed");
            }
        }
        
        
        void _reportError(const char* message, const char* stacktrace)
        {
            if (!check())
                return;

            try
            {
                LOGD("gameofwhales native _reportError");

                JNIEnv* env = cocos2d::JniHelper::getEnv();
                JNI_NOT_NULL(env);

                jstring jMessage = env->NewStringUTF(message);
                jstring jStacktrace = env->NewStringUTF(stacktrace);

                jmethodID _call = env->GetMethodID(_proxyClass, "reportError", "(Ljava/lang/String;Ljava/lang/String;)V");
                JNI_NOT_NULL(_call);

                env->CallVoidMethod(_proxyInstance, _call, jMessage, jStacktrace);

            }
            catch (const notFound&)
            {
                LOGD("gameofwhales native _reportError: failed");
            }
        }
        
        
        
    }
    
    
    
    
    
    
}
