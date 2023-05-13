#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "common.h"
#include "vehicle.h"

#include "..//CDebugInfo.h"
#include "util/patch.h"
#include "Enums/ePedState.h"
#include "game/Models/ModelInfo.h"
#include "game/RW/rphanim.h"
#include "RwHelper.h"
#include "World.h"
#include "game/Animation/AnimManager.h"

extern CGame* pGame;
extern CNetGame *pNetGame;
CPlayerPed* g_pCurrentFiredPed;
BULLET_DATA* g_pCurrentBulletData;

CPlayerPed::CPlayerPed()
{
	Log("CPlayerPed");
	m_dwGTAId = 1;
	m_pPed = (CPedGta*)GamePool_FindPlayerPed();
	m_pEntity = (CPhysical*)GamePool_FindPlayerPed();
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

//	CHook::NOP(g_libGTASA + 0x00434C02, 2);// cpedinteligence
//	CHook::NOP(g_libGTASA + 0x00434C0A, 2);// cpedinteligence

//	new CPedGta(PEDTYPE_PLAYER_NETWORK);
	ScriptCommand(&create_player, &iPlayerNum, fX, fY, fZ, &dwPlayerActorID);
	ScriptCommand(&create_actor_from_player, &iPlayerNum, &dwPlayerActorID);

	m_dwGTAId = dwPlayerActorID;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (CPhysical*)GamePool_Ped_GetAt(m_dwGTAId);

	m_bytePlayerNumber = bytePlayerNumber;

	SetPlayerPedPtrRecord(m_bytePlayerNumber, (uintptr_t)m_pPed);
	m_pPed->bDoesntDropWeaponsWhenDead = true;

	ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
	ScriptCommand(&set_actor_can_be_decapitated, m_dwGTAId, 0); // отрыв бошки
	m_pPed->bNeverEverTargetThisPed = true;

	//ScriptCommand(&set_char_never_targeted, m_dwGTAId, 1);

	ScriptCommand(&set_actor_money, m_dwGTAId, 0); // деньги падают при смерти

	SetModelIndex(iSkin);
	ForceTargetRotation(fRotation);
	RwMatrix mat;
	GetMatrix(&mat);
	mat.pos.x = fX;
	mat.pos.y = fY;
	mat.pos.z = fZ+ 0.15f;
	SetMatrix(mat);
	
	for (int i = 0; i < MAX_ATTACHED_OBJECTS; i++)
	{
		m_aAttachedObjects[i].bState = false;
	}

	memset(&RemotePlayerKeys[m_bytePlayerNumber], 0, sizeof(PAD_KEYS));
}

bool IsValidGamePed(CPedGta* pPed);

