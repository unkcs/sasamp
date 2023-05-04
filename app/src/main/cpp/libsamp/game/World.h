//
// Created by plaka on 04.05.2023.
//

#pragma once

#include "common.h"
#include "game/Core/Rect.h"
#include "CPlayerInfoGta.h"

constexpr int32 MAX_WORLD_UNITS = 6000;

constexpr int32 MAX_SECTORS_X = 120;
constexpr int32 MAX_SECTORS_Y = 120;
constexpr int32 MAX_SECTORS = MAX_SECTORS_X * MAX_SECTORS_Y;

constexpr int32 MAX_REPEAT_SECTORS_X = 16;
constexpr int32 MAX_REPEAT_SECTORS_Y = 16;
constexpr int32 MAX_REPEAT_SECTORS = MAX_REPEAT_SECTORS_X * MAX_REPEAT_SECTORS_Y;

constexpr int32 MAX_LOD_PTR_LISTS_X = 30;
constexpr int32 MAX_LOD_PTR_LISTS_Y = 30;
constexpr int32 MAX_LOD_PTR_LISTS = MAX_LOD_PTR_LISTS_X * MAX_LOD_PTR_LISTS_Y;

constexpr inline CRect WORLD_BOUNDS{-3000.0F, -3000.0F, 3000.0F, 3000.0F};
constexpr float MAP_Z_LOW_LIMIT = -100.0f;

class CWorld {
public:

    static inline CPlayerInfoGta Players[MAX_PLAYERS];
    static inline int PlayerInFocus;
};

CPlayerInfoGta&   FindPlayerInfo(int32 playerId = -1);
CPlayerPedGta*    FindPlayerPed(int32 playerId = -1);
CVehicleGta*      FindPlayerVehicle(int32 playerId = -1, bool bIncludeRemote = false);