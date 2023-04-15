#pragma once

#include "entity.h"
#include "RGB.h"
#include "CCustomPlateManager.h"

enum E_CUSTOM_COMPONENTS
{
	ccBumperF = 0,
	ccBumperR,
	ccFenderF,
	ccFenderR,
	ccSpoiler,
	ccExhaust,
	ccRoof,
	ccTaillights,
	ccHeadlights,
	ccDiffuser,
	ccSplitter,
	ccExtra,
	ccMax
};

enum E_HANDLING_PARAMS
{
	hpMaxSpeed,
	hpAcceleration,
	hpGear,
	hpEngineInertion,
	hpMass,
	hpMassTurn,

	hpBrakeDeceleration,
	hpTractionMultiplier,
	hpTractionLoss,
	hpTractionBias,
	hpSuspensionLowerLimit,
	hpSuspensionBias,

	hpWheelSize,

	hpMax
};

#define EXTRA_COMPONENT_BOOT			10
#define EXTRA_COMPONENT_BONNET			11
#define EXTRA_COMPONENT_BUMP_REAR		12
#define EXTRA_COMPONENT_DEFAULT_DOOR 	13
#define EXTRA_COMPONENT_WHEEL			14
#define EXTRA_COMPONENT_BUMP_FRONT		15

#define MAX_REPLACED_TEXTURES	32
#define MAX_REPLACED_TEXTURE_NAME	32

struct SReplacedTexture
{
	char szOld[MAX_REPLACED_TEXTURE_NAME] {};
	RwTexture* pTexture {nullptr};
};

struct SCustomCarShadow
{
	RwTexture* pTexture {nullptr};
	uint8_t r {};
	uint8_t g {};
	uint8_t b {};
	uint8_t fSizeX {};
	uint8_t fSizeY {};
};

enum eTurnState
{
	TURN_OFF,
	TURN_LEFT,
	TURN_RIGHT,
	TURN_ALL
};

void* GetSuspensionLinesFromModel(int nModelIndex, int& numWheels);
uint8_t* GetCollisionDataFromModel(int nModelIndex);

class CVehicle : public CEntity
{
public:
	CVehicle(int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, bool bSiren = false);
	~CVehicle();

	void LinkToInterior(int iInterior);
	void SetColor(int iColor1, int iColor2);

	void SetHealth(float fHealth);
	float GetHealth();

	// 0.3.7
	bool IsOccupied();

	// 0.3.7
	void SetInvulnerable(bool bInv);
	// 0.3.7
	bool IsDriverLocalPlayer();

	void ProcessMarkers();

	void RemoveEveryoneFromVehicle();

	void SetDoorState(int iState);

	void UpdateDamageStatus(uint32_t dwPanelDamage, uint32_t dwDoorDamage, uint8_t byteLightDamage, uint8_t byteTireDamage);

	void AttachTrailer();
	void DetachTrailer();
	void SetTrailer(CVehicle* pTrailer);

	unsigned int GetVehicleSubtype();

	void SetEngineState(bool bEnable);
	void SetLightsState(bool iState);
	bool GetLightsState();
	void SetBootAndBonnetState(int iBoot, int iBonnet);

	void RemoveComponent(uint16_t uiComponent);

	void SetComponentVisible(uint8_t group, uint16_t components);
	void SetHandlingData(std::vector<SHandlingData>& vHandlingData);
	void ResetVehicleHandling();

	void ApplyVinyls(uint8_t bSlot1, uint8_t bSlot2);

	void ApplyTexture(const char* szTexture, const char* szNew);
	void ApplyTexture(const char* szTexture, RwTexture* pTexture);
	void RemoveTexture(const char* szOldTexture);
	bool IsRetextured(const char* szOldTexture);

	void SetHeadlightsColor(uint8_t r, uint8_t g, uint8_t b);
	void ProcessHeadlightsColor(uint8_t& r, uint8_t& g, uint8_t& b);
	void SetWheelAlignment(int iWheel, float angle);
	void SetWheelOffset(int iWheel, float offset);
	void SetWheelWidth(float fValue);
	void ProcessWheelsOffset();

	void SetCustomShadow(uint8_t r, uint8_t g, uint8_t b, float fSizeX, float fSizeY, const char* szTex);

private:
	void ProcessWheelOffset(RwFrame* pFrame, bool bLeft, float fValue, int iID);
	void SetComponentAngle(bool bUnk, int iID, float angle);

	void SetComponentVisibleInternal(const char* szComponent, bool bVisible);
	std::string GetComponentNameByIDs(uint8_t group, int subgroup);

	void CopyGlobalSuspensionLinesToPrivate();
public:
	CRGBA tonerColor{255, 255, 255, 0};
	CRGBA mainColor{};
    CRGBA wheelColor{0, 0, 0, 0};
	CRGBA secondColor{};
	CRGBA lightColor{255, 255, 255, 0};
	RwTexture* pPlateTexture = nullptr;

