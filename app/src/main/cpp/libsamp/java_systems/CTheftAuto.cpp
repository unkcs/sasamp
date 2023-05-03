//
// Created by Лихватов Иван on 26.04.2023.
//
#include "CTheftAuto.h"

#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "util/CJavaWrapper.h"
#include "CNotification.h"

jclass CTheftAuto::clazz = nullptr;
jobject CTheftAuto::thiz = nullptr;
bool CTheftAuto::bIsShow = false;

void CTheftAuto::startRendering() {
    JNIEnv* env = g_pJavaWrapper->GetEnv();
    if(!env)return;
    if(!pNetGame)return;

    CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
    if(!pPlayerPool)return;

    if(CTheftAuto::thiz == nullptr) {
        jmethodID constructor = env->GetMethodID(CTheftAuto::clazz, "<init>",
                                                 "(Landroid/app/Activity;)V");
        CTheftAuto::thiz = env->NewObject(CTheftAuto::clazz, constructor, g_pJavaWrapper->activity);
        CTheftAuto::thiz = env->NewGlobalRef(CTheftAuto::thiz);
    }

    CTheftAuto::show();

}

void CTheftAuto::show() {
    if(CTheftAuto::thiz == nullptr)return;

    JNIEnv *env = g_pJavaWrapper->GetEnv();

    jmethodID method = env->GetMethodID(clazz, "showRendering", "()V");
    env->CallVoidMethod(CTheftAuto::thiz, method);

    CTheftAuto::bIsShow = true;
}

void CNetGame::packetTheftAuto(Packet* p)
{
    CTheftAuto::startRendering();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_theft_1auto_TheftAuto_finishRendering(JNIEnv *env, jobject thiz,
                                                                 jint status) {
    if (status == 1) {
        uint8_t packet = ID_CUSTOM_RPC;
        uint8_t RPC = RPC_THEFT_AUTO;

        RakNet::BitStream bsSend;
        bsSend.Write(packet);
        bsSend.Write(RPC);

        pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);

//        CNotification::show(type, (char *) utf_str, time, actionId);
    } else {
        char str[256] = "Отмычка сломалась! Попробуйте еще";
        CNotification::show(0, (char *) str, 5, -1);
    }

    CTheftAuto::bIsShow = false;
    env->DeleteGlobalRef(CTheftAuto::thiz);
    CTheftAuto::thiz = nullptr;
}