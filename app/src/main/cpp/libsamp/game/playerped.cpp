#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "common.h"
#include "vehicle.h"

#include "..//CDebugInfo.h"
#include "util/patch.h"
#include "ePedState.h"
#include "CWorld.h"
#include "CModelInfo.h"

extern CGame* pGame;
extern CNetGame *pNetGame;
CPlayerPed* g_pCurrentFiredPed;
BULLET_DATA* g_pCurrentBulletData;

CPlayerPed::CPlayerPed()
{
	m_dwGTAId = 1;
	m_pPed = (PED_TYPE*)GamePool_FindPlayerPed();
	m_pEntity = (ENTITY_TYPE*)GamePool_FindPlayerPed();
	m_bHaveBulletData = false;

	m_bytePlayerNumber = 0;
	SetPlayerPedPtrRecord(m_bytePlayerNumber,(uintptr_t)m_pPed);
	ScriptCommand(&set_actor_weapon_droppable, m_dwGTAId, 1);
	ScriptCommand(&set_char_never_targeted, m_dwGTAId, 1);
	ScriptCommand(&set_actor_can_be_decapitated, m_dwGTAId, 0);
	m_dwArrow = 0;

	for (int i = 0; i < MAX_ATTACHED_OBJECTS; i++)
	{
		m_aAttachedObjects[i].bState = false;
	}
}

CPlayerPed::CPlayerPed(uint8_t bytePlayerNumber, int iSkin, float fX, float fY, float fZ, float fRotation)
{
	CDebugInfo::uiStreamedPeds++;

	uint32_t dwPlayerActorID = 0;
	int iPlayerNum = bytePlayerNumber;

	m_pPed = nullptr;
	m_dwGTAId = 0;
	m_dwArrow = 0;
	m_bHaveBulletData = false;

	ScriptCommand(&create_player, &iPlayerNum, fX, fY, fZ, &dwPlayerActorID);
	ScriptCommand(&create_actor_from_player, &iPlayerNum, &dwPlayerActorID);

	m_dwGTAId = dwPlayerActorID;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE*)GamePool_Ped_GetAt(m_dwGTAId);

	m_bytePlayerNumber = bytePlayerNumber;

	SetPlayerPedPtrRecord(m_bytePlayerNumber, (uintptr_t)m_pPed);
	ScriptCommand(&set_actor_weapon_droppable, m_dwGTAId, 1);
	ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
	ScriptCommand(&set_actor_can_be_decapitated, m_dwGTAId, 0); // ����� �����
	ScriptCommand(&set_char_never_targeted, m_dwGTAId, 1);

	ScriptCommand(&set_actor_money, m_dwGTAId, 0); // ������ ������ ��� ������

	SetModelIndex(iSkin);
	ForceTargetRotation(fRotation);
	RwMatrix mat;
	GetMatrix(&mat);
	mat.pos.X = fX;
	mat.pos.Y = fY;
	mat.pos.Z = fZ+ 0.15f;
	SetMatrix(mat);
	
	for (int i = 0; i < MAX_ATTACHED_OBJECTS; i++)
	{
		m_aAttachedObjects[i].bState = false;
	}

	memset(&RemotePlayerKeys[m_bytePlayerNumber], 0, sizeof(PAD_KEYS));
}

bool IsValidGamePed(PED_TYPE* pPed);

CPlayerPed::~CPlayerPed()
{
    Log("~CPlayerPed()");
	CDebugInfo::uiStreamedPeds--;

	memset(&RemotePlayerKeys[m_bytePlayerNumber], 0, sizeof(PAD_KEYS));

	SetPlayerPedPtrRecord(m_bytePlayerNumber, 0);

	if(!m_dwGTAId)return;
	if(!GamePool_Ped_GetAt(m_dwGTAId))return;

	if (m_pPed && IsValidGamePed(m_pPed) && m_pPed->entity.vtable != (g_libGTASA + 0x5C7358))
	{
		FlushAttach();
		if (m_pPed->bInVehicle) {
			RemoveFromVehicleAndPutAt(100.0f, 100.0f, 20.0f);

		//	ClearAllTasks();
		}
		uintptr_t dwPedPtr = (uintptr_t)m_pPed;
		*(uint32_t*)(*(uintptr_t*)(dwPedPtr + 1088) + 76) = 0;
		// CPlayerPed::Destructor

		//(( void (*)(PED_TYPE*))(*(void**)(m_pPed->entity.vtable+0x4)))(m_pPed);
		((void (*)(uintptr_t))(g_libGTASA+0x45D82C+1))((uintptr_t)m_pEntity);
		//ScriptCommand(&DELETE_CHAR, m_dwGTAId);

		m_pPed = nullptr;
		m_pEntity = nullptr;
		m_dwGTAId = 0;
	}
	else
	{
		m_pPed = nullptr;
		m_pEntity = nullptr;
		m_dwGTAId = 0;
	}
}

CAMERA_AIM * CPlayerPed::GetCurrentAim()
{
	return GameGetInternalAim();
}

void CPlayerPed::SetCurrentAim(CAMERA_AIM * pAim)
{
	GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

uint16_t CPlayerPed::GetCameraMode()
{
	return GameGetLocalPlayerCameraMode();
}

void CPlayerPed::SetCameraMode(uint16_t byteCamMode)
{
	GameSetPlayerCameraMode(byteCamMode, m_bytePlayerNumber);
}

float CPlayerPed::GetCameraExtendedZoom()
{
	return GameGetLocalPlayerCameraExtZoom();
}

void CPlayerPed::ApplyCrouch()
{
	
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	if (!(m_pPed->bIsDucking))
	{
		if (!IsCrouching())
		{
			if (m_pPed->pPedIntelligence)
			{
				((int (*)(CPedIntelligence*, uint16_t))(g_libGTASA + 0x0044E0F4 + 1))(m_pPed->pPedIntelligence, 0);
			}
		}
	}
}

void CPlayerPed::ResetCrouch()
{
	
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}
	m_pPed->bIsDucking = false;
	if (m_pPed->pPedIntelligence)
	{
		((int (*)(CPedIntelligence*))(g_libGTASA + 0x0044E164 + 1))(m_pPed->pPedIntelligence);
	}
	//bKeepTasksAfterCleanUp
}

bool CPlayerPed::IsCrouching()
{
	
	if (!m_pPed || !m_dwGTAId)
	{
		return false;
	}
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return false;
	}
	return m_pPed->bIsDucking;
}

void CPlayerPed::SetCameraExtendedZoom(float fZoom)
{
	GameSetPlayerCameraExtZoom(m_bytePlayerNumber, fZoom);
}

void CPlayerPed::SetCameraExtendedZoom(float fZoom, float fAspectRatio)
{
	GameSetPlayerCameraExtZoom(m_bytePlayerNumber, fZoom, fAspectRatio);
}


void CPlayerPed::SetDead()
{
	
	if (!m_dwGTAId || !m_pPed)
	{
		return;
	}
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}
	
	RwMatrix mat;
	GetMatrix(&mat);
	// will reset the tasks
	TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);
	m_pPed->fHealth = 0.0f;

	uint8_t old = CWorld::PlayerInFocus;
	CWorld::PlayerInFocus = m_bytePlayerNumber;
	ScriptCommand(&kill_actor, m_dwGTAId);
	CWorld::PlayerInFocus = 0;
}

