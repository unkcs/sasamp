//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "main.h"
#include "game/Tasks/TaskManager.h"

class CPedIntelligence
{
public:
    class CPedGta* m_pPed;
    CTaskManager   m_TaskMgr;
//    CEventHandler          m_eventHandler;
//    CEventGroup            m_eventGroup;
//    int32                  m_nDecisionMakerType;
//    int32                  m_nDecisionMakerTypeInGroup;
//    float                  m_fHearingRange;
//    float                  m_fSeeingRange;
//    uint32                 m_nDmNumPedsToScan;
//    float                  m_fDmRadius;
//    float                  field_CC;
//    char                   field_D0;
//    uint8                  m_nEventId;
//    uint8                  m_nEventPriority;
//    char                   field_D3;
//    CVehicleScanner        m_vehicleScanner;
//    CPedScanner            m_pedScanner;
//    CMentalState           m_mentalState;
//    char                   field_188;
//    CEventScanner          m_eventScanner;
//    CCollisionEventScanner m_collisionScanner;
//    CPedStuckChecker       m_pedStuckChecker;
//    int32                  m_AnotherStaticCounter;
//    int32                  m_StaticCounter;
//    CVector                m_vecLastPedPosDuringDamageEntity;
//    CEntity*               m_apInterestingEntities[3];
    uint8_t pad0[612];

public:
    static void InjectHooks();

    CPedIntelligence(CPedGta* ped);
    ~CPedIntelligence();

    void Process();
};
VALIDATE_SIZE(CPedIntelligence, 0x298);

