//
// Created by admin on 05.04.2023.
//

#ifndef LIVERUSSIA_CSTYLING_H
#define LIVERUSSIA_CSTYLING_H

#include <jni.h>

struct RGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class CStyling
{
public:
    static jclass clazz;
    static jobject thiz;

    static RGB tonerDefault;
    static RGB lightsDefault;
    static RGB neonDefault;

    static void show(int money);
};

#endif //LIVERUSSIA_CSTYLING_H