// 0.3.7
bool CPlayerPed::IsInVehicle()
{
	if(!m_pPed) return false;

	return m_pPed->bInVehicle;
}
int GameGetWeaponModelIDFromWeaponID(int iWeaponID)
{
	switch (iWeaponID)
	{
	case WEAPON_BRASSKNUCKLE:
		return WEAPON_MODEL_BRASSKNUCKLE;

	case WEAPON_GOLFCLUB:
		return WEAPON_MODEL_GOLFCLUB;

	case WEAPON_NITESTICK:
		return WEAPON_MODEL_NITESTICK;

	case WEAPON_KNIFE:
		return WEAPON_MODEL_KNIFE;

	case WEAPON_BAT:
		return WEAPON_MODEL_BAT;

	case WEAPON_SHOVEL:
		return WEAPON_MODEL_SHOVEL;

	case WEAPON_POOLSTICK:
		return WEAPON_MODEL_POOLSTICK;

	case WEAPON_KATANA:
		return WEAPON_MODEL_KATANA;

	case WEAPON_CHAINSAW:
		return WEAPON_MODEL_CHAINSAW;

	case WEAPON_DILDO:
		return WEAPON_MODEL_DILDO;

	case WEAPON_DILDO2:
		return WEAPON_MODEL_DILDO2;

	case WEAPON_VIBRATOR:
		return WEAPON_MODEL_VIBRATOR;

	case WEAPON_VIBRATOR2:
		return WEAPON_MODEL_VIBRATOR2;

	case WEAPON_FLOWER:
		return WEAPON_MODEL_FLOWER;

	case WEAPON_CANE:
		return WEAPON_MODEL_CANE;

	case WEAPON_GRENADE:
		return WEAPON_MODEL_GRENADE;

	case WEAPON_TEARGAS:
		return WEAPON_MODEL_TEARGAS;

	case WEAPON_MOLTOV:
		return -1;

	case WEAPON_COLT45:
		return WEAPON_MODEL_COLT45;

	case WEAPON_SILENCED:
		return WEAPON_MODEL_SILENCED;

	case WEAPON_DEAGLE:
		return WEAPON_MODEL_DEAGLE;

	case WEAPON_SHOTGUN:
		return WEAPON_MODEL_SHOTGUN;

	case WEAPON_SAWEDOFF:
		return WEAPON_MODEL_SAWEDOFF;

	case WEAPON_SHOTGSPA:
		return WEAPON_MODEL_SHOTGSPA;

	case WEAPON_UZI:
		return WEAPON_MODEL_UZI;

	case WEAPON_MP5:
		return WEAPON_MODEL_MP5;

	case WEAPON_AK47:
		return WEAPON_MODEL_AK47;

	case WEAPON_M4:
		return WEAPON_MODEL_M4;

	case WEAPON_TEC9:
		return WEAPON_MODEL_TEC9;

	case WEAPON_RIFLE:
		return WEAPON_MODEL_RIFLE;

	case WEAPON_SNIPER:
		return WEAPON_MODEL_SNIPER;

	case WEAPON_ROCKETLAUNCHER:
		return WEAPON_MODEL_ROCKETLAUNCHER;

	case WEAPON_HEATSEEKER:
		return WEAPON_MODEL_HEATSEEKER;

	case WEAPON_FLAMETHROWER:
		return WEAPON_MODEL_FLAMETHROWER;

	case WEAPON_MINIGUN:
		return WEAPON_MODEL_MINIGUN;

	case WEAPON_SATCHEL:
		return WEAPON_MODEL_SATCHEL;

	case WEAPON_BOMB:
		return WEAPON_MODEL_BOMB;

	case WEAPON_SPRAYCAN:
		return WEAPON_MODEL_SPRAYCAN;

	case WEAPON_FIREEXTINGUISHER:
		return WEAPON_MODEL_FIREEXTINGUISHER;

	case WEAPON_CAMERA:
		return WEAPON_MODEL_CAMERA;

	case -1:
		return WEAPON_MODEL_NIGHTVISION;

	case -2:
		return WEAPON_MODEL_INFRARED;

	case WEAPON_PARACHUTE:
		return WEAPON_MODEL_PARACHUTE;

	}

	return -1;
}


void CPlayerPed::SetWeaponAmmo(int iWeaponID, int iAmmo)
{
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	int iModelID = 0;
	iModelID = GameGetWeaponModelIDFromWeaponID(iWeaponID);

	if (iModelID == -1) return;

	ScriptCommand(&SET_CHAR_AMMO, m_dwGTAId, iWeaponID, iAmmo);
	SetArmedWeapon(iWeaponID);
}
void CPlayerPed::GiveWeapon(int iWeaponID, int iAmmo)
{
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	int iModelID = 0;
	iModelID = GameGetWeaponModelIDFromWeaponID(iWeaponID);
	
	if (iModelID == -1) return;
	
	if (!pGame->IsModelLoaded(iModelID)) 
	{
		pGame->RequestModel(iModelID);
		pGame->LoadRequestedModels();
		while (!pGame->IsModelLoaded(iModelID)) sleep(1);
	}
	//ScriptCommand(&give_actor_weapon, m_dwGTAId, iWeaponID, iAmmo);

	((int(*)(uintptr_t, unsigned int, int))(g_libGTASA + 0x0043429C + 1))((uintptr_t)m_pPed, iWeaponID, iAmmo); // CPed::GiveWeapon(thisptr, weapoid, ammo)
	((int(*)(uintptr_t, unsigned int))(g_libGTASA + 0x00434528 + 1))((uintptr_t)m_pPed, iWeaponID);	// CPed::SetCurrentWeapon(thisptr, weapid)
}

void CPlayerPed::SetArmedWeapon(int iWeaponID)
{
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	((int(*)(uintptr_t, unsigned int))(g_libGTASA + 0x00434528 + 1))((uintptr_t)m_pPed, iWeaponID);	// CPed::SetCurrentWeapon(thisptr, weapid)
}

void CPlayerPed::SetPlayerAimState()
{
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	uintptr_t ped = (uintptr_t)m_pPed;
	uint8_t old = CWorld::PlayerInFocus;
	CWorld::PlayerInFocus = m_bytePlayerNumber;

	((uint32_t(*)(uintptr_t, int, int, int))(g_libGTASA + 0x00454A6C + 1))(ped, 1, 1, 1); // CPlayerPed::ClearWeaponTarget
	*(uint8_t *)(*(uint32_t *)(ped + 1088) + 52) = *(uint8_t *)(*(uint32_t *)(ped + 1088) + 52) & 0xF7 | 8 * (1 & 1); // magic 

	CWorld::PlayerInFocus = old;
}

void CPlayerPed::ApplyCommandTask(char* a2, int a4, int a5, int a6, VECTOR* a7, char a8, float a9, int a10, int a11, char a12)
{
	uint32_t dwPed = (uint32_t)m_pPed;
	if (!dwPed) return;
	// 00958484 - g_ikChainManager
	// 00463188 addr
	((int(*)(uintptr_t a1, char* a2, uint32_t a3, int a4, int a5, int a6, VECTOR* a7, char a8, float a9, int a10, int a11, char a12))(g_libGTASA + 0x00463188 + 1))
		(g_libGTASA + 0x00958484, a2, dwPed, a4, a5, a6, a7, a8, a9, a10, a11, a12);

}

void CPlayerPed::ClearPlayerAimState()
{
	if (!m_pPed || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	uintptr_t ped = (uintptr_t)m_pPed;
	uint8_t old = CWorld::PlayerInFocus;
	CWorld::PlayerInFocus = m_bytePlayerNumber;

	*(uint32_t *)(ped + 1432) = 0;	// unk
	((uint32_t(*)(uintptr_t, int, int, int))(g_libGTASA + 0x00454A6C + 1))(ped, 0, 0, 0);	// CPlayerPed::ClearWeaponTarget
	*(uint8_t *)(*(uint32_t *)(ped + 1088) + 52) = *(uint8_t *)(*(uint32_t *)(ped + 1088) + 52) & 0xF7 | 8 * (0 & 1);	// magic...

	CWorld::PlayerInFocus = old;
}

BYTE CPlayerPed::GetCurrentWeapon()
{
	if(!m_pPed) return 0;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0;

	DWORD dwRetVal;
	ScriptCommand(&get_actor_armed_weapon,m_dwGTAId,&dwRetVal);
	return (BYTE)dwRetVal;
}

// 0.3.7
bool CPlayerPed::IsAPassenger()
{
	//if(m_pPed->m_nPedState == PEDSTATE_PASSENGER) return true;
	if(m_pPed->pVehicle && m_pPed->bInVehicle)
	{
		VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

		if(	pVehicle->pDriver != m_pPed ||
			pVehicle->entity.nModelIndex == TRAIN_PASSENGER ||
			pVehicle->entity.nModelIndex == TRAIN_FREIGHT )
			return true;
	}

	return false;
}

// 0.3.7
VEHICLE_TYPE* CPlayerPed::GetGtaVehicle()
{
	return (VEHICLE_TYPE*)m_pPed->pVehicle;
}

// 0.3.7
void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}
	if (m_pPed->entity.vtable == (g_libGTASA + 0x5C7358)) return;
	if(m_pPed && m_pPed->bInVehicle)
		ScriptCommand(&remove_actor_from_car_and_put_at, m_dwGTAId, fX, fY, fZ);
}

