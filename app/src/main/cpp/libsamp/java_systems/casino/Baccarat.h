//
// Created by plaka on 19.01.2023.
//

#ifndef LIVERUSSIA_BACCARAT_H
#define LIVERUSSIA_BACCARAT_H


#include <jni.h>

static class CBaccarat {

public:
    static constexpr int BACCARAT_MAX_HISTORY = 9;
    static inline jobject thiz = nullptr;
    static inline jclass clazz = nullptr;
    static inline bool bIsShow = false;

    static void
    updateBaccarat(int redCard, int yellowCard, int totalPlayers, int totalRed, int totalYellow,
                   int totalGreen, int time, int betType, int betSum, int winner, int balance);

    static void tempToggle(bool toggle);

    static void updateLastWins(uint8_t* lastwins);

    static void hide();

    static void Contructor();
};


#endif //LIVERUSSIA_BACCARAT_H
