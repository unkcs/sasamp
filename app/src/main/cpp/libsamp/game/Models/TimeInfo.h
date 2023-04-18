//
// Created by plaka on 07.03.2023.
//

#ifndef LIVERUSSIA_TIMEINFO_H
#define LIVERUSSIA_TIMEINFO_H

#pragma pack(push, 1)
struct CTimeInfo {
    uint8_t m_nTimeOn;
    uint8_t m_nTimeOff;
    int16_t m_nOtherTimeModel = -1;
};
static_assert(sizeof(CTimeInfo) == 0x4, "Invalid size CTimeInfo");
#pragma pack(pop)

#endif //LIVERUSSIA_TIMEINFO_H
