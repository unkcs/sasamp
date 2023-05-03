//
// Created by plaka on 14.04.2023.
//

#include "Timer.h"
#include "../util/CJavaWrapper.h"
#include "util/patch.h"
#include "java_systems/CSpeedometr.h"
#include "keyboard.h"

uint8_t* gTimerRunning = nullptr;

float CTimer::game_FPS = 0;

bool        CTimer::m_CodePause = false;
bool        CTimer::m_UserPause = false;
float       CTimer::ms_fTimeScale;
uint32_t    CTimer::m_FrameCounter = 0;
uint32_t    CTimer::m_snTimeInMilliseconds = 0;
bool        CTimer::bSkipProcessThisFrame = false;
float       CTimer::ms_fTimeStep = 0;
uint32_t    CTimer::m_snPPPPreviousTimeInMilliseconds;
uint32_t    CTimer::m_snPPPreviousTimeInMilliseconds;
uint32_t    CTimer::m_snPPreviousTimeInMilliseconds;
uint32_t    CTimer::m_snPreviousTimeInMilliseconds;
uint32_t    CTimer::m_snTimeInMillisecondsNonClipped;
uint32_t    CTimer::m_snPreviousTimeInMillisecondsNonClipped;

void CTimer::InjectHooks()
{
    CHook::Write(g_libGTASA + 0x005D141C, &CTimer::m_CodePause);
    CHook::Write(g_libGTASA + 0x005D07E4, &CTimer::m_FrameCounter);
    CHook::Write(g_libGTASA + 0x005CF440, &CTimer::game_FPS);
    CHook::Write(g_libGTASA + 0x005CF47C, &CTimer::m_UserPause);
    CHook::Write(g_libGTASA + 0x005CE5E8, &CTimer::ms_fTimeScale);
    CHook::Write(g_libGTASA + 0x005CED58, &CTimer::m_snTimeInMilliseconds);
    CHook::Write(g_libGTASA + 0x005D1180, &CTimer::bSkipProcessThisFrame);
    CHook::Write(g_libGTASA + 0x005CF4E8, &CTimer::ms_fTimeStep);

    CHook::Write(g_libGTASA + 0x0067949C, &CTimer::m_snPPPPreviousTimeInMilliseconds);
    CHook::Write(g_libGTASA + 0x00677DF0, &CTimer::m_snPPPreviousTimeInMilliseconds);
    CHook::Write(g_libGTASA + 0x00679D3C, &CTimer::m_snPPreviousTimeInMilliseconds);
    CHook::Write(g_libGTASA + 0x006779A8, &CTimer::m_snPreviousTimeInMilliseconds);
    CHook::Write(g_libGTASA + 0x005D1DA4, &CTimer::m_snTimeInMillisecondsNonClipped);
    CHook::Write(g_libGTASA + 0x006775F8, &CTimer::m_snPreviousTimeInMillisecondsNonClipped);

    gTimerRunning = &*(uint8_t*)(g_libGTASA + 0x008C9B74);

    CHook::Redirect(g_libGTASA, 0x003BF784, &CTimer::StartUserPause);
    CHook::Redirect(g_libGTASA, 0x003BF7A0, &CTimer::EndUserPause);
    CHook::Redirect(g_libGTASA, 0x003BF6F4, &CTimer::Stop);
    CHook::Redirect(g_libGTASA, 0x003BF758, &CTimer::GetIsSlowMotionActive);
}


uint64_t GetMillisecondTime() {

}

// 0x5617E0
void CTimer::Initialise()
{

}

// 0x5618C0
void CTimer::Shutdown() {

}

// 0x5619D0
void CTimer::Suspend()
{

}

// 0x561A00
void CTimer::Resume()
{

}

// 0x561AA0
void CTimer::Stop()
{
    *gTimerRunning = 0;
    CTimer::m_snPPPPreviousTimeInMilliseconds = CTimer::m_snTimeInMilliseconds;
    CTimer::m_snPPPreviousTimeInMilliseconds = CTimer::m_snTimeInMilliseconds;
    CTimer::m_snPPreviousTimeInMilliseconds = CTimer::m_snTimeInMilliseconds;
    CTimer::m_snPreviousTimeInMilliseconds = CTimer::m_snTimeInMilliseconds;

    CTimer::m_snPreviousTimeInMillisecondsNonClipped = CTimer::m_snTimeInMillisecondsNonClipped;
}

// 0x561AF0
void CTimer::StartUserPause()
{
    if (g_pJavaWrapper)
    {
        CKeyBoard::Close();
        g_pJavaWrapper->SetPauseState(true);
    }
    m_UserPause = true;
}

// 0x561B00
void CTimer::EndUserPause()
{
    // process resume event
    if (g_pJavaWrapper)
    {
        g_pJavaWrapper->SetPauseState(false);
    }
    m_UserPause = false;
}

// 0x561A40
uint32_t CTimer::GetCyclesPerMillisecond()
{
    return CHook::CallFunction<uint32_t>(g_libGTASA + 0x0042100C + 1);
}

// cycles per ms * 20
// 0x561A50
uint32_t CTimer::GetCyclesPerFrame()
{

}

uint64_t CTimer::GetCurrentTimeInCycles()
{
    return CHook::CallFunction<uint64_t>(g_libGTASA + 0x00421040 + 1);
}

// 0x561AD0
bool CTimer::GetIsSlowMotionActive()
{
    return CTimer::ms_fTimeScale < 1.0;
}

// 0x5618D0
void CTimer::UpdateVariables(float timeElapsed)
{

}

// 0x561B10
void CTimer::Update()
{

}