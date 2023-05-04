//
// Created by plaka on 07.03.2023.
//

#pragma once

#include "common.h"
#include "game/Entity/Ped/Ped.h"
#include "game/Entity/Ped/PlayerPedGta.h"

struct CPlayerInfoGta {
    CPlayerPedGta *m_pPed;
    uint8_t skip[400];
};
VALIDATE_SIZE(CPlayerInfoGta, 0x194);

