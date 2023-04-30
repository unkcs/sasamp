//
// Created by �������� ���� on 26.04.2023.
//
#include "CTheftAuto.h"

#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "util/CJavaWrapper.h"

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