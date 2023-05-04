/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Ped.h"

#pragma pack(push, 1)
class CPlayerPedGta : public CPedGta {
public:
    CPed* m_p3rdPersonMouseTarget;
    int32 field_7A0;

    // did we display "JCK_HLP" message
    static bool& bHasDisplayedPlayerQuitEnterCarHelpText;

    // Android
    static bool bDebugPlayerInvincible;
    static bool bDebugTargeting;
    static bool bDebugTapToTarget;

};
#pragma pack(pop)

VALIDATE_SIZE(CPlayerPedGta, 0x7AC-4);