// 0.3.7
int CPlayerPed::SetInitialState()
{
	Log("CPlayerPed::SetInitialState()1");
	int a1 = (( int (*)(PED_TYPE*, bool))(g_libGTASA+0x458D1C+1))(m_pPed, 0);

	Log("CPlayerPed::SetInitialState()2");
	return a1;
}

// 0.3.7
void CPlayerPed::SetHealth(float fHealth)
{
	if(!m_pPed) return;
	if (IsValidGamePed(m_pPed))
	{
		m_pPed->fHealth = fHealth;
	}

}

// 0.3.7
float CPlayerPed::GetHealth()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fHealth;
}

// 0.3.7
void CPlayerPed::SetArmour(float fArmour)
{
	if(!m_pPed) return;
	m_pPed->fArmour = fArmour;
}

float CPlayerPed::GetArmour()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fArmour;
}

void CPlayerPed::SetInterior(uint8_t byteID, bool refresh)
{
	if(!m_pPed) return;

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}

	if(m_pPed && m_bytePlayerNumber != 0) {
		ScriptCommand(&link_actor_to_interior, m_dwGTAId, byteID);
	}
	else {
		ScriptCommand(&select_interior, byteID);
		ScriptCommand(&link_actor_to_interior, m_dwGTAId, byteID);

		if(refresh) {
			RwMatrix mat;
			this->GetMatrix(&mat);
			ScriptCommand(&refresh_streaming_at, mat.pos.X, mat.pos.Y);
		}
	}
}

void CPlayerPed::PutDirectlyInVehicle(CVehicle *pVehicle, int iSeat)
{

	if(!pVehicle) return;
	if (!GamePool_Vehicle_GetAt(pVehicle->m_dwGTAId)) return;

	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}
	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}
	if (m_pPed->entity.vtable == (g_libGTASA + 0x5C7358)) return;


    VEHICLE_TYPE *gtaVehicle = pVehicle->m_pVehicle;
    if(!gtaVehicle)return;
    if(gtaVehicle->fHealth == 0.0f) return;
    // check is cplaceable
    if (gtaVehicle->entity.vtable == g_libGTASA+0x5C7358) return;

	/*
	if(GetCurrentWeapon() == WEAPON_PARACHUTE) {
		SetArmedWeapon(0);
	}*/

	// check seatid

	Log("PutDirectlyInVehicle");

//	RwMatrix mat;
//	pVehicle->GetMatrix(&mat);
//
////	GetMatrix(&mat);
////	mat.pos.X = pVehicle->entity.mat->pos.X;
////	mat.pos.Y = pVehicle->entity.mat->pos.Y;
////	mat.pos.Z = pVehicle->entity.mat->pos.Z;
//	SetMatrix(mat);

	if(iSeat == 0)
	{
		if(pVehicle->m_pVehicle->pDriver) return;
		ScriptCommand(&TASK_WARP_CHAR_INTO_CAR_AS_DRIVER, m_dwGTAId, pVehicle->m_dwGTAId);
	}
	else
	{
		iSeat--;
		ScriptCommand(&put_actor_in_car2, m_dwGTAId, pVehicle->m_dwGTAId, iSeat);
	}
}

void CPlayerPed::EnterVehicle(int iVehicleID, bool bPassenger)
{
	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}
	if (m_pPed->entity.vtable == (g_libGTASA + 0x5C7358)) return;

	VEHICLE_TYPE* ThisVehicleType;
	if((ThisVehicleType = GamePool_Vehicle_GetAt(iVehicleID)) == 0) return;
	if (ThisVehicleType->fHealth == 0.0f) return;
	if (ThisVehicleType->entity.vtable == g_libGTASA + 0x5C7358) return;

	if(bPassenger)
	{
		if(ThisVehicleType->entity.nModelIndex == TRAIN_PASSENGER &&
			(m_pPed == GamePool_FindPlayerPed()))
		{
			ScriptCommand(&put_actor_in_car2, m_dwGTAId, iVehicleID, -1);
		}
		else
		{
			ScriptCommand(&send_actor_to_car_passenger,m_dwGTAId,iVehicleID, 3000, -1);
		}
	}
	else{
		ScriptCommand(&TASK_ENTER_CAR_AS_DRIVER, m_dwGTAId, iVehicleID, -1);
	}

}

// 0.3.7
void CPlayerPed::ExitCurrentVehicle()
{
    Log("ExitCurrentVehicle");
	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}
	if (m_pPed->entity.vtable == (g_libGTASA + 0x5C7358)) return;

	//VEHICLE_TYPE* ThisVehicleType = 0;

	if(m_pPed->bInVehicle)
	{
		ScriptCommand(&TASK_LEAVE_ANY_CAR, m_dwGTAId);

	}
}

// 0.3.7
VEHICLEID CPlayerPed::GetCurrentSampVehicleID()
{
	if(!m_pPed) return INVALID_VEHICLE_ID;
	if(!pNetGame)return INVALID_VEHICLE_ID;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(!pVehiclePool)return INVALID_VEHICLE_ID;
	if(!m_pPed->pVehicle)return INVALID_VEHICLE_ID;

	return pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)m_pPed->pVehicle);
}
CVehicle* CPlayerPed::GetCurrentVehicle()
{
	if(!m_pPed) return nullptr;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	for (size_t i = 0; i < MAX_VEHICLES; i++) {
		if (pVehiclePool->GetSlotState(i)) {
			CVehicle *pVehicle = pVehiclePool->GetAt(i);
			if (pVehicle && pVehicle->IsAdded()) {
				if (pVehicle->m_pVehicle == (VEHICLE_TYPE *) m_pPed->pVehicle) {
					return pVehicle;
				}
			}
		}
	}
	return nullptr;
}

VEHICLE_TYPE* CPlayerPed::GetCurrentGtaVehicle()
{
	if(!m_pPed) return nullptr;

	return (VEHICLE_TYPE *)m_pPed->pVehicle;
}

uint32_t CPlayerPed::GetCurrentGTAVehicleID(){
	if(!m_pPed) return 0;
	return GamePool_Vehicle_GetIndex(reinterpret_cast<VEHICLE_TYPE *>(m_pPed->pVehicle));
}

int CPlayerPed::GetVehicleSeatID()
{
	auto *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

	if( pVehicle->pDriver == m_pPed) return 0;

	if(pVehicle->pPassengers[0] == m_pPed) return 1;
	if(pVehicle->pPassengers[1] == m_pPed) return 2;
	if(pVehicle->pPassengers[2] == m_pPed) return 3;
	if(pVehicle->pPassengers[3] == m_pPed) return 4;
	if(pVehicle->pPassengers[4] == m_pPed) return 5;
	if(pVehicle->pPassengers[5] == m_pPed) return 6;
	if(pVehicle->pPassengers[6] == m_pPed) return 7;

	return (-1);
}

// 0.3.7
void CPlayerPed::TogglePlayerControllable(bool bToggle, bool isTemp)
{
	if(!isTemp) lToggle = bToggle;

	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}
	if (m_pPed->entity.vtable == (g_libGTASA + 0x5C7358)) return;

	if(!bToggle)
	{
		ScriptCommand(&toggle_player_controllable, m_bytePlayerNumber, 0);
		ScriptCommand(&lock_actor, m_dwGTAId, 1);
	}
	else if(lToggle)
	{
		ScriptCommand(&toggle_player_controllable, m_bytePlayerNumber, 1);
		ScriptCommand(&lock_actor, m_dwGTAId, 0);
	}

}

