//
// Created by Лихватов Иван on 26.04.2023.
//

#ifndef LIVERUSSIA_CTHEFTAUTO_H
#define LIVERUSSIA_CTHEFTAUTO_H

#include <jni.h>

static class CTheftAuto {
public:
    static jobject thiz;
    static jclass clazz;
    static bool bIsShow;

    static void startRendering();

    static void show();
};

#endif //LIVERUSSIA_CTHEFTAUTO_H
