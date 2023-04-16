//
// Created by plaka on 24.01.2023.
//

#include <jni.h>
#include "CLoader.h"
#include "game/bass.h"
#include "util/patch.h"
#include "crashlytics.h"
#include "CSettings.h"
#include "java_systems/CSpeedometr.h"
#include "java_systems/CDonate.h"
#include "java_systems/CEditobject.h"
#include "java_systems/CStyling.h"
#include "java_systems/CTireShop.h"
#include "java_systems/casino/Dice.h"

void InitBASSFuncs();
void CLoader::loadBassLib()
{
    InitBASSFuncs();
    BASS_Init(-1, 44100, BASS_DEVICE_3D, nullptr, nullptr);
    BASS_Set3DFactors(1, 0.10, 1);
    BASS_Apply3D();
}

void CLoader::initCrashLytics()
{
    firebase::crashlytics::SetCustomKey("build data", __DATE__);
    firebase::crashlytics::SetCustomKey("build time", __TIME__);

    firebase::crashlytics::SetUserId(CSettings::m_Settings.szNickName);
    firebase::crashlytics::SetCustomKey("Nick", CSettings::m_Settings.szNickName);

    uintptr_t libsamp = FindLibrary("libsamp.so");
    uintptr_t libc = FindLibrary("libc.so");

    char str[100];

    sprintf(str, "0x%x", g_libGTASA);
    firebase::crashlytics::SetCustomKey("libGTASA.so", str);

    sprintf(str, "0x%x", libsamp);
    firebase::crashlytics::SetCustomKey("libsamp.so", str);

    sprintf(str, "0x%x", libc);
    firebase::crashlytics::SetCustomKey("libc.so", str);
}

void CLoader::loadSetting()
{
    pthread_t thread;
    pthread_create(&thread, nullptr, CLoader::loadSettingThread, nullptr);
}

void *CLoader::loadSettingThread(void *p)
{
    Log("loadSettingThread");

    CSettings::LoadSettings(nullptr);

    pthread_exit(nullptr);
}

void CLoader::initJavaClasses(JavaVM* pjvm)
{
    JNIEnv* env = nullptr;
    pjvm->GetEnv((void**)& env, JNI_VERSION_1_6);

    CSpeedometr::clazz = env->FindClass("com/liverussia/cr/gui/Speedometer");
    CSpeedometr::clazz = (jclass) env->NewGlobalRef( CSpeedometr::clazz );

    CDonate::clazz = env->FindClass("com/liverussia/cr/gui/donate/Donate");
    CDonate::clazz = (jclass) env->NewGlobalRef( CDonate::clazz );

    CEditobject::clazz = env->FindClass("com/liverussia/cr/gui/AttachEdit");
    CEditobject::clazz = (jclass) env->NewGlobalRef( CEditobject::clazz );

    CStyling::clazz = env->FindClass("com/liverussia/cr/gui/styling/Styling");
    CStyling::clazz = (jclass) env->NewGlobalRef( CStyling::clazz );

    CTireShop::clazz = env->FindClass("com/liverussia/cr/gui/tire_shop/TireShop");
    CTireShop::clazz = (jclass) env->NewGlobalRef( CTireShop::clazz );

    CDice::clazz = env->FindClass("com/liverussia/cr/gui/casino/Dice");
    CDice::clazz = (jclass) env->NewGlobalRef( CDice::clazz );
}

