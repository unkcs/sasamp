//
// Created by plaka on 07.04.2023.
//

#pragma once

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

