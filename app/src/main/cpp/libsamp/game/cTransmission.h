//
// Created by plaka on 09.04.2023.
//

#ifndef LIVERUSSIA_CTRANSMISSION_H
#define LIVERUSSIA_CTRANSMISSION_H


#include <cstdint>
#include "tTransmissionGear.h"

#pragma pack(push, 4)
class cTransmission {
public:
    tGearRatio          m_aGears[6];
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
};
#pragma pack(pop)
static_assert(sizeof(cTransmission) == 0x68, "Invalid size cTransmission");

#endif //LIVERUSSIA_CTRANSMISSION_H