	VEHICLE_TYPE* 	m_pVehicle;

	// поворотники
	CObject*		m_pLeftFrontTurnLighter = nullptr;
	CObject*		m_pRightFrontTurnLighter = nullptr;
	CObject*		m_pLeftRearTurnLighter = nullptr;
	CObject*		m_pRightRearTurnLighter = nullptr;
	eTurnState 		m_iTurnState = TURN_OFF;
	bool 			m_bIsOnRightTurnLight = false;
	bool 			m_bIsOnLeftTurnLight = false;
	bool 			m_bIsOnAllTurnLight = false;

	// задний ход
	CObject*		m_pLeftReverseLight = nullptr;
	CObject*		m_pRightReverseLight = nullptr;

	bool 			m_bHasSiren = false;
	bool 			m_bIsSirenOn = false;
	bool 			m_bIsLocked = false;
	CVehicle* 		m_pTrailer = nullptr;
	uint32_t		m_dwMarkerID = 0;
	bool 			m_bIsInvulnerable = false;
	uint8_t			m_byteObjectiveVehicle = 0; // Is this a special objective vehicle? 0/1
	bool			m_bSpecialMarkerEnabled = false;

	tHandlingData* m_pCustomHandling = nullptr;

//	SReplacedTexture m_szReplacedTextures[MAX_REPLACED_TEXTURES];
//	bool m_bReplaceTextureStatus[MAX_REPLACED_TEXTURES];
//	bool m_bReplacedTexture;

	void* m_pSuspensionLines;
	bool bHasSuspensionLines;

	bool m_bWheelAlignmentX;
	float m_fWheelAlignmentX = 0.0f;

	bool m_bWheelAlignmentY;
	float m_fWheelAlignmentY = 0.0f;

	bool m_bWheelSize;
	float m_fWheelSize = 0.0f;

	bool m_bWheelWidth;
	float m_fWheelWidth = 0.0f;

	bool m_bWheelOffsetX;
	float m_fWheelOffsetX = 0.0f;

	bool m_bWheelOffsetY;
	float m_fWheelOffsetY = 0.0f;

	float m_fNewOffsetX = 0.0f;
	float m_fNewOffsetY = 0.0f;

	bool m_bWasWheelOffsetProcessedX;
	bool m_bWasWheelOffsetProcessedY;
	uint32_t m_uiLastProcessedWheelOffset;

	RwMatrix m_vInitialWheelMatrix[4];

	SCustomCarShadow m_Shadow;
	bool m_bShadow = false;
	//int bEngine;
	int fDoorState = 0;
	//int bLights;

	bool IsTrailer();

	bool m_bLightsOn = false;
	bool m_bEngineOn = false;

	// Damage status
	uint8_t			m_byteTyreStatus = 0;
	uint8_t			m_byteLightStatus = 0;
	uint32_t		m_dwDoorStatus = 0;
	uint32_t		m_dwPanelStatus = 0;

	bool HasDamageModel();

	void SetPanelStatus(uint8_t bPanel, uint8_t bPanelStatus);

	uint8_t GetPanelStatus(uint8_t bPanel);

	void SetDoorStatus(uint32_t dwDoorStatus, bool spawnFlyingComponen);

	void SetPanelStatus(uint32_t ulPanelStatus);

	void SetLightStatus(uint8_t bLight, uint8_t bLightStatus);

	uint8_t GetLightStatus(uint8_t bLight);

	void SetLightStatus(uint8_t ucStatus);

	void SetBikeWheelStatus(uint8_t bWheel, uint8_t bTireStatus);

	void SetWheelStatus(eWheelPosition bWheel, uint8_t bTireStatus);

	uint8_t GetBikeWheelStatus(uint8_t bWheel);

	void SetDoorStatus(eDoors bDoor, uint8_t bDoorStatus, bool spawnFlyingComponen);

	uint8_t GetDoorStatus(eDoors bDoor);

	void ProcessDamage();

	void
	GetDamageStatusEncoded(uint8_t *byteTyreFlags, uint8_t *byteLightFlags, uint32_t *dwDoorFlags,
						   uint32_t *dwPanelFlags);

	uint8_t GetWheelStatus(eWheelPosition bWheel);

	bool IsValidGameVehicle();

	void toggleRightTurnLight(bool toggle);

	VEHICLEID getSampId();

	void toggleLeftTurnLight(bool toggle);

	void toggleAllTurnLight(bool toggle);

	void toggleReverseLight(bool toggle);

	void setPlate(ePlateType type, char *szNumber, char *szRegion);
};

enum eVehicleOverrideLightsState {
	NO_CAR_LIGHT_OVERRIDE = 0,
	FORCE_CAR_LIGHTS_OFF = 1,
	FORCE_CAR_LIGHTS_ON = 2
};