CPlayerPed::~CPlayerPed()
{
    Log("~CPlayerPed()");
	CDebugInfo::uiStreamedPeds--;

	memset(&RemotePlayerKeys[m_bytePlayerNumber], 0, sizeof(PAD_KEYS));

	SetPlayerPedPtrRecord(m_bytePlayerNumber, 0);

	if(!m_dwGTAId)return;
	if(!GamePool_Ped_GetAt(m_dwGTAId))return;

	if (m_pPed && IsValidGamePed(m_pPed) && m_pPed->vtable != (g_libGTASA + 0x5C7358))
	{
		FlushAttach();
		if (m_pPed->bInVehicle) {
			RemoveFromVehicleAndPutAt(100.0f, 100.0f, 20.0f);

		//	ClearAllTasks();
		}
		uintptr_t dwPedPtr = (uintptr_t)m_pPed;
		*(uint32_t*)(*(uintptr_t*)(dwPedPtr + 1088) + 76) = 0;
		// CPlayerPed::Destructor

		//(( void (*)(CPedGta*))(*(void**)(m_pPed->vtable+0x4)))(m_pPed);
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
			if (m_pPed->m_pIntelligence)
			{
				((int (*)(CPedIntelligence*, uint16_t))(g_libGTASA + 0x0044E0F4 + 1))(m_pPed->m_pIntelligence, 0);
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
	if (m_pPed->m_pIntelligence)
	{
		((int (*)(CPedIntelligence*))(g_libGTASA + 0x0044E164 + 1))(m_pPed->m_pIntelligence);
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
	m_pPed->SetPosn(mat.pos.x, mat.pos.y, mat.pos.z);
	m_pPed->m_fHealth = 0.0f;

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

		case WEAPON_NIGHTSTICK:
			return WEAPON_MODEL_NITESTICK;

		case WEAPON_KNIFE:
			return WEAPON_MODEL_KNIFE;

		case WEAPON_BASEBALLBAT:
			return WEAPON_MODEL_BAT;

		case WEAPON_SHOVEL:
			return WEAPON_MODEL_SHOVEL;

		case WEAPON_POOL_CUE:
			return WEAPON_MODEL_POOLSTICK;

		case WEAPON_KATANA:
			return WEAPON_MODEL_KATANA;

		case WEAPON_CHAINSAW:
			return WEAPON_MODEL_CHAINSAW;

		case WEAPON_DILDO1:
			return WEAPON_MODEL_DILDO;

		case WEAPON_DILDO2:
			return WEAPON_MODEL_DILDO2;

		case WEAPON_VIBE1:
			return WEAPON_MODEL_VIBRATOR;

		case WEAPON_VIBE2:
			return WEAPON_MODEL_VIBRATOR2;

		case WEAPON_FLOWERS:
			return WEAPON_MODEL_FLOWER;

		case WEAPON_CANE:
			return WEAPON_MODEL_CANE;

		case WEAPON_GRENADE:
			return WEAPON_MODEL_GRENADE;

		case WEAPON_TEARGAS:
			return WEAPON_MODEL_TEARGAS;

		case WEAPON_MOLOTOV:
			return -1;

		case WEAPON_PISTOL:
			return WEAPON_MODEL_COLT45;

		case WEAPON_PISTOL_SILENCED:
			return WEAPON_MODEL_SILENCED;

		case WEAPON_DESERT_EAGLE:
			return WEAPON_MODEL_DEAGLE;

		case WEAPON_SHOTGUN:
			return WEAPON_MODEL_SHOTGUN;

		case WEAPON_SAWNOFF_SHOTGUN:
			return WEAPON_MODEL_SAWEDOFF;

		case WEAPON_SPAS12_SHOTGUN:
			return WEAPON_MODEL_SHOTGSPA;

		case WEAPON_MICRO_UZI:
			return WEAPON_MODEL_UZI;

		case WEAPON_MP5:
			return WEAPON_MODEL_MP5;

		case WEAPON_AK47:
			return WEAPON_MODEL_AK47;

		case WEAPON_M4:
			return WEAPON_MODEL_M4;

		case WEAPON_TEC9:
			return WEAPON_MODEL_TEC9;

		case WEAPON_COUNTRYRIFLE:
			return WEAPON_MODEL_RIFLE;

		case WEAPON_SNIPERRIFLE:
			return WEAPON_MODEL_SNIPER;

		case WEAPON_RLAUNCHER:
			return WEAPON_MODEL_ROCKETLAUNCHER;

		case WEAPON_RLAUNCHER_HS:
			return WEAPON_MODEL_HEATSEEKER;

		case WEAPON_FLAMETHROWER:
			return WEAPON_MODEL_FLAMETHROWER;

		case WEAPON_MINIGUN:
			return WEAPON_MODEL_MINIGUN;

		case WEAPON_REMOTE_SATCHEL_CHARGE:
			return WEAPON_MODEL_SATCHEL;

		case WEAPON_DETONATOR:
			return WEAPON_MODEL_BOMB;

		case WEAPON_SPRAYCAN:
			return WEAPON_MODEL_SPRAYCAN;

		case WEAPON_EXTINGUISHER:
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
	
	if (!CStreaming::IsModelLoaded(iModelID))
	{
		CStreaming::RequestModel(iModelID);
		CStreaming::LoadAllRequestedModels(false);
		while (!CStreaming::IsModelLoaded(iModelID)) sleep(1);
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

void CPlayerPed::ApplyCommandTask(char* a2, int a4, int a5, int a6, CVector* a7, char a8, float a9, int a10, int a11, char a12)
{
	uint32_t dwPed = (uint32_t)m_pPed;
	if (!dwPed) return;
	// 00958484 - g_ikChainManager
	// 00463188 addr
	((int(*)(uintptr_t a1, char* a2, uint32_t a3, int a4, int a5, int a6, CVector* a7, char a8, float a9, int a10, int a11, char a12))(g_libGTASA + 0x00463188 + 1))
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
		CVehicleGta *pVehicle = (CVehicleGta *)m_pPed->pVehicle;

		if(	pVehicle->pDriver != m_pPed ||
               pVehicle->m_nModelIndex == TRAIN_PASSENGER ||
               pVehicle->m_nModelIndex == TRAIN_FREIGHT )
			return true;
	}

	return false;
}

// 0.3.7
CVehicleGta* CPlayerPed::GetGtaVehicle()
{
	return (CVehicleGta*)m_pPed->pVehicle;
}

// 0.3.7
void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}
	if (m_pPed->vtable == (g_libGTASA + 0x5C7358)) return;
	if(m_pPed && m_pPed->bInVehicle)
		ScriptCommand(&remove_actor_from_car_and_put_at, m_dwGTAId, fX, fY, fZ);
}

// 0.3.7
int CPlayerPed::SetInitialState()
{
	Log("CPlayerPed::SetInitialState()1");
	int a1 = (( int (*)(CPedGta*, bool))(g_libGTASA + 0x458D1C + 1))(m_pPed, 0);

	Log("CPlayerPed::SetInitialState()2");
	return a1;
}

// 0.3.7
void CPlayerPed::SetHealth(float fHealth)
{
	if(!m_pPed) return;
	if (IsValidGamePed(m_pPed))
	{
		m_pPed->m_fHealth = fHealth;
	}

}

// 0.3.7
float CPlayerPed::GetHealth()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->m_fHealth;
}

// 0.3.7
void CPlayerPed::SetArmour(float fArmour)
{
	if(!m_pPed) return;
	m_pPed->m_fArmour = fArmour;
}

float CPlayerPed::GetArmour()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->m_fArmour;
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
			ScriptCommand(&refresh_streaming_at, mat.pos.x, mat.pos.y);
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
	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}
	if (m_pPed->vtable == (g_libGTASA + 0x5C7358)) return;


    CVehicleGta *gtaVehicle = pVehicle->m_pVehicle;

    if(gtaVehicle->fHealth == 0.0f) return;

	Log("PutDirectlyInVehicle");

	if(iSeat == 0)
	{
		//if(pVehicle->m_pVehicle->pDriver) return;
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

	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}

	CVehicleGta* ThisVehicleType;
	if((ThisVehicleType = GamePool_Vehicle_GetAt(iVehicleID)) == 0) return;
	if (ThisVehicleType->fHealth == 0.0f) return;
	if (ThisVehicleType->vtable == g_libGTASA + 0x5C7358) return;

	if(bPassenger)
	{
		ScriptCommand(&send_actor_to_car_passenger,m_dwGTAId,iVehicleID, 3000, -1);
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

	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}

	//CVehicleGta* ThisVehicleType = 0;

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

	return pVehiclePool->FindIDFromGtaPtr((CVehicleGta *)m_pPed->pVehicle);
}
CVehicle* CPlayerPed::GetCurrentVehicle()
{
	if(!m_pPed) return nullptr;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	for (size_t i = 0; i < MAX_VEHICLES; i++) {
		if (pVehiclePool->GetSlotState(i)) {
			CVehicle *pVehicle = pVehiclePool->GetAt(i);
			if (pVehicle && pVehicle->IsAdded()) {
				if (pVehicle->m_pVehicle == (CVehicleGta *) m_pPed->pVehicle) {
					return pVehicle;
				}
			}
		}
	}
	return nullptr;
}

