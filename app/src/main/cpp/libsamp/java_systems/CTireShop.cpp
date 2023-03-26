//
// Created by Лихватов Иван on 20.03.2023.
//
#include "CTireShop.h"

#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "util/CJavaWrapper.h"

jclass CTireShop::clazz = nullptr;
jobject CTireShop::thiz = nullptr;

void CTireShop::startRendering(bool toggle, uint32_t price) {
    JNIEnv* env = g_pJavaWrapper->GetEnv();
    if(!env)return;
    if(!pNetGame)return;

    CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
    if(!pPlayerPool)return;

    if(CTireShop::thiz == nullptr) {
        jmethodID constructor = env->GetMethodID(CTireShop::clazz, "<init>",
                                                 "(Landroid/app/Activity;)V");
        CTireShop::thiz = env->NewObject(CTireShop::clazz, constructor, g_pJavaWrapper->activity);
        CTireShop::thiz = env->NewGlobalRef(CTireShop::thiz);
    }

    CTireShop::show(toggle, price);

}

void CTireShop::show(bool toggle, uint32_t price) {
    if(CTireShop::thiz == nullptr)return;

    JNIEnv *env = g_pJavaWrapper->GetEnv();

    jmethodID method = env->GetMethodID(clazz, "showRendering", "(ZI)V");
    env->CallVoidMethod(CTireShop::thiz, method, toggle, price);
}

void CNetGame::packetTireShop(Packet* p)
{
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);

    bs.IgnoreBits(40); // skip packet and rpc id

    bool toggle;
    uint32_t price;

    bs.Read(toggle);
    bs.Read(price);

    CTireShop::startRendering(toggle, price);
}

//void CTireShop::chooseDisc()
//{
//    JNIEnv* env = g_pJavaWrapper->GetEnv();
//    if(!env)return;
//
//    jclass clazz = env->GetObjectClass(CTireShop::thiz);
//
//    int iComponent; //это я так понял id диска
//
//    // ------------ сидит в машине
//
//    CPlayerPed *pPlayerPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->m_pPlayerPed;
//    CVehicle* pVehicle = pPlayerPed->GetCurrentVehicle();
//
//    pVehicle->AddVehicleUpgrade(iComponent);
//
//    //------
//
//
//    jmethodID method = env->GetMethodID(clazz, "clearKillList", "()V");
//
//    env->CallVoidMethod(CTireShop::thiz, method);
//}



