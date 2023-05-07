/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Task.h"

class CPedGta;

#pragma pack(push, 1)
class CTaskSimple : public CTask {
public:
    CTaskSimple();
    ~CTaskSimple() override = default;

    CTask* GetSubTask() override;
    bool IsSimple() override;
    virtual bool ProcessPed(CPedGta* ped) = 0;
    virtual bool SetPedPosition(CPedGta* ped);
};
static_assert(sizeof(CTaskSimple)==0x8);
