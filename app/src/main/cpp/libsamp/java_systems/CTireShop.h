//
// Created by Лихватов Иван on 20.03.2023.
//

#ifndef LIVERUSSIA_CTIRESHOP_H
#define LIVERUSSIA_CTIRESHOP_H

#include <jni.h>

static class CTireShop {
public:
    static jobject thiz;
    static jclass clazz;

    static void startRendering(bool toggle, uint32_t price);

    static void show(bool toggle, uint32_t price);
};

#endif //LIVERUSSIA_CTIRESHOP_H
