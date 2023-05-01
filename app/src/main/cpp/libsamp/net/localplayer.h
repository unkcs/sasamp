#pragma once

#include "../game/common.h"
#include "../game/quaternion.h"
#include "../game/playerped.h"
#include "game/Enums/eWeaponType.h"

// spectate
#define SPECTATE_TYPE_NONE				0
#define SPECTATE_TYPE_PLAYER			1
#define SPECTATE_TYPE_VEHICLE			2

// special action's
#define SPECIAL_ACTION_NONE				0
#define SPECIAL_ACTION_USEJETPACK		2
#define SPECIAL_ACTION_DANCE1			5
#define SPECIAL_ACTION_DANCE2			6
#define SPECIAL_ACTION_DANCE3			7
#define SPECIAL_ACTION_DANCE4			8
#define SPECIAL_ACTION_HANDSUP			10
#define SPECIAL_ACTION_USECELLPHONE		11
#define SPECIAL_ACTION_SITTING			12
#define SPECIAL_ACTION_STOPUSECELLPHONE 13
#define SPECIAL_ACTION_NIGHTVISION		14
#define SPECIAL_ACTION_THERMALVISION	15
// added in 0.3
#define SPECIAL_ACTION_DUCK 			1
#define SPECIAL_ACTION_ENTER_VEHICLE 	3
#define SPECIAL_ACTION_EXIT_VEHICLE 	4
#define SPECIAL_ACTION_DRINK_BEER		20
#define SPECIAL_ACTION_SMOKE_CIGGY		21
#define SPECIAL_ACTION_DRINK_WINE		22
#define SPECIAL_ACTION_DRINK_SPRUNK		23
#define SPECIAL_ACTION_PISSING			68
// added in 0.3e
#define SPECIAL_ACTION_CUFFED			24
// added in 0.3x
#define SPECIAL_ACTION_CARRY			25

#define BULLET_HIT_TYPE_NONE            0
#define BULLET_HIT_TYPE_PLAYER          1
#define BULLET_HIT_TYPE_VEHICLE         2
#define BULLET_HIT_TYPE_OBJECT          3
#define BULLET_HIT_TYPE_PLAYER_OBJECT   4

#pragma pack(push, 1)
typedef struct _ONFOOT_SYNC_DATA
{
	uint16_t lrAnalog;				// +0
	uint16_t udAnalog;				// +2
	uint16_t wKeys;					// +4
	CVector vecPos;					// +6
	CQuaternion quat;				// +18
	uint8_t byteHealth;				// +34
	uint8_t byteArmour;				// +35
	uint8_t byteCurrentWeapon;		// +36
	uint8_t byteSpecialAction;		// +37
	CVector vecMoveSpeed;			// +38
	CVector vecSurfOffsets;			// +50
	uint16_t wSurfInfo;				// +62
	union {
		struct {
			uint16_t id;
			uint8_t  frameDelta;
			union {
				struct {
					bool    loop : 1;
					bool    lockX : 1;
					bool    lockY : 1;
					bool    freeze : 1;
					uint8_t time : 2;
					uint8_t _unused : 1;
					bool    regular : 1;
				};
				uint8_t value;
			} flags;
		} animation;
		struct {
			uint32_t  dwAnimation;
			//uint16_t  dwAnimationFlags;
		};
	};

} ONFOOT_SYNC_DATA;					// size = 68

typedef struct _TRAILER_SYNC_DATA
{
	VEHICLEID trailerID;
	CVector vecPos;
	CQuaternion quat;
	CVector vecMoveSpeed;
	CVector vecTurnSpeed;
} TRAILER_SYNC_DATA;

typedef struct _INCAR_SYNC_DATA
{
	VEHICLEID VehicleID;			// +0
	uint16_t lrAnalog;				// +2
	uint16_t udAnalog;				// +4
	uint16_t wKeys;					// +6
	CQuaternion quat;				// +8
	CVector vecPos;					// +24
	CVector vecMoveSpeed;			// +36
	float fCarHealth;				// +48
	uint8_t bytePlayerHealth;		// +52
	uint8_t bytePlayerArmour;		// +53
	uint8_t byteCurrentWeapon;		// +54
	uint8_t byteSirenOn;			// +55
	uint8_t byteLandingGearState;	// +56
	VEHICLEID TrailerID;			// +57

	union
	{
		uint32_t HydraThrustAngle;
		float fTrainSpeed;
	};

} INCAR_SYNC_DATA;					// size = 63