CVehicleGta* CPlayerPed::GetCurrentGtaVehicle()
{
	if(!m_pPed) return nullptr;

	return (CVehicleGta *)m_pPed->pVehicle;
}

uint32_t CPlayerPed::GetCurrentGTAVehicleID(){
	if(!m_pPed) return 0;
	return GamePool_Vehicle_GetIndex(reinterpret_cast<CVehicleGta *>(m_pPed->pVehicle));
}

int CPlayerPed::GetVehicleSeatID()
{
	auto *pVehicle = (CVehicleGta *)m_pPed->pVehicle;

	if( pVehicle->pDriver == m_pPed) return 0;

	if(pVehicle->m_apPassengers[0] == m_pPed) return 1;
	if(pVehicle->m_apPassengers[1] == m_pPed) return 2;
	if(pVehicle->m_apPassengers[2] == m_pPed) return 3;
	if(pVehicle->m_apPassengers[3] == m_pPed) return 4;
	if(pVehicle->m_apPassengers[4] == m_pPed) return 5;
	if(pVehicle->m_apPassengers[5] == m_pPed) return 6;
	if(pVehicle->m_apPassengers[6] == m_pPed) return 7;

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

	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}
	if (m_pPed->vtable == (g_libGTASA + 0x5C7358)) return;

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
	m_pPed->ResetMoveSpeed();
	m_pPed->m_matrix->UpdateRW();
	Log("ClearAnimations");
}

