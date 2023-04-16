//
// Created by plaka on 16.04.2023.
//

#pragma once

#include <jni.h>

class CDice {
public:
    static constexpr int MAX_PLAYERS_CASINO_DICE = 5;

    struct __attribute__((aligned(1))) DicePacket
    {
        uint8_t bIsShow;
        uint8_t tableId;
        uint32_t bet;
        uint32_t bank;

        uint16_t playerID[MAX_PLAYERS_CASINO_DICE];
        uint8_t playerStat[MAX_PLAYERS_CASINO_DICE];
        uint8_t time;
        uint16_t crupId;
    };

    static jobject thiz;
    static jclass clazz;

    static bool bIsShow;

    static void hide();

    static void update(DicePacket *data);

    static void tempToggle(bool toggle);
};
