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
#include "java_systems/CAuthorization.h"
#include "java_systems/CChooseSpawn.h"
#include "java_systems/LoadingScreen.h"

int CLoader::tick = 0;

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

    uintptr_t libsamp = CUtil::FindLibrary("libsamp.so");
    uintptr_t libc = CUtil::FindLibrary("libc.so");

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

    CAuthorization::clazz = env->FindClass("com/liverussia/cr/gui/AuthorizationManager");
    CAuthorization::clazz = (jclass) env->NewGlobalRef(CAuthorization::clazz);

    CChooseSpawn::clazz = env->FindClass("com/liverussia/cr/gui/ChooseSpawn");
    CChooseSpawn::clazz = (jclass) env->NewGlobalRef(CChooseSpawn::clazz);

    LoadingScreen::clazz = env->FindClass("com/liverussia/cr/gui/InGameLoadingScreen");
    LoadingScreen::clazz = (jclass) env->NewGlobalRef(LoadingScreen::clazz);
}