// 0.3.7
void CPlayerPed::SetModelIndex(unsigned int uiModel)
{
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(!IsPedModel(uiModel))
		uiModel = 0;

	if(m_pPed)
	{
		// CClothes::RebuildPlayer nulled
		CHook::WriteMemory(g_libGTASA + 0x3F1030, "\x70\x47", 2);
		DestroyFollowPedTask();
		CEntity::SetModelIndex(uiModel);

		// reset the Ped Audio Attributes
		(( void (*)(uintptr_t, uintptr_t))(g_libGTASA+0x34B2A8+1))(((uintptr_t)m_pPed+660), (uintptr_t)m_pPed);
	}
}

bool CPlayerPed::IsAnimationPlaying(char* szAnimName)
{
	if (!m_pPed) return false;
	if (!GamePool_Ped_GetAt(m_dwGTAId)) return false;
	if (!szAnimName || !strlen(szAnimName)) return false;

	if (ScriptCommand(&is_char_playing_anim, m_dwGTAId, szAnimName)) {
		return true;
	}

	return false;
}

void CPlayerPed::ClearAllTasks()
{
	if (!GamePool_Ped_GetAt(m_dwGTAId) || !m_pPed) {
		return;
	}

	ScriptCommand(&clear_char_tasks, m_dwGTAId);
}

void CPlayerPed::ClearAnimations()
{
	ApplyAnimation("crry_prtial", "CARRY", 4.0, 0, 0, 0, 0, 1);
	//ClearAllTasks();
	RwMatrix mat;
	GetMatrix(&mat);
	TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);

	Log("ClearAnimations");
}

void CPlayerPed::DestroyFollowPedTask()
{

}

int Weapon_FireSniper(CWeapon* pWeaponSlot, PED_TYPE* pPed)
{
	return ((int (*)(CWeapon*, PED_TYPE*))(g_libGTASA + 0x0056668C + 1))(pWeaponSlot, pPed);
}

void CPlayerPed::ClearAllWeapons()
{
	uintptr_t dwPedPtr = (uintptr_t)m_pPed;
	uint8_t old = CWorld::PlayerInFocus;
	CWorld::PlayerInFocus = m_bytePlayerNumber;

	((uint32_t(*)(uintptr_t, int, int, int))(g_libGTASA + 0x004345AC + 1))(dwPedPtr, 1, 1, 1); // CPed::ClearWeapons(void)

	CWorld::PlayerInFocus = old;
}

uintptr_t GetWeaponInfo(int iWeapon, int iSkill)
{
	// CWeaponInfo::GetWeaponInfo
	return ((uintptr_t(*)(int, int))(g_libGTASA + 0x0056BD60 + 1))(iWeapon, iSkill);
}

VECTOR* CPlayerPed::GetCurrentWeaponFireOffset() {
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return nullptr;
	}

	CWeapon* pSlot = GetCurrentWeaponSlot();
	if (pSlot) {
		return (VECTOR*)(GetWeaponInfo(pSlot->dwType, 1) + 0x24);
	}
	return nullptr;
}

void CPlayerPed::GetWeaponInfoForFire(int bLeft, VECTOR* vecBone, VECTOR* vecOut) {
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}

	VECTOR* pFireOffset = GetCurrentWeaponFireOffset();
	if (pFireOffset && vecBone && vecOut) {
		vecOut->X = pFireOffset->X;
		vecOut->Y = pFireOffset->Y;
		vecOut->Z = pFireOffset->Z;

		int bone_id = 24;
		if (bLeft) {
			bone_id = 34;
		}

		// CPed::GetBonePosition
		((void (*)(PED_TYPE*, VECTOR*, int, bool))(g_libGTASA + 0x00436590 + 1))(m_pPed, vecBone, bone_id, false);

		vecBone->Z += pFireOffset->Z + 0.15f;

		// CPed::GetTransformedBonePosition
		((void (*)(PED_TYPE*, VECTOR*, int, bool))(g_libGTASA + 0x004383C0 + 1))(m_pPed, vecOut, bone_id, false);
	}
}

extern uint32_t (*CWeapon__FireInstantHit)(CWeapon* thiz, PED_TYPE* pFiringEntity, VECTOR* vecOrigin, VECTOR* muzzlePosn, ENTITY_TYPE* targetEntity, VECTOR *target, VECTOR* originForDriveBy, int arg6, int muzzle);
extern uint32_t (*CWeapon__FireSniper)(CWeapon *pWeaponSlot, PED_TYPE *pFiringEntity, ENTITY_TYPE *a3, VECTOR *vecOrigin);

void CPlayerPed::FireInstant() {
	if(!m_pPed || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	uint8_t byteCameraMode;
	if(m_bytePlayerNumber != 0) {
		byteCameraMode = *pbyteCameraMode;
		*pbyteCameraMode = GameGetPlayerCameraMode(m_bytePlayerNumber);

		// wCameraMode2
		GameStoreLocalPlayerCameraExtZoom();
		GameSetRemotePlayerCameraExtZoom(m_bytePlayerNumber);

		GameStoreLocalPlayerAim();
		GameSetRemotePlayerAim(m_bytePlayerNumber);
	}

	g_pCurrentFiredPed = this;

	if(m_bHaveBulletData) {
		g_pCurrentBulletData = &m_bulletData;
	} else {
		g_pCurrentBulletData = nullptr;
	}

	CWeapon *pSlot = GetCurrentWeaponSlot();
	if(pSlot) {
		if(GetCurrentWeapon() == WEAPON_SNIPER)
		{
			if(m_pPed)
				CWeapon__FireSniper(pSlot, m_pPed, nullptr, nullptr);
			else
				CWeapon__FireSniper(nullptr, m_pPed, nullptr, nullptr);
		}
		else
		{
			VECTOR vecBonePos;
			VECTOR vecOut;

			GetWeaponInfoForFire(true, &vecBonePos, &vecOut);

			if(m_pPed)
				CWeapon__FireInstantHit(pSlot, m_pPed, &vecBonePos, &vecOut, nullptr, nullptr, nullptr, 0, 1);
			else
				CWeapon__FireInstantHit(nullptr, m_pPed, &vecBonePos, &vecOut, nullptr, nullptr, nullptr, 0, 1);
		}
	}

	g_pCurrentFiredPed = nullptr;
	g_pCurrentBulletData = nullptr;

	if(m_bytePlayerNumber != 0) {
		*pbyteCameraMode = byteCameraMode;

		// wCamera2
		GameSetLocalPlayerCameraExtZoom();
		GameSetLocalPlayerAim();
	}
}

void CPlayerPed::ResetDamageEntity()
{
	Log("ResetDamageEntity");
	m_pPed->pdwDamageEntity = 0;
}

// 0.3.7
void CPlayerPed::RestartIfWastedAt(VECTOR *vecRestart, float fRotation)
{	
	ScriptCommand(&restart_if_wasted_at, vecRestart->X, vecRestart->Y, vecRestart->Z, fRotation, 0);
}

// 0.3.7
void CPlayerPed::ForceTargetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->m_fCurrentRotation = DegToRad(fRotation);
	m_pPed->m_fAimingRotation = DegToRad(fRotation);

	ScriptCommand(&set_actor_z_angle,m_dwGTAId,fRotation);
}

void CPlayerPed::SetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->m_fCurrentRotation = DegToRad(fRotation);
	m_pPed->m_fAimingRotation = DegToRad(fRotation);
}

// 0.3.7
uint8_t CPlayerPed::GetActionTrigger()
{
	return (uint8_t)m_pPed->m_nPedState;
}

void CPlayerPed::SetActionTrigger(ePedState action)
{
	m_pPed->m_nPedState = action;
}

int GetInternalBoneIDFromSampID(int sampid);

