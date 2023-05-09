//
// Created by admin on 05.04.2023.
//

#include "CStyling.h"
#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "util/CJavaWrapper.h"
#include "CNotification.h"

extern CGame* pGame;

#define STYLING_PRICES_COUNT 6

jclass  CStyling::clazz = nullptr;
jobject CStyling::thiz = nullptr;
bool CStyling::bIsShow = false;

void CStyling::show(int money, int total, uint32_t* prices) {
    JNIEnv *env = g_pJavaWrapper->GetEnv();

    if(CStyling::thiz == nullptr) {
        jmethodID constructor = env->GetMethodID(CStyling::clazz, "<init>", "(II[I)V");

        jint arr[STYLING_PRICES_COUNT];
        for (int i = 0; i < STYLING_PRICES_COUNT ; i++) {
            arr[i] = prices[i];
        }

        jintArray array = env->NewIntArray(STYLING_PRICES_COUNT);
        env->SetIntArrayRegion(array, 0, STYLING_PRICES_COUNT, arr);

        CStyling::thiz = env->NewObject(CStyling::clazz, constructor, money, total, array);
        CStyling::thiz = env->NewGlobalRef(CStyling::thiz);
    }
    bIsShow = true;
}

void CStyling::update(int money, int total) {
    JNIEnv *env = g_pJavaWrapper->GetEnv();
    if(!env)return;

    jmethodID method = env->GetMethodID(clazz, "update", "(II)V");

    env->CallVoidMethod(CStyling::thiz, method, money, total);
}

void CNetGame::packetStylingCenter(Packet* p)
{
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);

    bs.IgnoreBits(40); // skip packet and rpc id

    bool toggle;
    uint32_t balance, total;
    uint32_t prices[STYLING_PRICES_COUNT];

    bs.Read(toggle);
    bs.Read(balance);
    bs.Read(total);

    for(int i = 0; i < STYLING_PRICES_COUNT; i++) {
        bs.Read(prices[i]);
    }


    if(toggle && !CStyling::bIsShow) {
        CStyling::show(balance, total, prices);
    }
    else if(toggle && CStyling::bIsShow)
    {
        CStyling::update(balance, total);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_exitClick(JNIEnv *env, jobject thiz) {
    CStyling::bIsShow = false;
    CStyling::thiz = nullptr;

    RakNet::BitStream bsSend;
    bsSend.Write((uint8_t)ID_CUSTOM_RPC);
    bsSend.Write((uint8_t)RPC_STYLING_CENTER);
    bsSend.Write((uint8_t)0);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_sendChoosedColor(JNIEnv *env, jobject thiz, jint type, jint r, jint g, jint b, jint a) {
    RakNet::BitStream bsSend;
    bsSend.Write((uint8_t)ID_CUSTOM_RPC);
    bsSend.Write((uint8_t)RPC_STYLING_CENTER);
    bsSend.Write((uint8_t)  2);
    bsSend.Write((uint8_t)  type);
    bsSend.Write((uint8_t)  r);
    bsSend.Write((uint8_t)  g);
    bsSend.Write((uint8_t)  b);
    bsSend.Write((uint8_t)  a);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_sendBuy(JNIEnv *env, jobject thiz) {
    RakNet::BitStream bsSend;
    bsSend.Write((uint8_t)ID_CUSTOM_RPC);
    bsSend.Write((uint8_t)RPC_STYLING_CENTER);
    bsSend.Write((uint8_t)3);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_onChangeColor(JNIEnv *env, jobject thiz, jint type, jint r, jint g, jint b, jint a) {
    auto pPed = pGame->FindPlayerPed();

    if(!pPed->IsInVehicle()) return;

    auto pVehicle = pPed->GetCurrentVehicle();

    switch(type) {
        case 1: {
            // light
            pVehicle->lightColor.Set(r, g, b, 50);
            break;
        }
        case 2: {
            // toner
            pVehicle->tonerColor.Set(r, g, b, a);
            break;
        }
        case 3: {
            // body
            pVehicle->mainColor.Set(r, g, b, 255);
            break;
        }
        case 4: {
            // body2
            pVehicle->secondColor.Set(r, g, b, 255);
            break;
        }
        case 5: {
            // wheel
            pVehicle->wheelColor.Set(r, g, b, 255);
            break;
        }
        case 0: {
            // neon
            pVehicle->neonColor.Set(r, g, b, 255); // y - ширина
            pVehicle->m_bShadow = true;
            break;
        }
    }

}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_changeVinyls(JNIEnv *env, jobject thiz,
                                                        jboolean is_next) {
    auto pPed = pGame->FindPlayerPed();

    if(!pPed->IsInVehicle()) return;

    auto pVehicle = pPed->GetCurrentVehicle();

    if(is_next) {
        pVehicle->m_iVinylId ++;

        if(pVehicle->m_iVinylId > std::size(CVehicle::m_pVinyls) - 1 )
            pVehicle->m_iVinylId = -1;
    } else {
        pVehicle->m_iVinylId --;

        if(pVehicle->m_iVinylId < -1)
            pVehicle->m_iVinylId = std::size(CVehicle::m_pVinyls) - 1;
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_liverussia_cr_gui_styling_Styling_getActiveColor(JNIEnv *env, jobject thiz, jint type) {
    auto pPed = pGame->FindPlayerPed();

    if(!pPed->IsInVehicle()) return 0;

    auto pVehicle = pPed->GetCurrentVehicle();

    switch(type) {
        case 1: {
            // light
            return pVehicle->lightColor.ToIntARGB();
        }
        case 2: {
            // toner
            return pVehicle->tonerColor.ToIntARGB();
        }
        case 3: {
            // body
            return pVehicle->mainColor.ToIntARGB();
        }
        case 4: {
            // body2
            return pVehicle->secondColor.ToIntARGB();
        }
        case 5: {
            // wheel
            return pVehicle->wheelColor.ToIntARGB();
        }
        case 0: {
            // neon
            return 0;
           // pVehicle->SetCustomShadow(r, g, b, 5.0, 5.0, nullptr);
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_sendOnChooseVinil(JNIEnv *env, jobject thiz) {
    auto pPed = pGame->FindPlayerPed();

    if(!pPed->IsInVehicle()) return;

    auto pVehicle = pPed->GetCurrentVehicle();

    RakNet::BitStream bsSend;
    bsSend.Write((uint8_t)ID_CUSTOM_RPC);
    bsSend.Write((uint8_t)RPC_STYLING_CENTER);
    bsSend.Write((uint8_t)44);
    bsSend.Write((int16_t)pVehicle->m_iVinylId);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_liverussia_cr_gui_styling_Styling_isAvailableStyle(JNIEnv *env, jobject thiz, jint type) {
    auto pPed = pGame->FindPlayerPed();

    if(!pPed->IsInVehicle()) return 0;

    auto pVehicle = pPed->GetCurrentVehicle();

    switch(type) {
        case 1: {
            // light
           return true;
        }
        case 2: {
            // toner
            return true;
        }
        case 3: {
            // body
//            if(pVehicle->m_iVinylId != -1) {
//                CNotification::show(0, "Недоступно с винилами!", 5, 0);
//                return false;
//            }
            return true;
        }
        case 4: {
            // body2
            return true;
        }
        case 5: {
            // wheel

            return true;
        }
        case 0: {
            // neon
            return true;
            // pVehicle->SetCustomShadow(r, g, b, 5.0, 5.0, nullptr);
        }
    }
}