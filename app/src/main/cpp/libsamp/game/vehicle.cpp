#include "../main.h"
#include "game.h"

extern CGame* pGame;
#include "..//CDebugInfo.h"
#include "..//net/netgame.h"
#include "game/Core/Vector.h"
#include "game/Models/VehicleModelInfo.h"
#include "game/Models/ModelInfo.h"

extern CNetGame* pNetGame;

RwTexture* CVehicle::m_pVinyls[82]{};

CVehicle::CVehicle(int iType, float fPosX, float fPosY, float fPosZ, float fRotation, bool bSiren)
{
	Log("CVehicle(%d, %4.f, %4.f, %4.f, %4.f)", iType, fPosX, fPosY, fPosZ, fRotation);

	CDebugInfo::uiStreamedVehicles++;
	RwMatrix mat;

	m_pVehicle = nullptr;
	m_dwGTAId = 0;

	if ((iType != TRAIN_PASSENGER_LOCO) &&
		(iType != TRAIN_FREIGHT_LOCO) &&
		(iType != TRAIN_PASSENGER) &&
		(iType != TRAIN_FREIGHT) &&
		(iType != TRAIN_TRAM))
	{
		// normal vehicle
		if (!pGame->IsModelLoaded(iType))
		{
			pGame->RequestModel(iType);
			pGame->LoadRequestedModels();
			while (!pGame->IsModelLoaded(iType)) usleep(10);
		}
		ScriptCommand(&create_car, iType, fPosX, fPosY, fPosZ, &m_dwGTAId);
		ScriptCommand(&set_car_z_angle, m_dwGTAId, fRotation);
		ScriptCommand(&car_gas_tank_explosion, m_dwGTAId, 0);
		ScriptCommand(&set_car_hydraulics, m_dwGTAId, 0);
		ScriptCommand(&toggle_car_tires_vulnerable, m_dwGTAId, 1);
		ScriptCommand(&set_car_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
		m_pVehicle = (CVehicleGta*)GamePool_Vehicle_GetAt(m_dwGTAId);
		m_pEntity = (CPhysical*)m_pVehicle;

		if (m_pVehicle)
		{
			//m_pVehicle->m_nOverrideLights = eVehicleOverrideLightsState::NO_CAR_LIGHT_OVERRIDE;
			m_pVehicle->m_nDoorLock = CARLOCK_UNLOCKED;
			m_pVehicle->fHealth = 1000.0;
			m_bIsLocked = false;

			GetMatrix(&mat);
			mat.pos.x = fPosX;
			mat.pos.y = fPosY;
			mat.pos.z = fPosZ;

			if (GetVehicleSubtype() != VEHICLE_SUBTYPE_BIKE &&
				GetVehicleSubtype() != VEHICLE_SUBTYPE_PUSHBIKE)
				mat.pos.z += 0.25f;

			SetMatrix(mat);
		}
	}

	uint8_t defComp = 0;
	BIT_SET(defComp, 0);
	for (int i = 0; i < E_CUSTOM_COMPONENTS::ccMax; i++)
	{
		if (i == E_CUSTOM_COMPONENTS::ccExtra)
		{
			uint16_t defComp_extra = 0;
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BOOT);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BONNET);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_DEFAULT_DOOR);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_WHEEL);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BUMP_REAR);
			BIT_SET(defComp_extra, EXTRA_COMPONENT_BUMP_FRONT);
			SetComponentVisible(i, defComp_extra);
		}
		else
		{
			SetComponentVisible(i, (uint16_t)defComp);
		}
	}

	bHasSuspensionLines = false;
	m_pSuspensionLines = nullptr;
	if (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		CopyGlobalSuspensionLinesToPrivate();
	}

	m_bWheelAlignmentX = false;
	m_bWheelAlignmentY = false;

	m_fWheelOffsetX = 0.0f;
	m_fWheelOffsetY = 0.0f;
	m_fNewOffsetX = 0.0f;
	m_fNewOffsetY = 0.0f;
	m_bWasWheelOffsetProcessedX = true;
	m_bWasWheelOffsetProcessedY = true;
	m_uiLastProcessedWheelOffset = 0;

	auto pWheelLF = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_lf_dummy");
	auto pWheelRF = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_rf_dummy");
	auto pWheelRB = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_rb_dummy");
	auto pWheelLB = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_lb_dummy");

	if (pWheelLF && pWheelRF && pWheelRB && pWheelLB)
	{
		memcpy(&m_vInitialWheelMatrix[0], (const void*)&(pWheelLF->modelling), sizeof(RwMatrix));
		memcpy(&m_vInitialWheelMatrix[1], (const void*)&(pWheelRF->modelling), sizeof(RwMatrix));
		memcpy(&m_vInitialWheelMatrix[2], (const void*)&(pWheelRB->modelling), sizeof(RwMatrix));
		memcpy(&m_vInitialWheelMatrix[3], (const void*)&(pWheelLB->modelling), sizeof(RwMatrix));
	}
}



CVehicle::~CVehicle()
{
	if(!m_dwGTAId)return;

	CDebugInfo::uiStreamedVehicles--;
	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);

	if(!m_pVehicle)return;


	if(pPlateTexture) {
		RwTextureDestroy(pPlateTexture);
		pPlateTexture = nullptr;
	}

	if(IsTrailer()){
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle *tmpVeh = pVehiclePool->GetVehicleFromTrailer(this);
		if(tmpVeh)
		{
			ScriptCommand(&detach_trailer_from_cab, m_dwGTAId, tmpVeh->m_dwGTAId);
			tmpVeh->m_pTrailer = nullptr;
		}
	}

	if (m_dwMarkerID) {
		ScriptCommand(&disable_marker, m_dwMarkerID);
		m_dwMarkerID = 0;
	}
	RemoveEveryoneFromVehicle();

	if(m_pTrailer) {
		ScriptCommand(&detach_trailer_from_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
		m_pTrailer = nullptr;
	}

	if (m_pVehicle->m_nModelIndex == TRAIN_PASSENGER_LOCO ||
		m_pVehicle->m_nModelIndex == TRAIN_FREIGHT_LOCO) {
		ScriptCommand(&destroy_train, m_dwGTAId);
	}
	else {
		ScriptCommand(&destroy_car, m_dwGTAId);
	}

	if (m_pCustomHandling) {
		delete m_pCustomHandling;
		m_pCustomHandling = nullptr;
	}
	if (m_bShadow) {
		if (m_Shadow.pTexture) {
			RwTextureDestroy(m_Shadow.pTexture);
			m_Shadow.pTexture = nullptr;
		}
	}

	if (bHasSuspensionLines && m_pSuspensionLines) {
		delete[] m_pSuspensionLines;
		m_pSuspensionLines = nullptr;
		bHasSuspensionLines = false;
	}

	//
	if(m_pLeftFrontTurnLighter != nullptr)
	{
		delete m_pLeftFrontTurnLighter;
		m_pLeftFrontTurnLighter = nullptr;
	}
	if(m_pLeftRearTurnLighter != nullptr)
	{
		delete m_pLeftRearTurnLighter;
		m_pLeftRearTurnLighter = nullptr;
	}
	if(m_pRightFrontTurnLighter != nullptr)
	{
		delete m_pRightFrontTurnLighter;
		m_pRightFrontTurnLighter = nullptr;
	}
	if(m_pRightRearTurnLighter != nullptr)
	{
		delete m_pRightRearTurnLighter;
		m_pRightRearTurnLighter = nullptr;
	}

	//
	if(m_pLeftReverseLight != nullptr)
	{
		delete m_pLeftReverseLight;
		m_pLeftReverseLight = nullptr;
	}
	if(m_pRightReverseLight != nullptr)
	{
		delete m_pRightReverseLight;
		m_pRightReverseLight = nullptr;
	}

	if (!GetModelReferenceCount(m_pVehicle->m_nModelIndex) &&
		pGame->IsModelLoaded(m_pVehicle->m_nModelIndex))
	{
		CStreaming::RemoveModel(m_pVehicle->m_nModelIndex);
	}
}

