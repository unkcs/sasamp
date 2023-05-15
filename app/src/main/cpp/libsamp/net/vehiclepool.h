#pragma once

#include "../game/common.h"
#include "../game/vehicle.h"
#include "../game/playerped.h"

typedef struct _NEW_VEHICLE
{
	VEHICLEID 	VehicleID;
	int 		iVehicleType;
	CVector		vecPos;
	float 		fRotation;
	uint8_t		aColor1;
	uint8_t		aColor2;
	float		fHealth;
	uint8_t		byteInterior;
	uint32_t	dwDoorDamageStatus;
	uint32_t 	dwPanelDamageStatus;
	uint8_t		byteLightDamageStatus;
	uint8_t		byteTireDamageStatus;
	uint8_t		byteAddSiren;
	uint8_t		byteModSlots[14];
	uint8_t	  	bytePaintjob;
	int	cColor1;
	int	cColor2;
} NEW_VEHICLE;

class CVehiclePool
{
public:
	CVehiclePool();
	~CVehiclePool();

	void Process();

	bool New(NEW_VEHICLE* pNewVehicle);
	bool Delete(VEHICLEID VehicleID);

	CVehicle* GetAt(VEHICLEID VehicleID)
	{
		if(VehicleID >= MAX_VEHICLES || !m_bVehicleSlotState[VehicleID])
			return nullptr;
		return m_pVehicles[VehicleID];
	}

	bool GetSlotState(VEHICLEID VehicleID)
	{
		if(VehicleID >= MAX_VEHICLES)
			return false;
		return m_bVehicleSlotState[VehicleID];
	}

	VEHICLEID FindIDFromGtaPtr(CEntityGta * pGtaVehicle);
	VEHICLEID FindIDFromRwObject(RwObject* pRWObject);
	int FindGtaIDFromID(int ID);
	
	void AssignSpecialParamsToVehicle(VEHICLEID VehicleID, uint8_t byteObjective, uint8_t byteDoorsLocked);

	int FindNearestToLocalPlayerPed();

	void NotifyVehicleDeath(VEHICLEID VehicleID);

	bool			m_bIsActive[MAX_VEHICLES];
	CVehicle*		m_pVehicles[MAX_VEHICLES];

	CVehicle *GetVehicleFromTrailer(CVehicle *pTrailer);

	CVehicle *FindVehicle(CVehicleGta *pGtaVehicle);

private:
	bool			m_bVehicleSlotState[MAX_VEHICLES];
	bool			m_bIsWasted[MAX_VEHICLES];

};