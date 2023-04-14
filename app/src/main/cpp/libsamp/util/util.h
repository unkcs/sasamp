#pragma once

#include "game/Entity/Vehicle.h"

static class CUtil {
public:
    static bool IsValidGameVehicle(VEHICLE_TYPE *pVehicle);
    static bool IsValidGamePed(PED_TYPE * pPed);

    static bool IsGameEntityArePlaceable(ENTITY_TYPE *pEntity);

    static void WorldRemoveEntity(uintptr_t pEnt);

    static const char *GetWeaponName(int iWeaponID);

    static int32_t FindPlayerSlotWithPedPointer(PED_TYPE *pPlayersPed);
};

uintptr_t FindLibrary(const char* library);
void cp1251_to_utf8(char *out, const char *in, unsigned int len = 0);

void AND_OpenLink(const char* szLink);
char* ConvertColorToHtml(std::string format);