void CPlayerPed::AttachObject(ATTACHED_OBJECT_INFO* pInfo, int iSlot)
{
	if (m_aAttachedObjects[iSlot].bState)
	{
		DeattachObject(iSlot);
	}
	memcpy((void*)& m_aAttachedObjects[iSlot], (const void*)pInfo, sizeof(ATTACHED_OBJECT_INFO));
	RwMatrix matPos;
	GetMatrix(&matPos);
	VECTOR vecRot{ 0.0f, 0.0f, 0.0f };
	m_aAttachedObjects[iSlot].pObject = new CObject(pInfo->dwModelId, matPos.pos.X, matPos.pos.Y, matPos.pos.Z, vecRot, 200.0f);
	*(uint32_t*)((uintptr_t)m_aAttachedObjects[iSlot].pObject->m_pEntity + 28) &= 0xFFFFFFFE; // disable collision
	m_aAttachedObjects[iSlot].bState = true;

}

void CPlayerPed::SetAttachOffset(int iSlot, VECTOR pos, VECTOR rot)
{
	if (iSlot < 0 || iSlot >= MAX_ATTACHED_OBJECTS)
	{
		return;
	}
	m_aAttachedObjects[iSlot].vecOffset = pos;
	m_aAttachedObjects[iSlot].vecRotation = rot;
}

void CPlayerPed::DeattachObject(int iSlot)
{
	if (m_aAttachedObjects[iSlot].bState)
	{
		delete m_aAttachedObjects[iSlot].pObject;
	}
	m_aAttachedObjects[iSlot].bState = false;
}

bool CPlayerPed::IsHasAttach()
{
	for (int i = 0; i < MAX_ATTACHED_OBJECTS; i++)
	{
		if (m_aAttachedObjects[i].bState) return true;
	}
	return false;
}

bool CPlayerPed::IsValidAttach(int iSlot)
{
	if(m_aAttachedObjects[iSlot].bState)return true;
	return false;
}
void CPlayerPed::FlushAttach()
{
	for (int i = 0; i < MAX_ATTACHED_OBJECTS; i++)
	{
		DeattachObject(i);
	}
}

RwMatrix* RwMatrixMultiplyByVector(VECTOR* out, RwMatrix* a2, VECTOR* in)
{
	RwMatrix* result;
	VECTOR* v4;

	result = a2;
	v4 = in;
	out->X = a2->at.X * in->Z + a2->up.X * in->Y + a2->right.X * in->X + a2->pos.X;
	out->Y = result->at.Y * v4->Z + result->up.Y * v4->Y + result->right.Y * v4->X + result->pos.Y;
	out->Z = result->at.Z * v4->Z + result->up.Z * v4->Y + a2->right.Z * in->X + result->pos.Z;
	return result;
}

void RwMatrixRotate(RwMatrix* pMat, VECTOR* axis, float angle)
{
	((int(*)(RwMatrix*, VECTOR*, float, int))(g_libGTASA + 0x001B9118 + 1))(pMat, axis, angle, 1);
}

void CPlayerPed::ProcessAttach()
{
	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(&m_pPed->entity)) {
		return;
	}
	if (m_pPed->entity.vtable == (g_libGTASA + 0x5C7358)) return;

	((int(*)(PED_TYPE*))(g_libGTASA + 0x00391968 + 1))(m_pPed); // UpdateRPHAnim

	if (IsAdded())
	{
		ProcessHeadMatrix();
	}
	for (int i = 0; i < MAX_ATTACHED_OBJECTS; i++)
	{
		if (!m_aAttachedObjects[i].bState) continue;
		CObject* pObject = m_aAttachedObjects[i].pObject;
		if (IsAdded())
		{
			RpHAnimHierarchy* hierarchy = ((RpHAnimHierarchy * (*)(uintptr_t*))(g_libGTASA + 0x00559338 + 1))((uintptr_t*)m_pPed->entity.m_pRwObject); // GetAnimHierarchyFromSkinClump
			int iID;
			uint32_t bone = m_aAttachedObjects[i].dwBone;
			if (hierarchy)
			{
				iID = ((int(*)(RpHAnimHierarchy*, int))(g_libGTASA + 0x0019A448 + 1))(hierarchy, bone); // RpHAnimIDGetIndex
			}
			else
			{
				continue;
			}
			if (iID == -1)
			{
				continue;
			}
			((void (*)(ENTITY_TYPE*))(*(void**)(pObject->m_pEntity->vtable + 16)))(pObject->m_pEntity); // CPhysical::Remove

			RwMatrix outMat;
			memcpy(&outMat, &hierarchy->pMatrixArray[iID], sizeof(RwMatrix));

			VECTOR vecOut;
			RwMatrixMultiplyByVector(&vecOut, &outMat, &m_aAttachedObjects[i].vecOffset);

			outMat.pos.X = vecOut.X;
			outMat.pos.Y = vecOut.Y;
			outMat.pos.Z = vecOut.Z;

			VECTOR axis = { 1.0f, 0.0f, 0.0f };
			if (m_aAttachedObjects[i].vecRotation.X != 0.0f)
			{
				RwMatrixRotate(&outMat, &axis, m_aAttachedObjects[i].vecRotation.X);
			}
			axis = { 0.0f, 1.0f, 0.0f };
			if (m_aAttachedObjects[i].vecRotation.Y != 0.0f)
			{
				RwMatrixRotate(&outMat, &axis, m_aAttachedObjects[i].vecRotation.Y);
			}
			axis = { 0.0f, 0.0f, 1.0f };
			if (m_aAttachedObjects[i].vecRotation.Z != 0.0f)
			{
				RwMatrixRotate(&outMat, &axis, m_aAttachedObjects[i].vecRotation.Z);
			}

			RwMatrixScale(&outMat, &m_aAttachedObjects[i].vecScale);
			*(uint32_t*)((uintptr_t)pObject->m_pEntity + 28) &= 0xFFFFFFFE; // disable collision

			if (outMat.pos.X >= 10000.0f || outMat.pos.X <= -10000.0f ||
				outMat.pos.Y >= 10000.0f || outMat.pos.Y <= -10000.0f ||
				outMat.pos.Z >= 10000.0f || outMat.pos.Z <= -10000.0f)
			{
				continue;
			}

			pObject->SetMatrix(outMat); // copy to CMatrix
			if (pObject->m_pEntity->m_pRwObject)
			{
				if (pObject->m_pEntity->mat)
				{
					uintptr_t v8 = *(uintptr_t*)(pObject->m_pEntity->m_pRwObject + 4) + 16;
					if (v8)
					{
						((int(*)(RwMatrix*, uintptr_t))(g_libGTASA + 0x003E862C + 1))(pObject->m_pEntity->mat, v8); // CEntity::UpdateRwFrame
					}
				}
			}
			//Log("pos %f %f %f", outMat.pos.X, outMat.pos.Y, outMat.pos.Z);
			((int(*)(ENTITY_TYPE*))(g_libGTASA + 0x0039194C + 1))(pObject->m_pEntity); // CEntity::UpdateRwFrame
			((void (*)(ENTITY_TYPE*))(*(void**)(pObject->m_pEntity->vtable + 8)))(pObject->m_pEntity); // CPhysical::Add
		}
		else
		{
			pObject->TeleportTo(0.0f, 0.0f, 0.0f);
		}
	}
}

void CPlayerPed::ProcessHeadMatrix()
{
	RpHAnimHierarchy* hierarchy = ((RpHAnimHierarchy * (*)(uintptr_t*))(g_libGTASA + 0x00559338 + 1))((uintptr_t*)m_pPed->entity.m_pRwObject); // GetAnimHierarchyFromSkinClump
	int iID;
	uint32_t bone = 4;
	if (hierarchy)
	{
		iID = ((int(*)(RpHAnimHierarchy*, int))(g_libGTASA + 0x0019A448 + 1))(hierarchy, bone); // RpHAnimIDGetIndex
	}
	else
	{
		return;
	}
	if (iID == -1)
	{
		return;
	}

	memcpy(&m_HeadBoneMatrix, &hierarchy->pMatrixArray[iID], sizeof(RwMatrix));
}

bool IsTaskRunNamedOrSlideToCoord(void* pTask)
{
	
	uintptr_t dwVTable = *(uintptr_t*)(pTask);
	if (dwVTable == (g_libGTASA + 0x005CB910) || dwVTable == (g_libGTASA + 0x005C8408)) // CTaskSimpleSlideToCoord CTaskSimpleRunNamedAnim
	{
		return true;
	}
	return false;
}

