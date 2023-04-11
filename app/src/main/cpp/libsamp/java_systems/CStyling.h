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

    static void show(int money, int total, int price1, int price2, int price3, int price4);
};

#endif //LIVERUSSIA_CSTYLING_H
