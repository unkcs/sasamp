//
// Created by plaka on 07.04.2023.
//

#pragma once

enum ePrimaryTasks //array indexes
{
    TASK_PRIMARY_PHYSICAL_RESPONSE = 0,
    TASK_PRIMARY_EVENT_RESPONSE_TEMP,
    TASK_PRIMARY_EVENT_RESPONSE_NONTEMP,
    TASK_PRIMARY_PRIMARY,
    TASK_PRIMARY_DEFAULT,
    TASK_PRIMARY_MAX
};

enum eSecondaryTasks : uint32_t//array indexes
{
    TASK_SECONDARY_ATTACK = 0,                // want duck to be after attack
    TASK_SECONDARY_DUCK,                    // because attack controls ducking movement
    TASK_SECONDARY_SAY,
    TASK_SECONDARY_FACIAL_COMPLEX,
    TASK_SECONDARY_PARTIAL_ANIM,
    TASK_SECONDARY_IK,
    TASK_SECONDARY_MAX
};

class CTaskManager
{
public:
    void* m_aPrimaryTasks[5];
    void* m_aSecondaryTasks[6];
    class CPed* m_pPed;
};

class CPedIntelligence
{
public:
    class CPed* m_pPed;
    CTaskManager   m_TaskMgr;
};

