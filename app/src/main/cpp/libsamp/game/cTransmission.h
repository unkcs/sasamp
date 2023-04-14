//
// Created by plaka on 09.04.2023.
//

#ifndef LIVERUSSIA_CTRANSMISSION_H
#define LIVERUSSIA_CTRANSMISSION_H


#include <cstdint>
#include "tTransmissionGear.h"

const float TRANSMISSION_FREE_ACCELERATION = 0.1f;
const float TRANSMISSION_SMOOTHER_FRAC = 0.85f;
const float TRANSMISSION_AI_CHEAT_MULT = 1.2f;
const float TRANSMISSION_NITROS_MULT = 2.0f;
const float TRANSMISSION_AI_CHEAT_INERTIA_MULT = 0.75f;
const float TRANSMISSION_NITROS_INERTIA_MULT = 0.5f;

enum {
    CHEAT_HANDLING_NONE = 0,
    CHEAT_HANDLING_PERFECT = 1,
    CHEAT_HANDLING_NITROS = 2
};

#pragma pack(push, 4)
class cTransmission {
public:
    tTransmissionGear   m_aGears[6];
    uint8_t             m_nDriveType;     // F/R/4
    uint8_t             m_nEngineType;    // P/D/E
    uint8_t             m_nNumberOfGears; // 1 to 6
   // uint8_t             undefined;
    uint32_t            m_handlingFlags;
    float               m_fEngineAcceleration; // 0.1 to 10.0
    float               m_fEngineInertia;      // 0.0 to 50.0
    float               m_fMaxGearVelocity;    // 5.0 to 150.0
    float               m_fMaxVelocity;
    float               m_maxReverseGearVelocity;
    float               m_fCurrentSpeed;

public:
    static void InjectHooks();

    cTransmission() = default; // 0x6D0450

    static void InitGearRatios(cTransmission* thiz);
    //void DisplayGearRatios();
    static void CalculateGearForSimpleCar(cTransmission*, float speed, uint8_t& currentGear); // todo: no static
    static float CalculateDriveAcceleration(cTransmission* thiz, const float& gasPedal, uint8_t& currentGear, float& gearChangeCount, float& velocity, float* a6, float* a7, uint8_t allWheelsOnGround, uint8_t handlingType);
};
#pragma pack(pop)
static_assert(sizeof(cTransmission) == 0x68, "Invalid size cTransmission");

#endif //LIVERUSSIA_CTRANSMISSION_H
