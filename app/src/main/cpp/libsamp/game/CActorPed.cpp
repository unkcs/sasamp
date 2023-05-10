#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "common.h"

#include "..//CDebugInfo.h"

#include "CActorPed.h"

#include "game/Entity/Ped/Ped.h"
extern CGame* pGame;

CActorPed::CActorPed(uint16_t usModel, CVector vecPosition, float fRotation, float fHealth, bool bInvulnerable)
{
	

	m_dwGTAId = 0;
	m_pPed = nullptr;
	m_pEntity = nullptr;

	if (!pGame->IsModelLoaded(usModel)) 
	{
		pGame->RequestModel(usModel);
		pGame->LoadRequestedModels();
	}

	if (!IsPedModel(usModel)) 
	{
		usModel = 0;
	}

	uint32_t actorGTAId = 0;
	ScriptCommand(&create_actor, 22, usModel, vecPosition.x, vecPosition.y, vecPosition.z, &actorGTAId);

	m_dwGTAId = actorGTAId;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (CPhysical*)m_pPed;

	ForceTargetRotation(fRotation);
	m_pEntity->SetPosn(vecPosition);

	if (fHealth < 1.0f) 
	{
		SetDead();
	}
	else 
	{
		SetHealth(fHealth);
	}

	ScriptCommand(&lock_actor, m_dwGTAId, 1);

	if (bInvulnerable) 
	{
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
	}
	else 
	{
		ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
	}
}

CActorPed::~CActorPed()
{
	
	Destroy();
}

bool IsValidGamePed(CPedGta* pPed)
{
	
	//IsPedPointerValid(CPed *) � 0x00435614
	if (((bool (*)(CPedGta*))(g_libGTASA + 0x00435614 + 1))(pPed)) {
		return true;
	}
	return false;
}

void CActorPed::Destroy()
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (IsValidGamePed(m_pPed)) 
	{
		// CPed::vtable + 0x4 destructor
		((void (*)(CPedGta*))(*(void**)(m_pPed->vtable + 0x4)))(m_pPed);
	}

	m_pPed = nullptr;
	m_pEntity = nullptr;
	m_dwGTAId = 0;
}

void CActorPed::SetHealth(float fHealth)
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	m_pPed->m_fHealth = fHealth;
}

void CActorPed::SetDead()
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	RwMatrix matEntity;
	GetMatrix(&matEntity);
	m_pEntity->SetPosn(matEntity.pos.x, matEntity.pos.y, matEntity.pos.z);

	SetHealth(0.0f);
	ScriptCommand(&kill_actor, m_dwGTAId);
}

void CActorPed::ForceTargetRotation(float fRotation)
{
	
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if (!IsValidGamePed(m_pPed)) 
	{
		return;
	}

	m_pPed->m_fCurrentRotation = DegToRad(fRotation);
	m_pPed->m_fAimingRotation = DegToRad(fRotation);

	ScriptCommand(&set_actor_z_angle, m_dwGTAId, fRotation);
}

void CActorPed::ApplyAnimation(char* szAnimName, char* szAnimFile, float fDelta, int bLoop, int bLockX, int bLockY, int bFreeze, int uiTime)
{
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(!pGame->IsAnimationLoaded(szAnimFile))
	{
		CGame::RequestAnimation(szAnimFile);
		ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
	}

	ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
}
//
//void CActorPed::PutDirectlyInVehicle(int iVehicleID, int iSeat)
//{
//	if (!m_pPed) return;
//	if (!GamePool_Vehicle_GetAt(iVehicleID)) return;
//	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
//
//	CVehicleGta* pVehicle = GamePool_Vehicle_GetAt(iVehicleID);
//
//	if (pVehicle->m_fHealth == 0.0f) return;
//	if (pVehicle->vtable == g_libGTASA + 0x5C7358) return;
//	// check seatid (��������)
//
//	if (iSeat == 0)
//	{
//		if (pVehicle->pDriver && IN_VEHICLE(pVehicle->pDriver)) return;
//		ScriptCommand(&TASK_WARP_CHAR_INTO_CAR_AS_DRIVER, m_dwGTAId, iVehicleID);
//	}
//	else
//	{
//		iSeat--;
//		ScriptCommand(&put_actor_in_car2, m_dwGTAId, iVehicleID, iSeat);
//	}
//}

void CActorPed::RemoveFromVehicle()
{
	if (!m_pPed) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

	RwMatrix mat;

	if (m_pPed->bInVehicle)
	{
		GetMatrix(&mat);
		RemoveFromVehicleAndPutAt(mat.pos.x, mat.pos.y, mat.pos.z);
	}
}

void CActorPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if (m_pPed && m_pPed->bInVehicle)
	{
		ScriptCommand(&remove_actor_from_car_and_put_at, m_dwGTAId, fX, fY, fZ);
	}
}
