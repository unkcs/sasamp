//
// Created by plaka on 14.04.2023.
//

#pragma once

#include "tHandlingData.h"
#include "tBikeHandlingData.h"
#include "tFlyingHandlingData.h"
#include "tBoatHandlingData.h"
#include <vector>

constexpr auto ACCEL_CONST = 1.f / (50.f * 50.f); // This number 50 seems to be coming up a lot...;

// 0xC2B9BC - Used for velocity conversion from file to game units
constexpr auto VELOCITY_CONST = 0.277778f / 50.f;

class cHandlingDataMgr {
public:
    float m_fCoefficientOfRestitution;
    float m_fWheelFriction;
    float m_aResistanceTable[3];

    static std::vector<tHandlingData> m_aHandlingData;
    static std::vector<tBikeHandlingData> m_aBikeHandlingData;
    static std::vector<tFlyingHandlingData> m_aFlyingHandlingData;
    static std::vector<tBoatHandlingData> m_aBoatHandlingData;
   // tHandlingData m_aHandlingData[210];
//    tBikeHandlingData m_aBikeHandlingData[13];
//    tFlyingHandlingData m_aFlyingHandlingData[24];
//    tBoatHandlingData m_aBoatHandlingData[12];

public:
    static void InjectHooks();

    cHandlingDataMgr();

    /// Process handling.cfg
    static void LoadHandlingData();

    tFlyingHandlingData* GetFlyingPointer(uint8_t handlingId);
    tBoatHandlingData*   GetBoatPointer(uint8_t handlingId);
    tHandlingData*       GetVehiclePointer(uint32_t handlingId);
    tBikeHandlingData*   GetBikeHandlingPointer(uint32_t handlingId);

    static int FindExactWord(const char *name, const char *nameTable, unsigned int entrySize,
                      unsigned int entryCount);

    static void ConvertDataToGameUnits(tHandlingData *h);
};
//static_assert(sizeof(cHandlingDataMgr) == 0xC624, "Invalid size cHandlingDataMgr");