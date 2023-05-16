#pragma once

#include <iostream>
#include <jni.h>

static class CTab {
public:
    inline static jobject   thiz    = nullptr;
    inline static jclass    clazz   = nullptr;
    inline static bool      bIsShow = false;

public:
    static void update();
    static void show();


    static void setStat(int id, char *name, int score, int ping);

    static void hide();
};