void* GetSubTaskFromTask(void* pTask)
{
	
	uintptr_t pVTableTask = *((uintptr_t*)pTask);
	return ((void* (*)(void*))(*(void**)(pVTableTask + 12)))(pTask);
}

bool CPlayerPed::IsPlayingAnim(int idx)
{
	
	if (!m_pPed || !m_dwGTAId || (idx == 0) )
	{
		return 0;
	}
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return 0;
	}
	if (!m_pPed->entity.m_pRwObject)
	{
		return 0;
	}

	const char* pAnim = GetAnimByIdx(idx - 1);
	if (!pAnim)
	{
		return false;
	}
	const char* pNameAnim = strchr(pAnim, ':') + 1;

	uintptr_t blendAssoc = ((uintptr_t(*)(uintptr_t clump, const char* szName))(g_libGTASA + 0x00340594 + 1))
		(m_pPed->entity.m_pRwObject, pNameAnim);	// RpAnimBlendClumpGetAssociation

	if (blendAssoc)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CPlayerPed::GetCurrentAnimationIndex(float& blendData)
{
	
	blendData = 4.0f;

	if (!m_pPed || !m_dwGTAId)
	{
		return 0;
	}

	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return 0;
	}

	if (!m_pPed->entity.m_pRwObject)
	{
		return 0;
	}
	sizeof(PED_TYPE);
	CPedIntelligence* pIntelligence = m_pPed->pPedIntelligence;

	if (pIntelligence)
	{
		void* pTask = pIntelligence->m_TaskMgr.m_aPrimaryTasks[TASK_PRIMARY_PRIMARY];

		if (pTask)
		{
			while (!IsTaskRunNamedOrSlideToCoord(pTask))
			{
				pTask = GetSubTaskFromTask(pTask);
				if (!pTask)
				{
					return 0;
				}
			}

			const char* szName = (const char*)((uintptr_t)pTask + 13);
			const char* szGroupName = (const char*)((uintptr_t)pTask + 37);

			std::string szStr = std::string(szGroupName);
			szStr += ":";
			szStr += szName;

			int idx = GetAnimIdxByName(szStr.c_str());
			if (idx == -1)
			{
				return 0;
			}
			else
			{
				return idx + 1;
			}
		}
	}
	return 0;
}

void CPlayerPed::PlayAnimByIdx(int idx, float BlendData, bool loop, bool freeze, uint8_t time)
{
	
	if (!idx)
	{
		RwMatrix mat;
		GetMatrix(&mat);
		TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);
		return;
	}
	std::string szAnim;
	std::string szBlock;

	char pszAnim[40];
	char pszBlock[40];

	memset(&pszAnim[0], 0, 40);
	memset(&pszBlock[0], 0, 40);

	bool bTest = false;
	const char* pBegin = GetAnimByIdx(idx - 1);
	if (!pBegin)
	{
		return;
	}
	while (*pBegin)
	{
		if (*pBegin == ':')
		{
			pBegin++;
			bTest = true;
			continue;
		}
		if (!bTest)
		{
			szBlock += *pBegin;
		}
		if (bTest)
		{
			szAnim += *pBegin;
		}
		pBegin++;
	}

	strcpy(&pszAnim[0], szAnim.c_str());
	strcpy(&pszBlock[0], szBlock.c_str());
	ApplyAnimation(&pszAnim[0], &pszBlock[0], BlendData, loop, 1, 1, freeze, time);
}

bool IsBlendAssocGroupLoaded(int iGroup)
{
	

	uintptr_t* pBlendAssocGroup = *(uintptr_t * *)(g_libGTASA + 0x00890350); // CAnimManager::ms_aAnimAssocGroups
	uintptr_t blendAssoc = (uintptr_t)pBlendAssocGroup;
	blendAssoc += (iGroup * 20);
	pBlendAssocGroup = (uintptr_t*)blendAssoc;
	return *(pBlendAssocGroup) != NULL;
}

void CPlayerPed::SetMoveAnim(int iAnimGroup)
{
	Log("SetMoveAnim %d", iAnimGroup);
	if (iAnimGroup == ANIM_GROUP_DEFAULT)
	{
		auto pModel = reinterpret_cast<CPedModelInfo*>(CModelInfo::GetModelInfo(m_pEntity->nModelIndex));
		iAnimGroup = pModel->m_nAnimType;
	}

	// Find which anim block to load
	const char* strBlockName = nullptr;
	switch (iAnimGroup)
	{
	case 55:
	case 58:
	case 61:
	case 64:
	case 67:
		strBlockName = "fat";
		break;

	case 56:
	case 59:
	case 62:
	case 65:
	case 68:
		strBlockName = "muscular";
		break;

	case 138:
		strBlockName = "skate";
		break;

	default:
		strBlockName = "ped";
		break;
	}
	if (!strBlockName)
	{
		return;
	}
	if (!m_dwGTAId || !m_pPed)
	{
		return;
	}
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}
	if (!IsBlendAssocGroupLoaded(iAnimGroup))
	{
		Log("Animgrp %d not loaded", iAnimGroup);
		uintptr_t pAnimBlock = ((uintptr_t(*)(const char*))(g_libGTASA + 0x0033DB7C + 1))(strBlockName);
		if (!pAnimBlock)
		{
			return;
		}
		uint8_t bLoaded = *((uint8_t*)pAnimBlock + 16);
		if (!bLoaded)
		{
			uintptr_t animBlocks = (uintptr_t)(g_libGTASA + 0x0089035C);
			uintptr_t idx = (pAnimBlock - animBlocks) / 32;

			uintptr_t modelId = idx + 25575;
			Log("trying to load modelid %u", modelId);
			if (!pGame->IsModelLoaded(modelId))
			{
				pGame->RequestModel(modelId);
				pGame->LoadRequestedModels();
				int tries = 0;
				while (!pGame->IsModelLoaded(modelId) && tries <= 10)
				{
					usleep(10);
					tries++;
				}
			}
		}
		if (!IsBlendAssocGroupLoaded(iAnimGroup))
		{
			Log("not loaded %d", iAnimGroup);
			return;
		}
		Log("animgrp %d loaded", iAnimGroup);
	}

	m_pPed->m_motionAnimGroup = static_cast<AssocGroupId>(iAnimGroup);

	((void(*)(PED_TYPE* thiz))(g_libGTASA + 0x004544F4 + 1))(m_pPed); // ReApplyMoveAnims
}



// 0.3.7
bool CPlayerPed::IsDead()
{
	
	if(!m_pPed) return true;
	if(m_pPed->fHealth > 0.0f) return false;
	return true;
}

// 0.3.7
void CPlayerPed::ShowMarker(uint32_t iMarkerColorID)
{
	if(m_dwArrow) HideMarker();
	ScriptCommand(&create_arrow_above_actor, m_dwGTAId, &m_dwArrow);
	ScriptCommand(&set_marker_color, m_dwArrow, iMarkerColorID);
	ScriptCommand(&show_on_radar2, m_dwArrow, 2);
}

// 0.3.7
void CPlayerPed::HideMarker()
{
	if(m_dwArrow) ScriptCommand(&disable_marker, m_dwArrow);
	m_dwArrow = 0;
}

// 0.3.7

void CPlayerPed::ClearLook()
{
	CHook::CallFunction<bool>(g_libGTASA + 0x0043388C + 1, m_pPed);
}

void CPlayerPed::TurnBody()
{
	CHook::CallFunction<bool>(g_libGTASA + 0x43358C + 1, m_pPed);
}

void CPlayerPed::SetFightingStyle(int iStyle)
{
	
	if(!m_pPed || !m_dwGTAId) return;
	if (!GamePool_Ped_GetAt(m_dwGTAId))
	{
		return;
	}
	//CChatWindow::AddDebugMessage("set fighting style %d", iStyle);
	ScriptCommand( &set_fighting_style, m_dwGTAId, iStyle, 6 );
}

// 0.3.7

