#include "main.h"
#include "CTab.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "vendor/imgui/imgui_internal.h"
#include "util/CJavaWrapper.h"
#include "CSettings.h"
#include "CHUD.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

extern CNetGame* pNetGame;
extern CGame* pGame;
extern CGUI* pGUI;

void CTab::show() {
    JNIEnv* env = g_pJavaWrapper->GetEnv();

    if(!CTab::thiz) {
        jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
        CTab::thiz = env->NewObject(clazz, constructor);
        CTab::thiz = env->NewGlobalRef(CTab::thiz);

        pNetGame->UpdatePlayerScoresAndPings();
    }
    CTab::bIsShow = true;
}

void CTab::hide() {
    JNIEnv* env = g_pJavaWrapper->GetEnv();

    jmethodID method = env->GetMethodID(clazz, "close", "()V");
    env->CallVoidMethod(CTab::thiz, method);

}

void CTab::update() {

    CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();

    CTab::setStat(pPlayerPool->GetLocalPlayerID(),
                               pPlayerPool->GetLocalPlayerName(),
                               pPlayerPool->GetLocalPlayerScore(),
                               pPlayerPool->GetLocalPlayerPing());

    PLAYERID i = 1, x;
    for (x = 0; x < MAX_PLAYERS; x++)
    {
        if (!pPlayerPool->GetSlotState(x)) continue;

        CTab::setStat(x,
                pPlayerPool->GetPlayerName(x),
                pPlayerPool->GetRemotePlayerScore(x),
                pPlayerPool->GetRemotePlayerPing(x) );

    }

    //Show Window
}

void CTab::setStat(int id, char name[], int score, int ping) {

    JNIEnv* env = g_pJavaWrapper->GetEnv();

    if (!env)
    {
        Log("No env");
        return;
    }

    jmethodID setStat = env->GetMethodID(clazz, "setStat", "(IILjava/lang/String;II)V");


    jstring jPlayerName = env->NewStringUTF( name );

    uint32_t color;
    if(id == pNetGame->GetPlayerPool()->GetLocalPlayerID())
        color = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerColor();
    else
        color = pNetGame->GetPlayerPool()->GetAt(id)->GetPlayerColor();

    if(color == 0) color = 0xffffffff;
    CRGBA gg;
    gg.Set(color);
    gg = gg.ToRGB();

    env->CallVoidMethod(CTab::thiz, setStat, id, (jint)gg.ToIntARGB(), jPlayerName, score, ping);

    env->DeleteLocalRef(jPlayerName);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_tab_Tab_onTabClose(JNIEnv *env, jobject thiz) {
    CTab::bIsShow = false;

    env->DeleteGlobalRef(CTab::thiz);
    CTab::thiz = nullptr;
}