void CPlayerPed::DestroyFollowPedTask()
{

}

int Weapon_FireSniper(CWeapon* pWeaponSlot, CPedGta* pPed)
{
	return ((int (*)(CWeapon*, CPedGta*))(g_libGTASA + 0x0056668C + 1))(pWeaponSlot, pPed);
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

CVector* CPlayerPed::GetCurrentWeaponFireOffset() {
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return nullptr;
	}

	CWeapon* pSlot = GetCurrentWeaponSlot();
	if (pSlot) {
		return (CVector*)(GetWeaponInfo(pSlot->m_nType, 1) + 0x24);
	}
	return nullptr;
}

void CPlayerPed::GetWeaponInfoForFire(int bLeft, CVector* vecBone, CVector* vecOut) {
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}

	CVector* pFireOffset = GetCurrentWeaponFireOffset();
	if (pFireOffset && vecBone && vecOut) {
		vecOut->x = pFireOffset->x;
		vecOut->y = pFireOffset->y;
		vecOut->z = pFireOffset->z;

		int bone_id = 24;
		if (bLeft) {
			bone_id = 34;
		}

		m_pPed->GetBonePosition(vecBone, bone_id, false);

		vecBone->z += pFireOffset->z + 0.15f;

		// CPed::GetTransformedBonePosition
		((void (*)(CPedGta*, CVector*, int, bool))(g_libGTASA + 0x004383C0 + 1))(m_pPed, vecOut, bone_id, false);
	}
}

extern uint32_t (*CWeapon__FireInstantHit)(CWeapon* thiz, CPedGta* pFiringEntity, CVector* vecOrigin, CVector* muzzlePosn, CEntityGta* targetEntity, CVector *target, CVector* originForDriveBy, int arg6, int muzzle);
extern uint32_t (*CWeapon__FireSniper)(CWeapon *pWeaponSlot, CPedGta *pFiringEntity, CEntityGta *a3, CVector *vecOrigin);

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
		if(GetCurrentWeapon() == WEAPON_SNIPERRIFLE)
		{
			if(m_pPed)
				CWeapon__FireSniper(pSlot, m_pPed, nullptr, nullptr);
			else
				CWeapon__FireSniper(nullptr, m_pPed, nullptr, nullptr);
		}
		else
		{
			CVector vecBonePos;
			CVector vecOut;

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
	m_pPed->m_pLastEntityDamage = nullptr;
}

