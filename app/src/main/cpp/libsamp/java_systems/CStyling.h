//
// Created by admin on 05.04.2023.
//

#ifndef LIVERUSSIA_CSTYLING_H
#define LIVERUSSIA_CSTYLING_H

#include <jni.h>

class CStyling
{
public:
    static jclass clazz;
    static jobject thiz;

    static bool bIsShow;

    static void show(int money, int total, uint32_t arr[]);

    static void update(int money, int total);
};

#endif //LIVERUSSIA_CSTYLING_H
