//
// Created by plaka on 05.05.2023.
//

#include "Ped.h"
#include "../../../util/patch.h"
#include "game/Plugins/RpAnimBlendPlugin/RpAnimBlend.h"

void CPedGta::GetBonePosition(RwV3d *posn, uint32 boneTag, bool bCalledFromCamera) {
    CHook::CallFunction<void>(g_libGTASA + 0x00436590 + 1, this, posn, boneTag, bCalledFromCamera);
}

/*
 * �������� �� ��������� ����������. ��� ��������!
 */
void CPedGta::RemoveFromVehicleAndPutAt(const CVector& pos)
{
    if (!this) return;
    if(!pVehicle) return;

    m_pIntelligence->m_TaskMgr.FlushImmediately();

    SetPosn(pos);

    UpdateRW();
    UpdateRwFrame();//? ���� ��
}

/*
 * �������� ����� �� ������. ��� ��������!
 */
void CPedGta::RemoveFromVehicle()
{
    if (!this) return;
    if(!pVehicle) return;

    m_pIntelligence->m_TaskMgr.FlushImmediately();

    auto pos = pVehicle->GetPosition();
    float ang = pVehicle->GetHeading();

    pos.x += (1.0f * sin(ang + 4.71239f)); // 270 deg
    pos.y += (1.0f * sin(-ang + 4.71239f));

    SetPosn(pos);
}

void CPedGta::StopNonPartialAnims() {
    for (auto assoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump); assoc; assoc = RpAnimBlendGetNextAssociation(assoc)) {
        if ((assoc->m_nFlags & ANIMATION_PARTIAL) == 0) {
            assoc->SetFlag(ANIMATION_STARTED, false);
        }
    }
}

CPedGta::CPedGta(ePedType pedType) : CPhysical(), m_pedIK{CPedIK(this)} {
    m_vecAnimMovingShiftLocal = CVector2D();

    m_fHealth = 100.0f;
    m_fMaxHealth = 100.0f;
    m_fArmour = 0.0f;

    m_nPedType = pedType;
    m_nType = ENTITY_TYPE_PED;

    // 0x5E8196
    physicalFlags.bCanBeCollidedWith = true;
    physicalFlags.bDisableTurnForce = true;

    m_nCreatedBy = PED_GAME;
    pVehicle = nullptr;
    m_nAntiSpazTimer = 0;
    m_nUnconsciousTimer = 0;
    m_nAttackTimer = 0;
    m_nLookTime = 0;
    m_nDeathTimeMS = 0;

    m_vecAnimMovingShift = CVector2D();
//    field_56C = CVector();
//    field_578 = CVector(0.0f, 0.0f, 1.0f);

    m_nPedState = PEDSTATE_IDLE;
    m_nMoveState = PEDMOVE_STILL;
    m_fCurrentRotation = 0.0f;
    m_fHeadingChangeRate = 15.0f;
    m_fMoveAnim = 0.1f;
    m_fAimingRotation = 0.0f;
  //  m_standingOnEntity = nullptr;
    m_nWeaponShootingRate = 40;
 //   field_594 = 0;
   // m_pEntityIgnoredCollision = nullptr;
   // m_nSwimmingMoveState = 0;
    m_pFire = nullptr;
    m_fireDmgMult = 1.0f;
   // m_pTargetedObject = nullptr;
    m_pLookTarget = nullptr;
    m_fLookDirection = 0.0f;
  //  m_pContactEntity = nullptr;
   // field_588 = 99999.992f;
    m_fMass = 70.0f;
    m_fTurnMass = 100.0f;
    m_fAirResistance = 1.f / 175.f;
    m_fElasticity = 0.05f;
//    m_nBodypartToRemove = -1;
    bHasACamera = CGeneral::GetRandomNumber() % 4 != 0;

   // m_weaponAudio.Initialise(this);
   // m_pedAudio.Initialise(this);

  //  m_acquaintance = CPedType::GetPedTypeAcquaintances(m_nPedType);
    m_nSavedWeapon = WEAPON_UNIDENTIFIED;
    m_nDelayedWeapon = WEAPON_UNIDENTIFIED;
    m_nActiveWeaponSlot = 0;

    for (auto& weapon : m_aWeapons ) {
        weapon.m_nType = WEAPON_UNARMED;
        weapon.m_nState = WEAPONSTATE_READY;
        weapon.dwAmmoInClip = 0;
        weapon.m_nTotalAmmo = 0;
      //  weapon.m_nTimeForNextShot = 0;
    }

    m_nWeaponSkill = eWeaponSkill::STD;
    m_nFightingStyle = STYLE_STANDARD;
    m_nAllowedAttackMoves = 0;

   // GiveWeapon(WEAPON_UNARMED, 0, true);

    m_nWeaponAccuracy = 60;
  //  m_nLastWeaponDamage = -1;
    m_pLastEntityDamage = nullptr;
  //  field_768 = 0;
    m_pAttachedTo = nullptr;
  //  m_nTurretAmmo = 0;
  //  field_460 = nullptr;
  // field_468 = 0;
    m_nWeaponModelId = -1;
 //   m_nMoneyCount = 0;
    field_72F = 0;
//    m_nTimeTillWeNeedThisPed = 0;
    m_VehDeadInFrontOf = nullptr;

    m_pWeaponObject = nullptr;
    m_pGunflashObject = nullptr;
    m_pGogglesObject = nullptr;
    m_pGogglesState = nullptr;

    m_nWeaponGunflashAlphaMP1 = 0;
    m_nWeaponGunFlashAlphaProgMP1 = 0;
    m_nWeaponGunflashAlphaMP2 = 0;
    m_nWeaponGunFlashAlphaProgMP2 = 0;

    m_pCoverPoint = nullptr;
 //   m_pEnex = nullptr;
  //  field_798 = -1;

    m_pIntelligence = new CPedIntelligence(this);
    m_pPlayerData = nullptr;

//    if (!IsPlayer()) {
//        GetTaskManager().SetTaskSecondary(new CTaskComplexFacial{}, TASK_SECONDARY_FACIAL_COMPLEX);
//    }
  //  GetTaskManager().SetTask(new CTaskSimpleStandStill{ 0, true, false, 8.0 }, TASK_PRIMARY_DEFAULT, false);

   // field_758 = 0;
    m_fRemovalDistMultiplier = 1.0f;
    m_nSpecialModelIndex = -1;

 //   CPopulation::UpdatePedCount(this, 0);

//    if (CCheat::IsActive(CHEAT_HAVE_ABOUNTY_ON_YOUR_HEAD)) {
//        if (!IsPlayer()) {
//            GetAcquaintance().SetAsAcquaintance(ACQUAINTANCE_HATE, CPedType::GetPedFlag(ePedType::PED_TYPE_PLAYER1));
//
//            CEventAcquaintancePedHate event(FindPlayerPed());
//            GetEventGroup().Add(&event);
//        }
//    }
}