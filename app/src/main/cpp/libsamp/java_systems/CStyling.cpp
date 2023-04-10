//
// Created by admin on 05.04.2023.
//

#include "CStyling.h"
#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "util/CJavaWrapper.h"

extern CGame* pGame;

jclass  CStyling::clazz = nullptr;
jobject CStyling::thiz = nullptr;

void CStyling::show(int money, int total, int price1, int price2, int price3, int price4) {
    JNIEnv *env = g_pJavaWrapper->GetEnv();

    if(CStyling::thiz == nullptr) {

        jmethodID constructor = env->GetMethodID(CStyling::clazz, "<init>",
                                                 "(Landroid/app/Activity;IIIIII)V");
        CStyling::thiz = env->NewObject(CStyling::clazz, constructor, g_pJavaWrapper->activity, money,
                                        total, price1, price2, price3, price4);
        CStyling::thiz = env->NewGlobalRef(CStyling::thiz);
    }
}

void CNetGame::packetStylingCenter(Packet* p)
{
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);

    bs.IgnoreBits(40); // skip packet and rpc id

    int balance, toggle, total;
    int price_1, price_2, price_3, price_4;

    bs.Read(toggle);
    bs.Read(balance);
    bs.Read(total);
    bs.Read(price_1);
    bs.Read(price_2);
    bs.Read(price_3);
    bs.Read(price_4);

    if(toggle)
        CStyling::show(balance, total, price_1, price_2, price_3, price_4);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_exitClick(JNIEnv *env, jobject thiz) {
    // TODO: implement exitClick()
    RakNet::BitStream bsSend;
    bsSend.Write(ID_CUSTOM_RPC);
    bsSend.Write(RPC_STYLING_CENTER);
    bsSend.Write(0);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_toStock(JNIEnv *env, jobject thiz) {
    RakNet::BitStream bsSend;
    bsSend.Write(ID_CUSTOM_RPC);
    bsSend.Write(RPC_STYLING_CENTER);
    bsSend.Write(1);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_sendChoosedColor(JNIEnv *env, jobject thiz, jint type, jint r,
                                                            jint g, jint b) {
    // TODO: implement sendChoosedColor()
    RakNet::BitStream bsSend;
    bsSend.Write(ID_CUSTOM_RPC);
    bsSend.Write(RPC_STYLING_CENTER);
    bsSend.Write(2);
    bsSend.Write(type);
    bsSend.Write(r);
    bsSend.Write(g);
    bsSend.Write(b);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_sendBuy(JNIEnv *env, jobject thiz) {
    // TODO: implement sendBuy()
    RakNet::BitStream bsSend;
    bsSend.Write(ID_CUSTOM_RPC);
    bsSend.Write(RPC_STYLING_CENTER);
    bsSend.Write(3);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_sendClickedCameraArrow(JNIEnv *env, jobject thiz,
                                                                  jint rightorleft) {
    // TODO: implement sendClickedCameraArrow()
    RakNet::BitStream bsSend;
    bsSend.Write(ID_CUSTOM_RPC);
    bsSend.Write(RPC_STYLING_CENTER);
    if(rightorleft)
        bsSend.Write(5);
    else
        bsSend.Write(4);

    pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
}