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

RGB CStyling::neonDefault = {0,0,0};
RGB CStyling::lightsDefault = {0,0,0};
RGB CStyling::tonerDefault = {0,0,0};

void CStyling::show(int money) {
    JNIEnv *env = g_pJavaWrapper->GetEnv();

    if(CStyling::thiz == nullptr) {

        jmethodID constructor = env->GetMethodID(CStyling::clazz, "<init>",
                                                 "(Landroid/app/Activity;I)V");
        CStyling::thiz = env->NewObject(CStyling::clazz, constructor, g_pJavaWrapper->activity, money);
        CStyling::thiz = env->NewGlobalRef(CStyling::thiz);
    }

    CPlayerPed* playerPed = pGame->FindPlayerPed();
    if(!playerPed) return;

    if (pNetGame->GetVehiclePool())
    {
        VEHICLEID vehId = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(playerPed->GetGtaVehicle());
        CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(vehId);
        if (pVehicle)
        {
            RGB defaultLights = {pVehicle->m_bHeadlightsR, pVehicle->m_bHeadlightsG, pVehicle->m_bHeadlightsB};
            memcpy(&lightsDefault, (void*)&defaultLights, sizeof(RGB));

            RGB defaultNeon = {pVehicle->m_Shadow.r, pVehicle->m_Shadow.g, pVehicle->m_Shadow.b};
            memcpy(&defaultNeon, (void*)&neonDefault, sizeof(RGB));

            RGB defaultToner = {pVehicle->colorToner[0], pVehicle->colorToner[1], pVehicle->colorToner[2]};
            memcpy(&defaultToner, (void*)&tonerDefault, sizeof(RGB));
        }
    }

    Log("lights: %d, %d, %d. neon: %d, %d, %d. toner: %d, %d, %d",
        lightsDefault.r, lightsDefault.g, lightsDefault.b, neonDefault.r, neonDefault.g, neonDefault.b,
        tonerDefault.r, tonerDefault.g, tonerDefault.b);
}

void CNetGame::packetStylingCenter(Packet* p)
{
    RakNet::BitStream bs((unsigned char*)p->data, p->length, false);

    bs.IgnoreBits(40); // skip packet and rpc id

    int balance;
    uint32_t price;

    bs.Read(balance);

    CStyling::show(balance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_exitClick(JNIEnv *env, jobject thiz) {
    // TODO: implement exitClick()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_styling_Styling_toStock(JNIEnv *env, jobject thiz) {
    if (pNetGame->GetVehiclePool())
    {
        VEHICLEID vehId = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(pGame->FindPlayerPed()->GetGtaVehicle());
        CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(vehId);
        if (pVehicle)
        {
            pVehicle->ApplyToner(1, CStyling::tonerDefault.r);
            pVehicle->ApplyToner(2, CStyling::tonerDefault.g);
            pVehicle->ApplyToner(3, CStyling::tonerDefault.b);

            pVehicle->SetHeadlightsColor(CStyling::lightsDefault.r, CStyling::lightsDefault.g, CStyling::lightsDefault.b);
        }
    }
}