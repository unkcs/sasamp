//
// Created by Лихватов Иван on 20.03.2023.
//
#include "CTireShop.h"

#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "util/CJavaWrapper.h"
#include "CSpeedometr.h"

#include "CHUD.h"

jclass CTireShop::clazz = nullptr;
jobject CTireShop::thiz = nullptr;
bool CTireShop::bIsShow = false;

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
    int currentBalance = CHUD::iLocalMoney;

    jmethodID method = env->GetMethodID(clazz, "showRendering", "(ZII)V");
    env->CallVoidMethod(CTireShop::thiz, method, toggle, price, currentBalance);

    if (toggle) {
        CTireShop::bIsShow = true;
    } else {
        CTireShop::bIsShow = false;
    }
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




//extern "C"
//JNIEXPORT void JNICALL
//Java_com_liverussia_cr_gui_tire_1shop_TireShop_sendClickItem(JNIEnv *env, jobject thiz,
//                                                             jint button_id, float value) {
//    uint8_t packet = ID_CUSTOM_RPC;
//    uint8_t RPC = RPC_TUNING_WHEELS;
//
//    RakNet::BitStream bsSend;
//    bsSend.Write(packet);
//    bsSend.Write(RPC);
//    bsSend.Write((uint8_t) button_id);
//    bsSend.Write((float_t) value);
////    bsSend.Write((uint16_t) item_type);
////    bsSend.Write((uint16_t) item_id);
//
//    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
//}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_tire_1shop_TireShop_sendClickItem(JNIEnv *env, jobject thiz,
                                                             jint button_id, jfloat value) {
    uint8_t packet = ID_CUSTOM_RPC;
    uint8_t RPC = RPC_TUNING_WHEELS;

    RakNet::BitStream bsSend;
    bsSend.Write(packet);
    bsSend.Write(RPC);
    bsSend.Write((int8_t) button_id);
    bsSend.Write((float) value);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_tire_1shop_TireShop_chooseDisk(JNIEnv *env, jobject thiz, jint disk_id) {
//    if(!env)return;

//    jclass clazz = env->GetObjectClass(CTireShop::thiz);

    // ------------ сидит в машине

//    CPlayerPed *pPlayerPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->m_pPlayerPed;
//    CVehicle* pVehicle = pPlayerPed->GetCurrentVehicle();
//
//    pVehicle->AddVehicleUpgrade(disk_id);


//    jmethodID method = env->GetMethodID(clazz, "clearKillList", "()V");
//
//    env->CallVoidMethod(CTireShop::thiz, method);
}