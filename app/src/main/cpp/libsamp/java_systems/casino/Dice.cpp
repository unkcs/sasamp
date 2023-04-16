//
// Created by plaka on 16.04.2023.
//

#include "Dice.h"
#include "net/netgame.h"
#include "java_systems/CHUD.h"

jobject CDice::thiz = nullptr;
jclass CDice::clazz = nullptr;
bool CDice::bIsShow = false;


void CDice::hide()
{
    bIsShow = false;

    JNIEnv *env = g_pJavaWrapper->GetEnv();

    if(thiz != nullptr) {
        jmethodID method = env->GetMethodID(clazz, "destroy", "()V");
        env->CallVoidMethod(thiz, method);

        thiz = nullptr;
    }
}

void CDice::tempToggle(bool toggle)
{
    JNIEnv *env = g_pJavaWrapper->GetEnv();

    if(thiz != nullptr) {
        jmethodID method = env->GetMethodID(clazz, "tempToggle", "(Z)V");
        env->CallVoidMethod(thiz, method, toggle);
    }
}

void CDice::update(DicePacket *data) {

    bIsShow = true;

    JNIEnv *env = g_pJavaWrapper->GetEnv();

    if(thiz == nullptr) {
        jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");
        thiz = env->NewObject(clazz, constructor);
        thiz = env->NewGlobalRef(thiz);
    }

    jstring jPlayerName[MAX_PLAYERS_CASINO_DICE];

    for(int i = 0; i < MAX_PLAYERS_CASINO_DICE; i++) {
        if(data->playerID[i] == INVALID_PLAYER_ID) {
            jPlayerName[i] = env->NewStringUTF("--");
        } else {
            jPlayerName[i] = env->NewStringUTF(
                    pNetGame->GetPlayerPool()->GetPlayerName(data->playerID[i]));
        }
    }

    jstring jCrupName;
    jCrupName = env->NewStringUTF(
            pNetGame->GetPlayerPool()->GetPlayerName(data->crupId));

    jmethodID method = env->GetMethodID(clazz, "update",
                                             "(IIIILjava/lang/String;ILjava/lang/String;ILjava/lang/String;ILjava/lang/String;ILjava/lang/String;IILjava/lang/String;I)V");


    env->CallVoidMethod(thiz, method, data->tableId, data->bet, data->bank, CHUD::iLocalMoney,
                        jPlayerName[0], data->playerStat[0],
                        jPlayerName[1], data->playerStat[1],
                        jPlayerName[2], data->playerStat[2],
                        jPlayerName[3], data->playerStat[3],
                        jPlayerName[4], data->playerStat[4],
                        data->time, jCrupName, data->crupId);

    for(auto & i : jPlayerName)
        env->DeleteLocalRef(i);

    env->DeleteLocalRef(jCrupName);
}

void CNetGame::packetDice(Packet *p) {
    RakNet::BitStream bs((unsigned char *) p->data, p->length, false);

    bs.IgnoreBits(40); // skip packet and rpc id

    CDice::DicePacket data{};
    bs.Read((char*)&data, sizeof(CDice::DicePacket));

    if(data.bIsShow) {
        CDice::update(&data);
    } else {
        CDice::hide();
    }
}