void CPlayerPed::ApplyAnimation(char* szAnimName, char* szAnimFile, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int uiTime)
{

    if (!m_pPed) return;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

    if (!strcasecmp(szAnimFile, "SEX")) return;

	if(!pGame->IsAnimationLoaded(szAnimFile)) {
		CGame::RequestAnimation(szAnimFile);

		ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
	//	usleep(100000);
	}

//	animFlagTime = (uint8_t)uiTime;
//	animFlagFreeze = bFreeze;
//	animFlagLoop = bLoop;

	ScriptCommand(&apply_animation, m_dwGTAId, szAnimName, szAnimFile, fDelta, bLoop, bLockX, bLockY, bFreeze, uiTime);
}

PLAYERID CPlayerPed::FindDeathResponsiblePlayer()
{
	CPlayerPool *pPlayerPool;
	CVehiclePool *pVehiclePool;
	PLAYERID PlayerIDWhoKilled = INVALID_PLAYER_ID;

	if(pNetGame)
	{
		pVehiclePool = pNetGame->GetVehiclePool();
		pPlayerPool = pNetGame->GetPlayerPool();
	}
	else
	{ // just leave if there's no netgame.
		return INVALID_PLAYER_ID;
	}

	if(m_pPed)
	{
		if(m_pPed->pdwDamageEntity)
		{
			PlayerIDWhoKilled = pPlayerPool->FindRemotePlayerIDFromGtaPtr((PED_TYPE *)m_pPed->pdwDamageEntity);
			if(PlayerIDWhoKilled != INVALID_PLAYER_ID)
			{
				// killed by another player with a weapon, this is all easy.
				return PlayerIDWhoKilled;
			}
			else
			{
				if(pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)m_pPed->pdwDamageEntity) != INVALID_VEHICLE_ID)
				{
					VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)m_pPed->pdwDamageEntity;
					PlayerIDWhoKilled = pPlayerPool->FindRemotePlayerIDFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);

					if(PlayerIDWhoKilled != INVALID_PLAYER_ID)
					{
						return PlayerIDWhoKilled;
					}
				}
			}
		}
	}
	return INVALID_PLAYER_ID;
}

// 0.3.7
void CPlayerPed::GetBonePosition(int iBoneID, VECTOR* vecOut)
{
	if(!m_pPed) return;
	if(m_pEntity->vtable == g_libGTASA+0x5C7358) return;

	(( void (*)(PED_TYPE*, VECTOR*, int, int))(g_libGTASA+0x436590+1))(m_pPed, vecOut, iBoneID, 0);
}

ENTITY_TYPE* CPlayerPed::GetEntityUnderPlayer()
{
	uintptr_t entity;
	VECTOR vecStart;
	VECTOR vecEnd;
	char buf[100];

	if(!m_pPed) return nullptr;
	if( m_pPed->bInVehicle || !GamePool_Ped_GetAt(m_dwGTAId))
		return 0;

	vecStart.X = m_pPed->entity.mat->pos.X;
	vecStart.Y = m_pPed->entity.mat->pos.Y;
	vecStart.Z = m_pPed->entity.mat->pos.Z - 0.25f;

	vecEnd.X = m_pPed->entity.mat->pos.X;
	vecEnd.Y = m_pPed->entity.mat->pos.Y;
	vecEnd.Z = vecStart.Z - 1.75f;

	LineOfSight(&vecStart, &vecEnd, (void*)buf, (uintptr_t)&entity,
		0, 1, 0, 1, 0, 0, 0, 0);

	return (ENTITY_TYPE*)entity;
}
void CPlayerPed::ClumpUpdateAnimations(float step, int flag)
{
	if (m_pPed)
	{
		uintptr_t pRwObj = m_pEntity->m_pRwObject;
		if (pRwObj)
		{
			((void (*)(uintptr_t, float, int))(g_libGTASA + 0x33D6E4 + 1))(pRwObj, step, flag);
		}
	}
}

uint8_t CPlayerPed::GetExtendedKeys()
{
	uint8_t result = 0;
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_YES])
	{
		LocalPlayerKeys.bKeys[ePadKeys::KEY_YES] = false;
		result = 1;
	}
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_NO])
	{
		LocalPlayerKeys.bKeys[ePadKeys::KEY_NO] = false;
		result = 2;
	}
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_CTRL_BACK])
	{
		LocalPlayerKeys.bKeys[ePadKeys::KEY_CTRL_BACK] = false;
		result = 3;
	}

	return result;
}

uint16_t CPlayerPed::GetKeys(uint16_t *lrAnalog, uint16_t *udAnalog)
{
	*lrAnalog = LocalPlayerKeys.wKeyLR;
	*udAnalog = LocalPlayerKeys.wKeyUD;
	uint16_t wRet = 0;

	// KEY_ANALOG_RIGHT
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_ANALOG_RIGHT]) wRet |= 1;
	wRet <<= 1;
	// KEY_ANALOG_LEFT
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_ANALOG_LEFT]) wRet |= 1;
	wRet <<= 1;
	// KEY_ANALOG_DOWN
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_ANALOG_DOWN]) wRet |= 1;
	wRet <<= 1;
	// KEY_ANALOG_UP
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_ANALOG_UP]) wRet |= 1;
	wRet <<= 1;
	// KEY_WALK
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_WALK]) wRet |= 1;
	wRet <<= 1;
	// KEY_SUBMISSION
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_SUBMISSION]) wRet |= 1;
	wRet <<= 1;
	// KEY_LOOK_LEFT
	if (LocalPlayerKeys.bKeys[ePadKeys::KEY_LOOK_LEFT]) wRet |= 1;
	wRet <<= 1;

	if (GetCameraMode() == 0x35)
	{
		LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = 1;
	}
	else
	{
		LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = 0;
	}

	// KEY_HANDBRAKE
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE]/*true*/) wRet |= 1;
	wRet <<= 1;
	// KEY_LOOK_RIGHT
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_LOOK_RIGHT]) wRet |= 1;
	wRet <<= 1;
	// KEY_JUMP
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP]) wRet |= 1;
	wRet <<= 1;
	// KEY_SECONDARY_ATTACK
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK]) wRet |= 1;
	wRet <<= 1;
	// KEY_SPRINT
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_SPRINT]) wRet |= 1;
	wRet <<= 1;
	// KEY_FIRE
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE]) wRet |= 1;
	wRet <<= 1;
	// KEY_CROUCH
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_CROUCH]) wRet |= 1;
	wRet <<= 1;
	// KEY_ACTION
	if(LocalPlayerKeys.bKeys[ePadKeys::KEY_ACTION]) wRet |= 1;

	memset(LocalPlayerKeys.bKeys, 0, ePadKeys::SIZE);

	return wRet;
}

CWeapon * CPlayerPed::GetCurrentWeaponSlot()
{
	if (m_pPed) 
	{
		return &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot];
	}
	return NULL;
}


void CPlayerPed::SetKeys(uint16_t wKeys, uint16_t lrAnalog, uint16_t udAnalog)
{
	PAD_KEYS *pad = &RemotePlayerKeys[m_bytePlayerNumber];

	// LEFT/RIGHT
	pad->wKeyLR = lrAnalog;
	// UP/DOWN
	pad->wKeyUD = udAnalog;

	// KEY_ACTION
	pad->bKeys[ePadKeys::KEY_ACTION] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_CROUCH
	pad->bKeys[ePadKeys::KEY_CROUCH] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_FIRE
	pad->bKeys[ePadKeys::KEY_FIRE] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_SPRINT
	pad->bKeys[ePadKeys::KEY_SPRINT] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_SECONDARY_ATTACK
	pad->bKeys[ePadKeys::KEY_SECONDARY_ATTACK] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_JUMP
	pad->bKeys[ePadKeys::KEY_JUMP] = (wKeys & 1);
	if(!pad->bKeys[ePadKeys::KEY_JUMP]) pad->bIgnoreJump = false;
	wKeys >>= 1;
	// KEY_LOOK_RIGHT
	pad->bKeys[ePadKeys::KEY_LOOK_RIGHT] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_HANDBRAKE
	pad->bKeys[ePadKeys::KEY_HANDBRAKE] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_LOOK_LEFT
	pad->bKeys[ePadKeys::KEY_LOOK_LEFT] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_SUBMISSION
	pad->bKeys[ePadKeys::KEY_SUBMISSION] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_WALK
	pad->bKeys[ePadKeys::KEY_WALK] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_ANALOG_UP
	pad->bKeys[ePadKeys::KEY_ANALOG_UP] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_ANALOG_DOWN
	pad->bKeys[ePadKeys::KEY_ANALOG_DOWN] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_ANALOG_LEFT
	pad->bKeys[ePadKeys::KEY_ANALOG_LEFT] = (wKeys & 1);
	wKeys >>= 1;
	// KEY_ANALOG_RIGHT
	pad->bKeys[ePadKeys::KEY_ANALOG_RIGHT] = (wKeys & 1);

	return;
}