void CVehicle::toggleRightTurnLight(bool toggle)
{
    m_bIsOnRightTurnLight = toggle;


	auto pModelInfoStart = CModelInfo::GetVehicleModelInfo(m_pEntity->m_nModelIndex);

	CVector* m_avDummyPos = pModelInfoStart->m_pVehicleStruct->m_avDummyPos;

	CVector vecFront;
	// 0 - front light
	vecFront.x = m_avDummyPos[0].x + 0.1;
	vecFront.y = m_avDummyPos[0].y;
	vecFront.z = m_avDummyPos[0].z;

	CVector vecRear;
	vecRear.x = m_avDummyPos[1].x + 0.1;
	vecRear.y = m_avDummyPos[1].y;
	vecRear.z = m_avDummyPos[1].z;

	CVector vec;
	vec.x = vec.y = vec.z = 0;

	if(m_pRightFrontTurnLighter != nullptr)
	{
		delete m_pRightFrontTurnLighter;
		m_pRightFrontTurnLighter = nullptr;
	}
	if(m_pRightRearTurnLighter != nullptr)
	{
		delete m_pRightRearTurnLighter;
		m_pRightRearTurnLighter = nullptr;
	}

	if(!toggle) return;

	m_pRightFrontTurnLighter = pGame->NewObject(19294, 0.0, 0.0, 0.0, vec, 300.0);
    m_pRightFrontTurnLighter->AttachToVehicle(getSampId(), &vecFront, &vecFront);

	m_pRightRearTurnLighter = pGame->NewObject(19294, 0.0, 0.0, 0.0, vec, 300.0);
	m_pRightRearTurnLighter->AttachToVehicle(getSampId(), &vecRear, &vecRear);

	m_pRightFrontTurnLighter->ProcessAttachToVehicle(this);
	m_pRightRearTurnLighter->ProcessAttachToVehicle(this);
}

void CVehicle::toggleReverseLight(bool toggle)
{
	auto pModelInfoStart = CModelInfo::GetVehicleModelInfo(m_pEntity->m_nModelIndex);

	CVector* m_avDummyPos = pModelInfoStart->m_pVehicleStruct->m_avDummyPos;

	CVector vecRight;
	vecRight.x = m_avDummyPos[1].x;
	vecRight.y = m_avDummyPos[1].y;
	vecRight.z = m_avDummyPos[1].z;

	CVector vecLeft;
	vecLeft.x = -m_avDummyPos[1].x;
	vecLeft.y = m_avDummyPos[1].y;
	vecLeft.z = m_avDummyPos[1].z;

	CVector vec;
	vec.x = vec.y = vec.z = 0;

	if(m_pLeftReverseLight != nullptr)
	{
		delete m_pLeftReverseLight;
		m_pLeftReverseLight = nullptr;
	}
	if(m_pRightReverseLight != nullptr)
	{
		delete m_pRightReverseLight;
		m_pRightReverseLight = nullptr;
	}

	if(!toggle) return;

	m_pLeftReverseLight = pGame->NewObject(19281, 0.0, 0.0, 0.0, vec, 300.0);
	m_pLeftReverseLight->AttachToVehicle(getSampId(), &vecLeft, &vecLeft);

	m_pRightReverseLight = pGame->NewObject(19281, 0.0, 0.0, 0.0, vec, 300.0);
	m_pRightReverseLight->AttachToVehicle(getSampId(), &vecRight, &vecRight);

	m_pRightReverseLight->ProcessAttachToVehicle(this);
	m_pLeftReverseLight->ProcessAttachToVehicle(this);
}

void CVehicle::toggleLeftTurnLight(bool toggle)
{
    m_bIsOnLeftTurnLight = toggle;

	auto pModelInfoStart = CModelInfo::GetVehicleModelInfo(m_pEntity->m_nModelIndex);

	CVector* m_avDummyPos = pModelInfoStart->m_pVehicleStruct->m_avDummyPos;

	CVector vecFront;
    // 0 - front light
    vecFront.x = -(m_avDummyPos[0].x + 0.1f);
    vecFront.y = m_avDummyPos[0].y;
    vecFront.z = m_avDummyPos[0].z;

	CVector vecRear;
    vecRear.x = -(m_avDummyPos[1].x + 0.1f);
    vecRear.y = m_avDummyPos[1].y;
    vecRear.z = m_avDummyPos[1].z;

	CVector vec;
    vec.x = vec.y = vec.z = 0;

    if(m_pLeftFrontTurnLighter != nullptr)
    {
        delete m_pLeftFrontTurnLighter;
        m_pLeftFrontTurnLighter = nullptr;
    }
    if(m_pLeftRearTurnLighter != nullptr)
    {
        delete m_pLeftRearTurnLighter;
        m_pLeftRearTurnLighter = nullptr;
    }

    if(!toggle) return;

    m_pLeftFrontTurnLighter = pGame->NewObject(19294, 0.0, 0.0, 0.0, vec, 300.0);
    m_pLeftFrontTurnLighter->AttachToVehicle(getSampId(), &vecFront, &vecFront);

    m_pLeftRearTurnLighter = pGame->NewObject(19294, 0.0, 0.0, 0.0, vec, 300.0);
    m_pLeftRearTurnLighter->AttachToVehicle(getSampId(), &vecRear, &vecRear);

    m_pLeftFrontTurnLighter->ProcessAttachToVehicle(this);
    m_pLeftRearTurnLighter->ProcessAttachToVehicle(this);
}

