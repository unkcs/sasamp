//
// Created by plaka on 10.05.2023.
//

#include "PedIntelligence.h"
#include "../util/patch.h"

void CPedIntelligence::Process() {
//    g_LoadMonitor.StartTimer(0);

//    m_vehicleScanner.ScanForVehiclesInRange(*m_pPed);

//    m_pedScanner.ScanForPedsInRange(*m_pPed);

//    m_eventScanner.ScanForEvents(*m_pPed);

    CHook::CallFunction<void>(g_libGTASA + 0x0033460C  + 1, (CPedIntelligence *)((char *)this + 0x34));
//    m_eventHandler.HandleEvents();

    CHook::CallFunction<void>(g_libGTASA + 0x004BAC10 + 1, &this->m_TaskMgr);
   // m_TaskMgr.ManageTasks();

//    GetPlayerRelationshipRecorder().RecordRelationshipWithPlayer(m_pPed);

//    LookAtInterestingEntities();

//    g_LoadMonitor.EndTimer(0);
}

CPedIntelligence::CPedIntelligence(CPedGta* ped) :
        m_pPed{ ped },
        m_TaskMgr{ CTaskManager(ped) }
      //  m_eventHandler{ CEventHandler(ped) },
       // m_eventGroup{ CEventGroup(ped) }
{
//    m_nDecisionMakerType                  = DM_EVENT_UNDEFINED;
//    m_nDecisionMakerTypeInGroup           = -1;
//    m_fHearingRange                       = 15.0f;
//    m_fSeeingRange                        = 15.0f;
//    m_nDmNumPedsToScan                    = 3;
//    m_fDmRadius                           = 15.0f;
//    field_CC                              = 30.0f;
//    field_D0                              = -1;
//    m_nEventId                            = 0;
//    m_nEventPriority                      = 0;
//    field_188                             = 0;
//    m_collisionScanner.m_bAlreadyHitByCar = false;
//    m_AnotherStaticCounter                = 0;
//    m_StaticCounter                       = 0;
//    if (IsPedTypeGang(ped->m_nPedType)) {
//        m_fSeeingRange = 40.f;
//        m_fHearingRange = 40.f;
//    }
//    std::ranges::fill(m_apInterestingEntities, nullptr);
}


// -------- hooks


void CPedIntelligence__Process(CPedIntelligence* thiz) {
    thiz->Process();
}

void CPedIntelligence::InjectHooks() {
    CHook::Redirect(g_libGTASA + 0x00452FC0, &CPedIntelligence__Process);
}