typedef struct _PASSENGER_SYNC_DATA
{
	VEHICLEID VehicleID;			// +0
	union {
		uint16_t DriveBySeatAdditionalKeyWeapon;
		struct
		{
			uint8_t byteSeatFlags : 2;
			uint8_t byteDriveBy : 6;
			uint8_t byteCurrentWeapon : 6;
			uint8_t AdditionalKey : 2;
		};
	};
	uint8_t bytePlayerHealth;		// +4
	uint8_t bytePlayerArmour;		// +5
	uint16_t lrAnalog;				// +6
	uint16_t udAnalog;				// +8
	uint16_t wKeys;					// +10
	CVector vecPos;					// +12
} PASSENGER_SYNC_DATA;				// size = 24

typedef struct _SPECTATOR_SYNC_DATA
{
	uint16_t lrAnalog;
	uint16_t udAnalog;
	uint16_t wKeys;
	CVector vecPos;
} SPECTATOR_SYNC_DATA;
#pragma pack(pop)

class CLocalPlayer
{
public:
	CLocalPlayer();
	~CLocalPlayer();

	void ResetAllSyncAttributes();

	bool Process();

	static void sendDeath();

	void GoEnterVehicle(bool passenger);
	uint32_t GetPlayerColorAsARGB();
	uint32_t GetPlayerColor();
	void SetPlayerColor(uint32_t dwColor);
	void UpdateSurfing();
	void SendEnterVehicleNotification(VEHICLEID VehicleID, bool bPassenger);
	void SendExitVehicleNotification(VEHICLEID VehicleID);
	void UpdateRemoteInterior(uint8_t byteInterior);
	bool Spawn(const CVector pos, float rot);
	int GetOptimumOnFootSendRate();
	uint8_t DetermineNumberOfPlayersInLocalRange();
	int GetOptimumInCarSendRate();
	void ProcessSpectating();
	void ToggleSpectating(bool bToggle);
	void SpectatePlayer(PLAYERID playerId);
	void SpectateVehicle(VEHICLEID VehicleID);
	bool IsSpectating() { return m_bIsSpectating; }

	CPlayerPed * GetPlayerPed() { return m_pPlayerPed; };

	bool IsInRCMode() { return m_bInRCMode; };

	void SendOnFootFullSyncData();
	void SendOnKeyFullSyncData();
	void SendInCarFullSyncData();
	void SendPassengerFullSyncData();
	void SendAimSyncData();
	void SendStatsUpdate();
	void CheckWeapons();

	CPlayerPed			*m_pPlayerPed;

	void SendBulletSyncData(PLAYERID byteHitID, uint8_t byteHitType, CVector vecHitPos);

public:
	bool				m_bIsSpectating;
	uint8_t				m_byteSpectateMode;
	uint8_t				m_byteSpectateType;
	uint32_t			m_SpectateID; // Vehicle or player id
	bool				m_bSpectateProcessed;

	VEHICLEID			m_CurrentVehicle;
	VEHICLEID 			m_LastVehicle;
	bool				m_bIsActive;
	PLAYERID 			lastDamageId;
	eWeaponType 		lastDamageWeap;

private:
	uint32_t			GetCurrentAnimationIndexFlag();

	bool				m_bIsWasted;
	uint8_t				m_byteCurInterior;
	bool				m_bInRCMode;

	uint32_t			m_dwPassengerEnterExit;

	ONFOOT_SYNC_DATA 	m_OnFootData;
	INCAR_SYNC_DATA 	m_InCarData;
	PASSENGER_SYNC_DATA m_PassengerData;

	uint32_t 			m_dwLastSendTick;
	uint32_t			m_dwLastSendSpecTick;
	uint32_t			m_dwLastAimSendTick;
	uint32_t			m_dwLastSendAimTick;
	uint32_t			m_dwLastHeadUpdate;
	uint32_t 			m_dwLastUpdateOnFootData;
	uint32_t			m_dwLastUpdateInCarData;
	uint32_t 			m_dwLastUpdatePassengerData;
	uint32_t			m_dwLastStatsUpdateTick;

	uint8_t				m_byteLastWeapon[MAX_WEAPONS_SLOT];
	uint32_t			m_dwLastAmmo[MAX_WEAPONS_SLOT];
	uint32_t 			m_dwLastUpdateHudButtons;
};