VEHICLEID CVehicle::getSampId()
{
	return pNetGame->GetVehiclePool()->FindIDFromGtaPtr(m_pVehicle);
}

void CVehicle::test()
{
	auto pEnt = m_pVehicle;

	Log("%x", m_pVehicle->m_pRemapTexture);
}

void CVehicle::LinkToInterior(int iInterior)
{
   // test();
	if (GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		ScriptCommand(&link_vehicle_to_interior, m_dwGTAId, iInterior);
	}
}

void CVehicle::SetColor(int iColor1, int iColor2)
{
	if (iColor1 >= 256 || iColor1 < 0)
	{
		iColor1 = 0;
	}
	if (iColor2 >= 256 || iColor2 < 0)
	{
		iColor2 = 0;
	}
	if (m_pVehicle)
	{
		if (GamePool_Vehicle_GetAt(m_dwGTAId))
		{
			m_pVehicle->m_nPrimaryColor = (uint8_t)iColor1;
			m_pVehicle->m_nSecondaryColor = (uint8_t)iColor2;
		}
	}
}

void CVehicle::AttachTrailer()
{
	if (m_pTrailer && GamePool_Vehicle_GetAt(m_pTrailer->m_dwGTAId) )
	{
		ScriptCommand(&put_trailer_on_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
	}
}

//-----------------------------------------------------------

void CVehicle::DetachTrailer()
{
	if (m_pTrailer && GamePool_Vehicle_GetAt(m_pTrailer->m_dwGTAId))
	{
		ScriptCommand(&detach_trailer_from_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
	}
	m_pTrailer = nullptr;
}

//-----------------------------------------------------------

void CVehicle::SetTrailer(CVehicle* pTrailer)
{
	m_pTrailer = pTrailer;
}

//-----------------------------------------------------------

void CVehicle::SetHealth(float fHealth)
{
	if (m_pVehicle)
	{
		m_pVehicle->fHealth = fHealth;
	}
}

float CVehicle::GetHealth()
{
	if (m_pVehicle) return m_pVehicle->fHealth;
	else return 0.0f;
}

// 0.3.7
void CVehicle::SetInvulnerable(bool bInv)
{
	if (!m_pVehicle) return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;
	if (m_pVehicle->vtable == g_libGTASA + 0x5C7358) return;

	if (bInv)
	{
		ScriptCommand(&set_car_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
		ScriptCommand(&toggle_car_tires_vulnerable, m_dwGTAId, 0);
		m_bIsInvulnerable = true;
	}
	else
	{
		ScriptCommand(&set_car_immunities, m_dwGTAId, 0, 0, 0, 0, 0);
		ScriptCommand(&toggle_car_tires_vulnerable, m_dwGTAId, 1);
		m_bIsInvulnerable = false;
	}
}

// 0.3.7
bool CVehicle::IsDriverLocalPlayer()
{
	if (m_pVehicle)
	{
		if ((CPedGta*)m_pVehicle->pDriver == GamePool_FindPlayerPed())
			return true;
	}

	return false;
}

bool IsValidGamePed(CPedGta* pPed);

void CVehicle::RemoveEveryoneFromVehicle()
{
	Log("RemoveEveryoneFromVehicle");
	if (!m_pVehicle) return;
	if(!m_dwGTAId)return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	float fPosX = m_pVehicle->m_matrix->m_pos.x;
	float fPosY = m_pVehicle->m_matrix->m_pos.y;
	float fPosZ = m_pVehicle->m_matrix->m_pos.z;

	int iPlayerID = 0;
	if (m_pVehicle->pDriver)
	{
		if(IsValidGamePed(m_pVehicle->pDriver))
		{
			iPlayerID = GamePool_Ped_GetIndex(m_pVehicle->pDriver);
			ScriptCommand(&remove_actor_from_car_and_put_at, iPlayerID, fPosX, fPosY, fPosZ + 2.0f);
		}

	}

	for (int i = 0; i < 7; i++)
	{
		if (m_pVehicle->pPassengers[i] != nullptr)
		{
			if(IsValidGamePed(m_pVehicle->pPassengers[i])) {
				iPlayerID = GamePool_Ped_GetIndex(m_pVehicle->pPassengers[i]);
				ScriptCommand(&remove_actor_from_car_and_put_at, iPlayerID, fPosX, fPosY,
							  fPosZ + 2.0f);
			}
		}
	}
}

// 0.3.7
bool CVehicle::IsOccupied()
{
	if (m_pVehicle)
	{
		if (m_pVehicle->pDriver) return true;
		if (m_pVehicle->pPassengers[0]) return true;
		if (m_pVehicle->pPassengers[1]) return true;
		if (m_pVehicle->pPassengers[2]) return true;
		if (m_pVehicle->pPassengers[3]) return true;
		if (m_pVehicle->pPassengers[4]) return true;
		if (m_pVehicle->pPassengers[5]) return true;
		if (m_pVehicle->pPassengers[6]) return true;
	}

	return false;
}

void CVehicle::ProcessMarkers()
{
	if (!m_pVehicle) return;

	if (m_byteObjectiveVehicle)
	{
		if (!m_bSpecialMarkerEnabled)
		{
			if (m_dwMarkerID)
			{
				ScriptCommand(&disable_marker, m_dwMarkerID);
				m_dwMarkerID = 0;
			}

			ScriptCommand(&tie_marker_to_car, m_dwGTAId, 1, 3, &m_dwMarkerID);
			ScriptCommand(&set_marker_color, m_dwMarkerID, 1006);
			ScriptCommand(&show_on_radar, m_dwMarkerID, 3);
			m_bSpecialMarkerEnabled = true;
		}

		return;
	}

	if (!m_byteObjectiveVehicle && m_bSpecialMarkerEnabled)
	{
		if (m_dwMarkerID)
		{
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_bSpecialMarkerEnabled = false;
			m_dwMarkerID = 0;
		}
	}

	if (GetDistanceFromLocalPlayerPed() < 200.0f && !IsOccupied())
	{
		if (!m_dwMarkerID)
		{
			// show
			ScriptCommand(&tie_marker_to_car, m_dwGTAId, 1, 2, &m_dwMarkerID);
			ScriptCommand(&set_marker_color, m_dwMarkerID, 1004);
		}
	}

	else if (IsOccupied() || GetDistanceFromLocalPlayerPed() >= 200.0f)
	{
		// remove
		if (m_dwMarkerID)
		{
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_dwMarkerID = 0;
		}
	}
}

void CVehicle::SetDoorState(int iState)
{
	if (!m_pVehicle) return;
	if (iState)
	{
		m_pVehicle->m_nDoorLock = CARLOCK_LOCKED;
		m_bIsLocked = true;
	}
	else
	{
		m_pVehicle->m_nDoorLock = CARLOCK_UNLOCKED;
		m_bIsLocked = false;
	}
}

void CVehicle::SetLightsState(bool iState)
{
	if(!m_dwGTAId)return;
	if(!m_pVehicle)return;

	//if (GamePool_Vehicle_GetAt(m_dwGTAId))
	//{


		m_pVehicle->m_nOverrideLights = 0;
		m_pVehicle->m_nVehicleFlags.bLightsOn = iState;
//		ScriptCommand(&FORCE_CAR_LIGHTS, m_dwGTAId, iState ? 2 : 1);
	//}
	m_bLightsOn = iState;
}

bool CVehicle::GetLightsState(){
	return m_bLightsOn;
}

void CVehicle::SetBootAndBonnetState(int iBoot, int iBonnet)
{
	if (GamePool_Vehicle_GetAt(m_dwGTAId) && m_pVehicle)
	{
		if (iBoot == 1)
		{
			SetComponentAngle(1, 17, 1.0f);
		}
		else
		{
			SetComponentAngle(1, 17, 0.0f);
		}

		if (iBonnet == 1)
		{
			SetComponentAngle(0, 16, 1.0f);
		}
		else
		{
			SetComponentAngle(0, 16, 0.0f);
		}
	}
}

void CVehicle::RemoveComponent(uint16_t uiComponent)
{

	int component = (uint16_t)uiComponent;

	if (!m_dwGTAId || !m_pVehicle)
	{
		return;
	}

	if (GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		ScriptCommand(&remove_component, m_dwGTAId, component);
	}
}

void CVehicle::SetComponentVisible(uint8_t group, uint16_t components)
{

	if (group == E_CUSTOM_COMPONENTS::ccExtra)
	{
		for (int i = 0; i < 16; i++)
		{
			std::string szName = GetComponentNameByIDs(group, i);
			SetComponentVisibleInternal(szName.c_str(), false);

			if (BIT_CHECK(components, i))
			{
				SetComponentVisibleInternal(szName.c_str(), true);
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			std::string szName = GetComponentNameByIDs(group, i);
			SetComponentVisibleInternal(szName.c_str(), false);
			if (BIT_CHECK(components, i))
			{
				SetComponentVisibleInternal(szName.c_str(), true);
			}
		}
	}
}

void* GetSuspensionLinesFromModel(int nModelIndex, int& numWheels)
{
	uint8_t* pCollisionData = GetCollisionDataFromModel(nModelIndex);

	if (!pCollisionData)
	{
		return nullptr;
	}

	void* pLines = *(void**)(pCollisionData + 16);

	numWheels = *(uint8_t*)(pCollisionData + 6);

	return pLines;
}

uint8_t* GetCollisionDataFromModel(int nModelIndex)
{
	auto dwModelarray = CModelInfo::ms_modelInfoPtrs;
	uint8_t* pModelInfoStart = (uint8_t*)dwModelarray[nModelIndex];

	if (!pModelInfoStart)
	{
		return nullptr;
	}

	uint8_t* pColModel = *(uint8_t * *)(pModelInfoStart + 44);

	if (!pColModel)
	{
		return nullptr;
	}

	uint8_t* pCollisionData = *(uint8_t * *)(pColModel + 44);

	return pCollisionData;
}
void CVehicle::SetHandlingData(std::vector<SHandlingData>& vHandlingData)
{
	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}
	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR && !IsTrailer())
	{
		return;
	}


	if (!m_pCustomHandling)
	{
		m_pCustomHandling = new tHandlingData;
	}

	auto pModel = CModelInfo::GetVehicleModelInfo(m_pVehicle->m_nModelIndex);

	if (!pModel)
	{
		return;
	}

	CHandlingDefault::GetDefaultHandling(pModel->m_nHandlingId, m_pCustomHandling);

	//bool bNeedRecalculate = false;

	for (auto& i : vHandlingData)
	{
		if(i.fValue == 0.0f && i.flag != E_HANDLING_PARAMS::hpWheelSize)
			continue;

		switch (i.flag)
		{
			case E_HANDLING_PARAMS::hpMaxSpeed:
				m_pCustomHandling->m_transmissionData.m_fMaxGearVelocity = i.fValue * 0.84;
				break;
			case E_HANDLING_PARAMS::hpAcceleration: {
				m_pCustomHandling->m_transmissionData.m_fEngineAcceleration =  i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpEngineInertion:
				m_pCustomHandling->m_transmissionData.m_fEngineInertia = i.fValue;
				break;
			case E_HANDLING_PARAMS::hpGear:

				if (i.fValue == 1)
				{
					m_pCustomHandling->m_transmissionData.m_nDriveType = 'R';
				}

				if (i.fValue == 2)
				{
					m_pCustomHandling->m_transmissionData.m_nDriveType = 'F';
				}

				if (i.fValue == 3)
				{
					m_pCustomHandling->m_transmissionData.m_nDriveType = '4';
				}

				break;
			case E_HANDLING_PARAMS::hpMass:
				m_pCustomHandling->m_fMass = i.fValue;
				break;
			case E_HANDLING_PARAMS::hpMassTurn:
				m_pCustomHandling->m_fTurnMass = i.fValue;
				break;
			case E_HANDLING_PARAMS::hpBrakeDeceleration:
			{
				m_pCustomHandling->m_fBrakeDeceleration = i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpTractionMultiplier:
			{
				m_pCustomHandling->m_fTractionMultiplier = i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpTractionLoss:
			{
				m_pCustomHandling->m_fTractionLoss = i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpTractionBias:
			{
				m_pCustomHandling->m_fTractionBias = i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpSuspensionLowerLimit:
			{
				m_pCustomHandling->m_fSuspensionLowerLimit = i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpSuspensionBias:
			{
				m_pCustomHandling->m_fSuspensionBiasBetweenFrontAndRear = i.fValue;
				break;
			}
			case E_HANDLING_PARAMS::hpWheelSize:
			{
				m_fWheelSize = i.fValue* 1.3f;
				break;
			}
		}

	}

	auto fDefaultFrontWheelSize = pModel->m_fWheelSizeFront;
	auto fDefaultRearWheelSize = pModel->m_fWheelSizeRear;

	if(m_fWheelSize != 0.0f) {
		pModel->m_fWheelSizeFront = m_fWheelSize;
		pModel->m_fWheelSizeRear = m_fWheelSize;
	} else {
		m_fWheelSize = pModel->m_fWheelSizeFront;
	}

	((void (*)(int, tHandlingData*))(g_libGTASA + 0x004FBCF4 + 1))(0, m_pCustomHandling);
	m_pVehicle->pHandling = m_pCustomHandling;

	((void (*)(CVehicleGta*))(g_libGTASA + 0x004D3E2C + 1))(m_pVehicle); // CAutomobile::SetupSuspensionLines

	CopyGlobalSuspensionLinesToPrivate();

	pModel->m_fWheelSizeFront = fDefaultFrontWheelSize;
	pModel->m_fWheelSizeRear = fDefaultRearWheelSize;

	//if (bNeedRecalculate)
	//{
	((void (*)(CVehicleGta*))(g_libGTASA + 0x004D6078 + 1))(m_pVehicle); // process suspension
	//}
}

void CVehicle::setPlate(ePlateType type, char* szNumber, char* szRegion)
{
	if(pPlateTexture) {
		RwTextureDestroy(pPlateTexture);
		pPlateTexture = nullptr;
	}

	pPlateTexture = CCustomPlateManager::createTexture(type, szNumber, szRegion);
}

RwObject* GetAllAtomicObjectCB(RwObject* object, void* data)
{

	std::vector<RwObject*>& result = *((std::vector<RwObject*>*) data);
	result.push_back(object);
	return object;
}

// Get all atomics for this frame (even if they are invisible)
void GetAllAtomicObjects(RwFrame* frame, std::vector<RwObject*>& result)
{

	((uintptr_t(*)(RwFrame*, void*, uintptr_t))(g_libGTASA + 0x001AEE2C + 1))(frame, (void*)GetAllAtomicObjectCB, (uintptr_t)& result);
}

void CVehicle::ProcessHeadlightsColor(uint8_t& r, uint8_t& g, uint8_t& b)
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	r = lightColor.r;
	g = lightColor.g;
	b = lightColor.b;
}

void CVehicle::SetWheelAlignment(int iWheel, float angle)
{
	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}

	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	if (iWheel == 0)
	{
		m_bWheelAlignmentX = true;
		m_fWheelAlignmentX = (M_PI / 180.0f) * angle;
	}
	else
	{
		m_bWheelAlignmentY = true;
		m_fWheelAlignmentY = (M_PI / 180.0f) * angle;
	}
}

void CVehicle::SetWheelOffset(int iWheel, float offset)
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	//CChatWindow::AddDebugMessage("set for %d wheel %f offset", iWheel, offset);
	if (iWheel == 0)
	{
		m_fWheelOffsetX = offset;
		m_bWasWheelOffsetProcessedX = false;
	}
	else
	{
		m_fWheelOffsetX = offset;
		m_bWasWheelOffsetProcessedY = false;
	}

	m_uiLastProcessedWheelOffset = GetTickCount();
}

void CVehicle::SetWheelWidth(float fValue)
{
	m_fWheelWidth = fValue;
}

RwMatrix* RwMatrixMultiplyByVector(CVector* out, RwMatrix* a2, CVector* in);

void CVehicle::ProcessWheelsOffset()
{
	if (GetTickCount() - m_uiLastProcessedWheelOffset <= 30)
	{
		return;
	}

	if (!m_bWasWheelOffsetProcessedX)
	{
		auto pWheelLF = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_lf_dummy");
		auto pWheelRF = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_rf_dummy");

		ProcessWheelOffset(pWheelLF, true, m_fWheelOffsetX, 0);
		ProcessWheelOffset(pWheelRF, false, m_fWheelOffsetX, 1);

		m_bWasWheelOffsetProcessedX = true;
	}
	if (!m_bWasWheelOffsetProcessedY)
	{
		auto pWheelRB = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_rb_dummy");
		auto pWheelLB = CClumpModelInfo::GetFrameFromName(m_pVehicle->m_pRwClump, "wheel_lb_dummy");

		ProcessWheelOffset(pWheelRB, false, m_fWheelOffsetY, 2);
		ProcessWheelOffset(pWheelLB, true, m_fWheelOffsetY, 3);
		
		m_bWasWheelOffsetProcessedY = true;
	}
}

void CVehicle::SetCustomShadow(uint8_t r, uint8_t g, uint8_t b, float fSizeX, float fSizeY, const char* szTex)
{
	if (m_Shadow.pTexture)
	{
		RwTextureDestroy(m_Shadow.pTexture);
		m_Shadow.pTexture = nullptr;
	}

	if (fSizeX == 0.0f || fSizeY == 0.0f)
	{
		m_bShadow = false;
		return;
	}

	m_bShadow = true;

	m_Shadow.r = r;
	m_Shadow.g = g;
	m_Shadow.b = b;
	m_Shadow.fSizeX = fSizeX;
	m_Shadow.fSizeY = fSizeY;
	m_Shadow.pTexture = CUtil::LoadTextureFromDB("samp", "neonaper3");
}

void CVehicle::ProcessWheelOffset(RwFrame* pFrame, bool bLeft, float fValue, int iID)
{
	CVector vecOffset;
	vecOffset.x = 0.0f - fValue;
	vecOffset.y = 0.0f;
	vecOffset.z = 0.0f;
	if (bLeft)
	{
		vecOffset.x *= -1.0f;
	}

	CVector vecOut;
	RwMatrixMultiplyByVector(&vecOut, &(m_vInitialWheelMatrix[iID]), &vecOffset);


	pFrame->modelling.pos = vecOut;
}

void CVehicle::SetComponentAngle(bool bUnk, int iID, float angle)
{
	if (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		((void(*)(CVehicleGta*, int a2, int a3, int a4, float a5, uint8_t a6))(g_libGTASA + 0x004DA0E4 + 1))(m_pVehicle, 0, iID, bUnk, angle, 1); // CAutomobile::OpenDoor
	}
}

void CVehicle::SetComponentVisibleInternal(const char* szComponent, bool bVisible)
{
	if (!m_pVehicle || !m_dwGTAId)
	{
		return;
	}

	if (!GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return;
	}

	if (!m_pVehicle->m_pRwObject)
	{
		return;
	}


	RwFrame* pFrame = ((RwFrame * (*)(RwObject*, const char*))(g_libGTASA + 0x00335CEC + 1))(m_pVehicle->m_pRwObject, szComponent); // GetFrameFromname
	if (pFrame != nullptr)
	{
		// Get all atomics for this component - Usually one, or two if there is a damaged version
		std::vector<RwObject*> atomicList;
		GetAllAtomicObjects(pFrame, atomicList);

		// Count number currently visible
		uint uiNumAtomicsCurrentlyVisible = 0;
		for (uint i = 0; i < atomicList.size(); i++)
		{
			if (!atomicList[i])
			{
				continue;
			}
			if (atomicList[i]->flags & 0x04)
			{
				uiNumAtomicsCurrentlyVisible++;
			}
		}

		if (bVisible && uiNumAtomicsCurrentlyVisible == 0)
		{
			// Make atomic (undamaged version) visible. TODO - Check if damaged version should be made visible instead
			for (uint i = 0; i < atomicList.size(); i++)
			{
				RwObject* pAtomic = atomicList[i];
				if (!pAtomic)
				{
					continue;
				}
				int       AtomicId = ((int(*)(RwObject*))(g_libGTASA + 0x0055C670 + 1))(pAtomic); // CVisibilityPlugins::GetAtomicId

				if (!(AtomicId & ATOMIC_ID_FLAG_TWO_VERSIONS_DAMAGED))
				{
					// Either only one version, or two versions and this is the undamaged one
					pAtomic->flags |= 0x04;
				}
			}
		}
		else if (!bVisible && uiNumAtomicsCurrentlyVisible > 0)
		{
			// Make all atomics invisible
			for (uint i = 0; i < atomicList.size(); i++)
			{
				if (!atomicList[i])
				{
					continue;
				}
				atomicList[i]->flags &= ~0x05;            // Mimic what GTA seems to do - Not sure what the bottom bit is for
			}
		}
	}
}

std::string CVehicle::GetComponentNameByIDs(uint8_t group, int subgroup)
{

	if (group == E_CUSTOM_COMPONENTS::ccExtra && subgroup >= EXTRA_COMPONENT_BOOT)
	{
		switch (subgroup)
		{
			case EXTRA_COMPONENT_BOOT:
				return {"boot_dummy"};
			case EXTRA_COMPONENT_BONNET:
				return {"bonnet_dummy"};
			case EXTRA_COMPONENT_BUMP_REAR:
				return {"bump_rear_dummy"};
			case EXTRA_COMPONENT_DEFAULT_DOOR:
				return {"door_lf_dummy"};
			case EXTRA_COMPONENT_WHEEL:
				return {"wheel_lf_dummy"};
			case EXTRA_COMPONENT_BUMP_FRONT:
				return {"bump_front_dummy"};
		}
	}

	std::string retn;

	switch (group)
	{
		case E_CUSTOM_COMPONENTS::ccBumperF:
			retn += "bumberF_";
			break;
		case E_CUSTOM_COMPONENTS::ccBumperR:
			retn += "bumberR_";
			break;
		case E_CUSTOM_COMPONENTS::ccFenderF:
			retn += "fenderF_";
			break;
		case E_CUSTOM_COMPONENTS::ccFenderR:
			retn += "fenderR_";
			break;
		case E_CUSTOM_COMPONENTS::ccSpoiler:
			retn += "spoiler_";
			break;
		case E_CUSTOM_COMPONENTS::ccExhaust:
			retn += "exhaust_";
			break;
		case E_CUSTOM_COMPONENTS::ccRoof:
			retn += "roof_";
			break;
		case E_CUSTOM_COMPONENTS::ccTaillights:
			retn += "taillights_";
			break;
		case E_CUSTOM_COMPONENTS::ccHeadlights:
			retn += "headlights_";
			break;
		case E_CUSTOM_COMPONENTS::ccDiffuser:
			retn += "diffuser_";
			break;
		case E_CUSTOM_COMPONENTS::ccSplitter:
			retn += "splitter_";
			break;
		case E_CUSTOM_COMPONENTS::ccExtra:
			retn += "ext_";
			break;
		default:
			retn = std::string("err");
			break;
	}

	retn += ('0' + (char)subgroup);

	return retn;
}

void CVehicle::CopyGlobalSuspensionLinesToPrivate()
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR)
	{
		return;
	}

	if (!bHasSuspensionLines)
	{
		int numWheels;
		void* pOrigSuspension = GetSuspensionLinesFromModel(m_pVehicle->m_nModelIndex, numWheels);

		if (pOrigSuspension && numWheels)
		{
			bHasSuspensionLines = true;
			m_pSuspensionLines = new uint8_t[0x20 * numWheels];
		}
	}

	int numWheels;
	void* pOrigSuspension = GetSuspensionLinesFromModel(m_pVehicle->m_nModelIndex, numWheels);

	if (pOrigSuspension && numWheels)
	{
		memcpy(m_pSuspensionLines, pOrigSuspension, 0x20 * numWheels);
	}
}

void CVehicle::SetEngineState(bool bEnable)
{
	if(!m_dwGTAId)return;
	if(!m_pVehicle)return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) {
		return;
	}
	//m_pVehicle->m_nVehicleFlags.bEngineBroken = 1;
	//m_bEngineOn = bEnable;
	m_pVehicle->m_nVehicleFlags.bEngineOn = m_bEngineOn = bEnable;;
}

bool CVehicle::HasDamageModel()
{
	if (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
		return true;
	return false;
}

uint8_t CVehicle::GetPanelStatus(uint8_t bPanel)
{
	if (m_pVehicle && bPanel < MAX_PANELS)
	{
		uintptr_t* pDamageManager = (uintptr_t*)((uintptr_t)m_pVehicle + 1456);
		return ((uint8_t(*)(uintptr_t, uint8_t))(g_libGTASA + 0x4F93D8 + 1))(((uintptr_t)m_pVehicle + 1456), bPanel);
	}
	return 0;
}

void CVehicle::SetPanelStatus(uint8_t bPanel, uint8_t bPanelStatus)
{
	if (m_pVehicle && bPanel < MAX_PANELS && bPanelStatus <= 3)
	{
		if (GetPanelStatus(bPanel) != bPanelStatus)
		{
			uintptr_t* pDamageManager = (uintptr_t*)((uintptr_t)m_pVehicle + 1456);
			((uint8_t(*)(uintptr_t, uint8_t, uint8_t))(g_libGTASA + 0x4F93B8 + 1))(((uintptr_t)m_pVehicle + 1456), bPanel, bPanelStatus);

			if (bPanelStatus == DT_PANEL_INTACT)
			{
				// Grab the car node index for the given panel
				static int s_iCarNodeIndexes[7] = { 0x0F, 0x0E, 0x00 /*?*/, 0x00 /*?*/, 0x12, 0x0C, 0x0D };
				int iCarNodeIndex = s_iCarNodeIndexes[bPanel];

				// CAutomobile::FixPanel
				((uint8_t(*)(uintptr_t, uint32_t, uint32_t))(g_libGTASA + 0x4DD238 + 1))((uintptr_t)m_pVehicle, iCarNodeIndex, static_cast<uint32_t>(bPanel));
			}
			else
			{
				((uint8_t(*)(uintptr_t, uint32_t, bool))(g_libGTASA + 0x4DB024 + 1))((uintptr_t)m_pVehicle, static_cast<uint32_t>(bPanel), false);
			}
		}
	}
}

uint8_t CVehicle::GetDoorStatus(eDoors bDoor)
{
	if (m_pVehicle && bDoor < MAX_DOORS)
	{
		DAMAGE_MANAGER_INTERFACE* pDamageManager = (DAMAGE_MANAGER_INTERFACE*)((uintptr_t)m_pVehicle + 1456);
		if (pDamageManager) return pDamageManager->Door[bDoor];
	}
	return 0;
}

void CVehicle::SetDoorStatus(eDoors bDoor, uint8_t bDoorStatus, bool spawnFlyingComponen)
{
	if (m_pVehicle && bDoor < MAX_DOORS)
	{
		if (GetDoorStatus(bDoor) != bDoorStatus)
		{
			uintptr_t* pDamageManager = (uintptr_t*)((uintptr_t)m_pVehicle + 1456);
			((uint8_t(*)(uintptr_t, uint8_t, uint8_t, bool))(g_libGTASA + 0x4F9410 + 1))(((uintptr_t)m_pVehicle + 1456), bDoor, bDoorStatus, spawnFlyingComponen);

			if (bDoorStatus == DT_DOOR_INTACT || bDoorStatus == DT_DOOR_SWINGING_FREE)
			{
				// Grab the car node index for the given door id
				static int s_iCarNodeIndexes[6] = { 0x10, 0x11, 0x0A, 0x08, 0x0B, 0x09 };
				int iCarNodeIndex = s_iCarNodeIndexes[bDoor];

				// CAutomobile::FixDoor
				((uint8_t(*)(uintptr_t, uint32_t, uint32_t))(g_libGTASA + 0x4DD13C + 1))((uintptr_t)m_pVehicle, iCarNodeIndex, static_cast<uint32_t>(bDoor));
			}
			else
			{
				bool bQuiet = !spawnFlyingComponen;
				((uint8_t(*)(uintptr_t, uint32_t, bool))(g_libGTASA + 0x4DB174 + 1))((uintptr_t)m_pVehicle, static_cast<uint32_t>(bDoor), bQuiet);
			}
		}
	}
}

void CVehicle::SetDoorStatus(uint32_t dwDoorStatus, bool spawnFlyingComponen)
{
	if (m_pVehicle)
	{
		for (uint8_t uiIndex = 0; uiIndex < MAX_DOORS; uiIndex++)
		{
			SetDoorStatus(static_cast<eDoors>(uiIndex), static_cast<uint8_t>(dwDoorStatus), spawnFlyingComponen);
			dwDoorStatus >>= 8;
		}
	}
}

void CVehicle::SetPanelStatus(uint32_t ulPanelStatus)
{
	if (m_pVehicle)
	{
		for (uint8_t uiIndex = 0; uiIndex < MAX_PANELS; uiIndex++)
		{
			SetPanelStatus(uiIndex, static_cast<uint8_t>(ulPanelStatus));
			ulPanelStatus >>= 4;
		}
	}
}

void CVehicle::SetLightStatus(uint8_t bLight, uint8_t bLightStatus)
{
	if (m_pVehicle && bLight < MAX_LIGHTS)
	{
		uintptr_t* pDamageManager = (uintptr_t*)((uintptr_t)m_pVehicle + 1456);
		((uint8_t(*)(uintptr_t, uint8_t, uint8_t))(g_libGTASA + 0x4F9380 + 1))(((uintptr_t)m_pVehicle + 1456), bLight, bLightStatus);
	}
}

void CVehicle::SetLightStatus(uint8_t ucStatus)
{
	if (m_pVehicle)
	{
		DAMAGE_MANAGER_INTERFACE* pDamageManager = (DAMAGE_MANAGER_INTERFACE*)((uintptr_t)m_pVehicle + 1456);
		if (pDamageManager) pDamageManager->Lights = static_cast<uint32_t>(ucStatus);
	}
}

uint8_t CVehicle::GetLightStatus(uint8_t bLight)
{
	if (m_pVehicle && bLight < MAX_LIGHTS)
	{
		uintptr_t* pDamageManager = (uintptr_t*)((uintptr_t)m_pVehicle + 1456);
		return ((uint8_t(*)(uintptr_t, uint8_t))(g_libGTASA + 0x4F93A0 + 1))(((uintptr_t)m_pVehicle + 1456), bLight);
	}
	return 0;
}

uint8_t CVehicle::GetWheelStatus(eWheelPosition bWheel)
{
	if (m_pVehicle && bWheel < MAX_WHEELS)
	{
		return ((uint8_t(*)(uintptr_t, uint8_t))(g_libGTASA + 0x4F9400 + 1))(((uintptr_t)m_pVehicle + 1456), bWheel);
	}
	return 0;
}

void CVehicle::SetWheelStatus(eWheelPosition bWheel, uint8_t bTireStatus)
{
	if (m_pVehicle && bWheel < MAX_WHEELS)
	{
		uintptr_t* pDamageManager = (uintptr_t*)((uintptr_t)m_pVehicle + 1456);
		((uint8_t(*)(uintptr_t, uint8_t, uint8_t))(g_libGTASA + 0x4F93F0 + 1))(((uintptr_t)m_pVehicle + 1456), bWheel, bTireStatus);
	}
}

void CVehicle::SetBikeWheelStatus(uint8_t bWheel, uint8_t bTireStatus)
{
	if (m_pVehicle && bWheel < 2)
	{
		if (bWheel == 0)
		{

			*(uint8_t*)((uintptr_t)m_pVehicle + 1644) = bTireStatus;
		}
		else
		{
			*(uint8_t*)((uintptr_t)m_pVehicle + 1645) = bTireStatus;
		}
	}
}

uint8_t CVehicle::GetBikeWheelStatus(uint8_t bWheel)
{
	if (m_pVehicle && bWheel < 2)
	{
		if (bWheel == 0)
		{
			return *(uint8_t*)((uintptr_t)m_pVehicle + 1644);
		}
		else
		{
			return *(uint8_t*)((uintptr_t)m_pVehicle + 1645);
		}
	}
	return 0;
}

void CVehicle::UpdateDamageStatus(uint32_t dwPanelDamage, uint32_t dwDoorDamage, uint8_t byteLightDamage, uint8_t byteTireDamage)
{
	if (HasDamageModel())
	{
		SetPanelStatus(dwPanelDamage);
		SetDoorStatus(dwDoorDamage, false);

		SetLightStatus(eLights::LEFT_HEADLIGHT, byteLightDamage & 1);
		SetLightStatus(eLights::RIGHT_HEADLIGHT, (byteLightDamage >> 2) & 1);
		if ((byteLightDamage >> 6) & 1)
		{
			SetLightStatus(eLights::LEFT_TAIL_LIGHT, 1);
			SetLightStatus(eLights::RIGHT_TAIL_LIGHT, 1);
		}

		SetWheelStatus(eWheelPosition::REAR_RIGHT_WHEEL, byteTireDamage & 1);
		SetWheelStatus(eWheelPosition::FRONT_RIGHT_WHEEL, (byteTireDamage >> 1) & 1);
		SetWheelStatus(eWheelPosition::REAR_LEFT_WHEEL, (byteTireDamage >> 2) & 1);
		SetWheelStatus(eWheelPosition::FRONT_LEFT_WHEEL, (byteTireDamage >> 3) & 1);
	}
	else if (GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE)
	{
		SetBikeWheelStatus(1, byteTireDamage & 1);
		SetBikeWheelStatus(0, (byteTireDamage >> 1) & 1);
	}
}

unsigned int CVehicle::GetVehicleSubtype()
{
	if (m_pVehicle)
	{
		if (m_pVehicle->vtable == g_libGTASA + 0x5CC9F0) // 0x871120
		{
			return VEHICLE_SUBTYPE_CAR;
		}
		else if (m_pVehicle->vtable == g_libGTASA + 0x5CCD48) // 0x8721A0
		{
			return VEHICLE_SUBTYPE_BOAT;
		}
		else if (m_pVehicle->vtable == g_libGTASA + 0x5CCB18) // 0x871360
		{
			return VEHICLE_SUBTYPE_BIKE;
		}
		else if (m_pVehicle->vtable == g_libGTASA + 0x5CD0B0) // 0x871948
		{
			return VEHICLE_SUBTYPE_PLANE;
		}
		else if (m_pVehicle->vtable == g_libGTASA + 0x5CCE60) // 0x871680
		{
			return VEHICLE_SUBTYPE_HELI;
		}
		else if (m_pVehicle->vtable == g_libGTASA + 0x5CCC30) // 0x871528
		{
			return VEHICLE_SUBTYPE_PUSHBIKE;
		}
		else if (m_pVehicle->vtable == g_libGTASA + 0x5CD428) // 0x872370
		{
			return VEHICLE_SUBTYPE_TRAIN;
		}
	}

	return 0;
}

bool CVehicle::IsTrailer()
{
	if(!m_pVehicle)return false;
	if(!m_pVehicle->m_nModelIndex)return false;

	return ((bool (*)(int)) (g_libGTASA + 0x00336940 + 1))(m_pVehicle->m_nModelIndex);
}

void CVehicle::GetDamageStatusEncoded(uint8_t* byteTyreFlags, uint8_t* byteLightFlags, uint32_t* dwDoorFlags, uint32_t* dwPanelFlags)
{
	if (byteTyreFlags) *byteTyreFlags = GetWheelStatus(eWheelPosition::REAR_RIGHT_WHEEL) | (GetWheelStatus(eWheelPosition::FRONT_RIGHT_WHEEL) << 1)
										| (GetWheelStatus(eWheelPosition::REAR_LEFT_WHEEL) << 2) | (GetWheelStatus(eWheelPosition::FRONT_LEFT_WHEEL) << 3);

	if (byteLightFlags) *byteLightFlags = GetLightStatus(eLights::LEFT_HEADLIGHT) | (GetLightStatus(eLights::RIGHT_HEADLIGHT) << 2);
	if (GetLightStatus(eLights::LEFT_TAIL_LIGHT) && GetLightStatus(eLights::RIGHT_TAIL_LIGHT))
		*byteLightFlags |= (1 << 6);

	if (dwDoorFlags) *dwDoorFlags = GetDoorStatus(eDoors::BONNET) | (GetDoorStatus(eDoors::BOOT) << 8) |
									(GetDoorStatus(eDoors::FRONT_LEFT_DOOR) << 16) | (GetDoorStatus(eDoors::FRONT_RIGHT_DOOR) << 24);

	if (dwPanelFlags) *dwPanelFlags = GetPanelStatus(ePanels::FRONT_LEFT_PANEL) | (GetPanelStatus(ePanels::FRONT_RIGHT_PANEL) << 4)
									  | (GetPanelStatus(ePanels::REAR_LEFT_PANEL) << 8) | (GetPanelStatus(ePanels::REAR_RIGHT_PANEL) << 12)
									  | (GetPanelStatus(ePanels::WINDSCREEN_PANEL) << 16) | (GetPanelStatus(ePanels::FRONT_BUMPER) << 20)
									  | (GetPanelStatus(ePanels::REAR_BUMPER) << 24);
}

void CVehicle::ProcessDamage()
{
	if (pNetGame)
	{
		VEHICLEID vehId = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(m_pVehicle);
		if (vehId != INVALID_VEHICLE_ID)
		{
			if (HasDamageModel())
			{
				uint8_t byteTyreFlags, byteLightFlags;
				uint32_t dwDoorFlags, dwPanelFlags;

				GetDamageStatusEncoded(&byteTyreFlags, &byteLightFlags, &dwDoorFlags, &dwPanelFlags);
				if (byteTyreFlags != m_byteTyreStatus || byteLightFlags != m_byteLightStatus ||
					dwDoorFlags != m_dwDoorStatus || dwPanelFlags != m_dwPanelStatus)
				{
					m_byteLightStatus = byteLightFlags;
					m_byteTyreStatus = byteTyreFlags;
					m_dwDoorStatus = dwDoorFlags;
					m_dwPanelStatus = dwPanelFlags;

					RakNet::BitStream bsDamage;

					bsDamage.Write(vehId);
					bsDamage.Write(dwPanelFlags);
					bsDamage.Write(dwDoorFlags);
					bsDamage.Write(byteLightFlags);
					bsDamage.Write(byteTyreFlags);

					pNetGame->GetRakClient()->RPC(&RPC_VehicleDamage, &bsDamage, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
				}
			}
			else if (GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE)
			{
				uint8_t byteTyreFlags = GetBikeWheelStatus(1) | (GetBikeWheelStatus(0) << 1);
				if (m_byteTyreStatus != byteTyreFlags)
				{
					m_byteTyreStatus = byteTyreFlags;

					RakNet::BitStream bsDamage;
					bsDamage.Write(vehId);
					bsDamage.Write((uint32_t)0);
					bsDamage.Write((uint32_t)0);
					bsDamage.Write((uint8_t)0);
					bsDamage.Write(byteTyreFlags);

					pNetGame->GetRakClient()->RPC(&RPC_VehicleDamage, &bsDamage, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
				}
			}
		}
	}
}