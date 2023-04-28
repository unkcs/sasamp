
#include "Collision.h"
#include "util/patch.h"

void CCollision::InjectHooks() {
    CHook::Write(g_libGTASA + 0x005CF884, &ms_colModelCache);
    CHook::Write(g_libGTASA + 0x005D022C, &ms_iProcessLineNumCrossings);
    CHook::Write(g_libGTASA + 0x005CF174, &ms_collisionInMemory);

    CHook::Write(g_libGTASA + 0x005D0C58, &bCamCollideWithVehicles);
}