// 0.3.7
void CPlayerPed::RestartIfWastedAt(const CVector vec, float fRotation)
{	
	ScriptCommand(&restart_if_wasted_at, vec.x, vec.y, vec.z, fRotation, 0);
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
	CVector vecRot{ 0.0f, 0.0f, 0.0f };
	m_aAttachedObjects[iSlot].pObject = new CObject(pInfo->dwModelId, matPos.pos.x, matPos.pos.y, matPos.pos.z, vecRot, 200.0f);
	*(uint32_t*)((uintptr_t)m_aAttachedObjects[iSlot].pObject->m_pEntity + 28) &= 0xFFFFFFFE; // disable collision
	m_aAttachedObjects[iSlot].bState = true;

}

void CPlayerPed::SetAttachOffset(int iSlot, CVector pos, CVector rot)
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

RwMatrix* RwMatrixMultiplyByVector(CVector* out, RwMatrix* a2, CVector* in)
{
	RwMatrix* result;
	CVector* v4;

	result = a2;
	v4 = in;
	out->x = a2->at.x * in->z + a2->up.x * in->y + a2->right.x * in->x + a2->pos.x;
	out->y = result->at.y * v4->z + result->up.y * v4->y + result->right.y * v4->x + result->pos.y;
	out->z = result->at.z * v4->z + result->up.z * v4->y + a2->right.z * in->x + result->pos.z;
	return result;
}

void RwMatrixRotate(RwMatrix* pMat, CVector* axis, float angle)
{
	((int(*)(RwMatrix*, CVector*, float, int))(g_libGTASA + 0x001B9118 + 1))(pMat, axis, angle, 1);
}

void CPlayerPed::ProcessAttach()
{
	if (!m_pPed) return;
	if(!m_dwGTAId)return;
	if (!IsValidGamePed(m_pPed) || !GamePool_Ped_GetAt(m_dwGTAId)) {
		return;
	}

	if (CUtil::IsGameEntityArePlaceable(m_pPed)) {
		return;
	}

	m_pPed->UpdateRpHAnim();

	if (IsAdded())
	{
		ProcessHeadMatrix();
	}
	for (auto & pAttach : m_aAttachedObjects)
	{
		if (!pAttach.bState) continue;
		CObject* pObject = pAttach.pObject;
		if (IsAdded())
		{
			auto hierarchy = GetAnimHierarchyFromSkinClump(m_pPed->m_pRwClump);

			int iID;
			if (hierarchy)
			{

				iID = RpHAnimIDGetIndex(hierarchy, pAttach.dwBone);
			}
			else
			{
				continue;
			}
			if (iID == -1)
			{
				continue;
			}
			((void (*)(CEntityGta*))(*(void**)(pObject->m_pEntity->vtable + 16)))(pObject->m_pEntity); // CPhysical::Remove

			RwMatrix outMat;
			memcpy(&outMat, &hierarchy->pMatrixArray[iID], sizeof(RwMatrix));

			CVector vecOut;
			RwMatrixMultiplyByVector(&vecOut, &outMat, &pAttach.vecOffset);

			outMat.pos = vecOut;

			CVector axis { 1.0f, 0.0f, 0.0f };
			if (pAttach.vecRotation.x != 0.0f)
			{
				RwMatrixRotate(&outMat, &axis, pAttach.vecRotation.x);
			}
			axis.Set( 0.0f, 1.0f, 0.0f );
			if (pAttach.vecRotation.y != 0.0f)
			{
				RwMatrixRotate(&outMat, &axis, pAttach.vecRotation.y);
			}
			axis.Set( 0.0f, 0.0f, 1.0f );
			if (pAttach.vecRotation.z != 0.0f)
			{
				RwMatrixRotate(&outMat, &axis, pAttach.vecRotation.z);
			}

			RwMatrixScale(&outMat, &pAttach.vecScale);
			*(uint32_t*)((uintptr_t)pObject->m_pEntity + 28) &= 0xFFFFFFFE; // disable collision

			if (outMat.pos.x >= 10000.0f || outMat.pos.x <= -10000.0f ||
				outMat.pos.y >= 10000.0f || outMat.pos.y <= -10000.0f ||
				outMat.pos.z >= 10000.0f || outMat.pos.z <= -10000.0f)
			{
				continue;
			}

			pObject->SetMatrix(outMat); // copy to CMatrix

			pObject->m_pEntity->UpdateRW();
			pObject->m_pEntity->UpdateRwFrame();

			((void (*)(CEntityGta*))(*(void**)(pObject->m_pEntity->vtable + 8)))(pObject->m_pEntity); // CPhysical::Add
		}
		else
		{
			pObject->m_pEntity->SetPosn(0.0f, 0.0f, 0.0f);
		}
	}
}