void CPlayerPed::SetAimZ(float fAimZ)
{
	if (!m_pPed)
	{
		return;
	}
	*(float*)(*((uintptr_t*)m_pPed + 272) + 84) = fAimZ;
	//m_pPed + 272 - dwPlayerInfo
}

float CPlayerPed::GetAimZ()
{
	if (!m_pPed)
	{
		return 0.0f;
	}
	return *(float*)(*((uintptr_t*)m_pPed + 272) + 84);
}

void CPlayerPed::ProcessSpecialAction(BYTE byteSpecialAction) {

	if (byteSpecialAction == SPECIAL_ACTION_CARRY && !IsAnimationPlaying("CRRY_PRTIAL"))
	{
		ApplyAnimation("CRRY_PRTIAL", "CARRY", 4.1, 0, 0, 0, 1, 1);
		IsActionCarry = true;
	}
	if(IsActionCarry && byteSpecialAction != SPECIAL_ACTION_CARRY)
	{
		ApplyAnimation("crry_prtial", "CARRY", 4.00, false, false, false, false, 1);
		//ClearAnimations();
		//ApplyAnimation("CRRY_PRTIAL", "CARRY", 4.0, 0, 0, 0, 0, 0);
		IsActionCarry = false;
	}

    // pissing:start
    if(!m_iPissingState && byteSpecialAction == SPECIAL_ACTION_PISSING) {
        ApplyAnimation("PISS_LOOP", "PAULNMAC", 4.0f, 1, true, true, true, -1);

        //opcode_066a('PETROLCAN', lhActor0, 0.0, 0.58, -0.08, 0.0, 0.01, 0.0, 1, l000f);

        ScriptCommand(&attach_particle_to_actor2,"PETROLCAN",m_dwGTAId,
                      0.0f, 0.58f, -0.08f, 0.0f, 0.01f, 0.0f, 1, &m_dwPissParticlesHandle);

        ScriptCommand(&make_particle_visible,m_dwPissParticlesHandle);

        m_iPissingState = 1;
    }

    // pissing:stop
    if(m_iPissingState && byteSpecialAction != SPECIAL_ACTION_PISSING) {
        if(m_dwPissParticlesHandle) {
            ScriptCommand(&destroy_particle,m_dwPissParticlesHandle);
            m_dwPissParticlesHandle = 0;
        }
        ClearAnimations();
        m_iPissingState = 0;
    }
}

void CPlayerPed::ProcessBulletData(BULLET_DATA* btData)
{
	if (!m_pPed || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	BULLET_SYNC bulletSyncData;

	if (btData) {
		m_bHaveBulletData = true;
		m_bulletData.pEntity = btData->pEntity;
		m_bulletData.vecOrigin.X = btData->vecOrigin.X;
		m_bulletData.vecOrigin.Y = btData->vecOrigin.Y;
		m_bulletData.vecOrigin.Z = btData->vecOrigin.Z;

		m_bulletData.vecPos.X = btData->vecPos.X;
		m_bulletData.vecPos.Y = btData->vecPos.Y;
		m_bulletData.vecPos.Z = btData->vecPos.Z;

		m_bulletData.vecOffset.X = btData->vecOffset.X;
		m_bulletData.vecOffset.Y = btData->vecOffset.Y;
		m_bulletData.vecOffset.Z = btData->vecOffset.Z;

		uint8_t byteHitType = 0;
		unsigned short InstanceID = 0xFFFF;

		if (m_bytePlayerNumber == 0)
		{
			if (pNetGame)
			{
				CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
				if (pPlayerPool)
				{
					CPlayerPed* pPlayerPed = pPlayerPool->GetLocalPlayer()->GetPlayerPed();
					if (pPlayerPed)
					{
						memset(&bulletSyncData, 0, sizeof(BULLET_SYNC));
						if (pPlayerPed->GetCurrentWeapon() != WEAPON_SNIPER || btData->pEntity)
						{
							if (btData->pEntity)
							{
								CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
								CObjectPool* pObjectPool = pNetGame->GetObjectPool();

								uint16_t PlayerID;
								uint16_t VehicleID;
								uint16_t ObjectID;

								if (pVehiclePool && pObjectPool)
								{
									PlayerID = pPlayerPool->FindRemotePlayerIDFromGtaPtr((PED_TYPE*)btData->pEntity);
									if (PlayerID == INVALID_PLAYER_ID)
									{
										VehicleID = pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE*)btData->pEntity);
										if (VehicleID == INVALID_VEHICLE_ID)
										{
											ObjectID = pObjectPool->FindIDFromGtaPtr(btData->pEntity);
											if (ObjectID == INVALID_OBJECT_ID)
											{
												VECTOR vecOut;
												vecOut.X = 0.0f;
												vecOut.Y = 0.0f;
												vecOut.Z = 0.0f;

												if (btData->pEntity->mat)
												{
													ProjectMatrix(&vecOut, btData->pEntity->mat, &btData->vecOffset);
													btData->vecOffset.X = vecOut.X;
													btData->vecOffset.Y = vecOut.Y;
													btData->vecOffset.Z = vecOut.Z;
												}
												else
												{
													btData->vecOffset.X = btData->pEntity->mat->pos.X + btData->vecOffset.X;
													btData->vecOffset.Y = btData->pEntity->mat->pos.Y + btData->vecOffset.Y;
													btData->vecOffset.Z = btData->pEntity->mat->pos.Z + btData->vecOffset.Z;
												}
											}
											else
											{
												// object
												byteHitType = BULLET_HIT_TYPE_OBJECT;
												InstanceID = ObjectID;
											}
										}
										else
										{
											// vehicle
											byteHitType = BULLET_HIT_TYPE_VEHICLE;
											InstanceID = VehicleID;
										}
									}
									else
									{
										// player
										byteHitType = BULLET_HIT_TYPE_PLAYER;
										InstanceID = PlayerID;
									}
								}
							}

							bulletSyncData.vecOrigin.X = btData->vecOrigin.X;
							bulletSyncData.vecOrigin.Y = btData->vecOrigin.Y;
							bulletSyncData.vecOrigin.Z = btData->vecOrigin.Z;

							bulletSyncData.vecPos.X = btData->vecPos.X;
							bulletSyncData.vecPos.Y = btData->vecPos.Y;
							bulletSyncData.vecPos.Z = btData->vecPos.Z;

							bulletSyncData.vecOffset.X = btData->vecOffset.X;
							bulletSyncData.vecOffset.Y = btData->vecOffset.Y;
							bulletSyncData.vecOffset.Z = btData->vecOffset.Z;

							bulletSyncData.byteHitType = byteHitType;
							bulletSyncData.PlayerID = InstanceID;
							bulletSyncData.byteWeaponID = pPlayerPed->GetCurrentWeapon();

							RakNet::BitStream bsBullet;
							bsBullet.Write((char)ID_BULLET_SYNC);
							bsBullet.Write((char*)&bulletSyncData, sizeof(BULLET_SYNC));


							pNetGame->GetRakClient()->Send(&bsBullet, HIGH_PRIORITY, RELIABLE, 0);
						}
					}
				}
			}
		}
	}
	else
	{
		m_bHaveBulletData = false;
		memset(&m_bulletData, 0, sizeof(BULLET_DATA));
	}
}

