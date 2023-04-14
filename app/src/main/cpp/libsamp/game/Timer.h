//
// Created by plaka on 14.04.2023.
//

#pragma once

#include <cstdint>

extern uint8_t* gTimerRunning;

class CTimer {
    /*!
    * Thanks guys for figuring this out for me!
    *
    * So basically, a timestep is just fraction of the frametime (timestep = frametime / TIMESTEP_PER_SECOND)
    * this timestep is used basically everywhere to calculate physics, etc.
    */
    static constexpr float TIMESTEP_PER_SECOND = 50.f;                         /// Number of steps/second
    static constexpr float TIMESTEP_LEN_IN_MS  = 1000.f / TIMESTEP_PER_SECOND; /// How long (in ms) a timestep is

public:
    typedef uint64_t(__cdecl* TimerFunction_t)();

    static float game_FPS;
    static float ms_fTimeScale;
    static bool m_CodePause;
    static bool m_UserPause;
    static uint32_t m_FrameCounter;
    static uint32_t m_snTimeInMilliseconds;
    static uint32_t m_snPPPPreviousTimeInMilliseconds;
    static uint32_t m_snPPPreviousTimeInMilliseconds;
    static uint32_t m_snPPreviousTimeInMilliseconds;
    static uint32_t m_snPreviousTimeInMilliseconds;
    static uint32_t m_snTimeInMillisecondsNonClipped;
    static uint32_t m_snPreviousTimeInMillisecondsNonClipped;
    static bool bSkipProcessThisFrame;
    static float ms_fTimeStep;

public:
    static void InjectHooks();

    static void   Initialise();
    static void   Shutdown();
    static void   Suspend();
    static void   Resume();
    static void   Stop();
    static void   StartUserPause();
    static void   EndUserPause();
    static uint32_t GetCyclesPerMillisecond();
    static uint32_t GetCyclesPerFrame();
    static uint64_t GetCurrentTimeInCycles();
    static bool   GetIsSlowMotionActive();
    static void   UpdateVariables(float timeElapsed);
    static void   Update();

    static bool GetIsPaused() { return m_UserPause || m_CodePause; }
    static bool GetIsUserPaused() { return m_UserPause; }
    static bool GetIsCodePaused() { return m_CodePause; }
    static void SetCodePause(bool pause) { m_CodePause = pause; }

    static float  GetTimeStep() { return ms_fTimeStep; }

};