void CPlayerPed::ProcessHeadMatrix()
{
	auto hierarchy = GetAnimHierarchyFromSkinClump(m_pPed->m_pRwClump);

	if(!hierarchy)
		return;

	uint32_t bone = 4;
	int iID = RpHAnimIDGetIndex(hierarchy, bone);

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
	if (!m_pPed->m_pRwObject)
	{
		return 0;
	}

	const char* pAnim = GetAnimByIdx(idx - 1);
	if (!pAnim)
	{
		return false;
	}
	const char* pNameAnim = strchr(pAnim, ':') + 1;

	uintptr_t blendAssoc = ((uintptr_t(*)(RwObject* clump, const char* szName))(g_libGTASA + 0x00340594 + 1))
		(m_pPed->m_pRwObject, pNameAnim);	// RpAnimBlendClumpGetAssociation

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

	if (!m_pPed->m_pRwObject)
	{
		return 0;
	}
	sizeof(CPedGta);
	CPedIntelligence* pIntelligence = m_pPed->m_pIntelligence;

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
		m_pPed->SetPosn(mat.pos.x, mat.pos.y, mat.pos.z);
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
	auto pBlendAssocGroup = CAnimManager::ms_aAnimAssocGroups[iGroup].m_pAnimBlock;

	return pBlendAssocGroup != nullptr;
}

