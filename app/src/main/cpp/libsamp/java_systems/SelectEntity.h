//
// Created by plaka on 01.05.2023.
//

#pragma once

#include <jni.h>
#include "game/common.h"

class SelectEntity {
public:
    static inline jobject     thiz;
    static inline jclass      clazz;

public:
    static void findSelectEntity();
    static void onSelectedPed(PLAYERID playerId);
};