void CPlayerPed::SetMoveAnim(int iAnimGroup)
{
	Log("SetMoveAnim %d", iAnimGroup);
	if (iAnimGroup == ANIM_GROUP_DEFAULT || iAnimGroup == 255)
	{
		auto pModel = CModelInfo::GetModelInfo(m_pEntity->m_nModelIndex)->AsPedModelInfoPtr();
		iAnimGroup = pModel->m_nAnimType;
	}
	const char* strBlockName = nullptr;
	switch (iAnimGroup) {
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

	if (!m_dwGTAId || !m_pPed) return;

	if (!IsBlendAssocGroupLoaded(iAnimGroup))
	{
		Log("Animgrp %d not loaded", iAnimGroup);
		auto pAnimBlock = CAnimManager::GetAnimationBlock(strBlockName);

		if (!pAnimBlock) return;

		if (!pAnimBlock->bLoaded)
		{
			auto idx = CAnimManager::GetAnimationBlockIndex(pAnimBlock);

			auto modelId = IFPToModelId(idx);
			// nned 25680
			Log("trying to load modelid %d, %d", modelId, idx);
			if (!CStreaming::IsModelLoaded(modelId))
			{
				CStreaming::RequestModel(modelId);
				CStreaming::LoadAllRequestedModels(false);
				int tries = 0;
				while (!CStreaming::IsModelLoaded(modelId) && tries <= 10)
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

	m_pPed->m_nAnimGroup = static_cast<AssocGroupId>(iAnimGroup);

	auto pPlayerPed = (CPlayerPedGta*)m_pPed;
	pPlayerPed->ReApplyMoveAnims();
}



// 0.3.7
bool CPlayerPed::IsDead()
{
	
	if(!m_pPed) return true;
	if(m_pPed->m_fHealth > 0.0f) return false;
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
//
//CEntityGta* CPlayerPed::GetEntityUnderPlayer()
//{
//	uintptr_t entity;
//	CVector vecStart;
//	CVector vecEnd;
//	char buf[100];
//
//	if(!m_pPed) return nullptr;
//	if( m_pPed->bInVehicle || !GamePool_Ped_GetAt(m_dwGTAId))
//		return 0;
//
//	vecStart.x = m_pPed->m_matrix->m_pos.x;
//	vecStart.y = m_pPed->m_matrix->m_pos.y;
//	vecStart.z = m_pPed->m_matrix->m_pos.z - 0.25f;
//
//	vecEnd.x = m_pPed->m_matrix->m_pos.x;
//	vecEnd.y = m_pPed->m_matrix->m_pos.y;
//	vecEnd.z = vecStart.z - 1.75f;
//
//	LineOfSight(&vecStart, &vecEnd, (void*)buf, (uintptr_t)&entity,
//		0, 1, 0, 1, 0, 0, 0, 0);
//
//	return (CEntityGta*)entity;
//}

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
		return &m_pPed->m_aWeapons[m_pPed->m_nActiveWeaponSlot];
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

        ScriptCommand(&attach_particle_to_actor2,"PETROLCAN",m_dwGTAId,
                      0.0f, 0.58f, -0.08f, 0.0f, 0.01f, 0.0f, 1, &m_dwPissParticlesHandle);

        ScriptCommand(&make_particle_visible,m_dwPissParticlesHandle);

        m_iPissingState = true;
    }

    // pissing:stop
    if(m_iPissingState && byteSpecialAction != SPECIAL_ACTION_PISSING) {
        if(m_dwPissParticlesHandle) {
            ScriptCommand(&destroy_particle,m_dwPissParticlesHandle);
            m_dwPissParticlesHandle = 0;
        }
        ClearAnimations();
        m_iPissingState = false;
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
		m_bulletData.vecOrigin.x = btData->vecOrigin.x;
		m_bulletData.vecOrigin.y = btData->vecOrigin.y;
		m_bulletData.vecOrigin.z = btData->vecOrigin.z;

		m_bulletData.vecPos.x = btData->vecPos.x;
		m_bulletData.vecPos.y = btData->vecPos.y;
		m_bulletData.vecPos.z = btData->vecPos.z;

		m_bulletData.vecOffset.x = btData->vecOffset.x;
		m_bulletData.vecOffset.y = btData->vecOffset.y;
		m_bulletData.vecOffset.z = btData->vecOffset.z;

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
						if (pPlayerPed->GetCurrentWeapon() != WEAPON_SNIPERRIFLE || btData->pEntity)
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
									PlayerID = pPlayerPool->FindRemotePlayerIDFromGtaPtr((CPedGta*)btData->pEntity);
									if (PlayerID == INVALID_PLAYER_ID)
									{
										VehicleID = pVehiclePool->FindIDFromGtaPtr((CVehicleGta*)btData->pEntity);
										if (VehicleID == INVALID_VEHICLE_ID)
										{
											ObjectID = pObjectPool->FindIDFromGtaPtr(btData->pEntity);
											if (ObjectID == INVALID_OBJECT_ID)
											{
//												CVector vecOut;
//												vecOut.x = 0.0f;
//												vecOut.y = 0.0f;
//												vecOut.z = 0.0f;
//
//												if (btData->pEntity->m_matrix)
//												{
//													ProjectMatrix(&vecOut, btData->pEntity->m_matrix, &btData->vecOffset);
//													btData->vecOffset.x = vecOut.x;
//													btData->vecOffset.y = vecOut.y;
//													btData->vecOffset.z = vecOut.z;
//												}
//												else
//												{
//													btData->vecOffset.x = btData->pEntity->m_matrix->m_pos.x + btData->vecOffset.x;
//													btData->vecOffset.y = btData->pEntity->m_matrix->m_pos.y + btData->vecOffset.y;
//													btData->vecOffset.z = btData->pEntity->m_matrix->m_pos.z + btData->vecOffset.z;
//												}
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

							bulletSyncData.vecOrigin 	= btData->vecOrigin;
							bulletSyncData.vecPos 		= btData->vecPos;
							bulletSyncData.vecOffset 	= btData->vecOffset;

							bulletSyncData.byteHitType = static_cast<eBulletSyncHitType>(byteHitType);
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

