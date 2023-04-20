#pragma once
#include <string>
#include "game/RW/rwplcore.h"

#include "tHandlingData.h"


constexpr float E              = 2.71828f;          // e
constexpr float E_CONST        = 0.577f;            // Euler-Mascheroni constant
constexpr float FRAC_1_TAU     = 0.159154f;         // 1 / τ
constexpr float FRAC_1_PI      = 0.318309f;         // 1 / π
constexpr float FRAC_2_TAU     = 0.318309f;         // 2 / τ
constexpr float FRAC_2_PI      = 0.636619f;         // 2 / π
constexpr float FRAC_2_SQRT_PI = 1.12837f;          // 2 / √π
constexpr float FRAC_4_TAU     = 0.636619f;         // 4 / τ
constexpr float FRAC_1_SQRT_2  = 0.707106f;         // 1 / √2
constexpr float FRAC_PI_2      = 1.57079f;          // π / 2
constexpr float FRAC_PI_3      = 1.04719f;          // π / 3
constexpr float FRAC_PI_4      = 0.785398f;         // π / 4
constexpr float FRAC_PI_6      = 0.523598f;         // π / 6
constexpr float FRAC_PI_8      = 0.392699f;         // π / 8
constexpr float FRAC_TAU_2     = 3.14159f;          // τ / 2 = π
constexpr float FRAC_TAU_3     = 2.09439f;          // τ / 3
constexpr float FRAC_TAU_4     = 1.57079f;          // τ / 4
constexpr float FRAC_TAU_6     = 1.04719f;          // τ / 6
constexpr float FRAC_TAU_8     = 0.785398f;         // τ / 8
constexpr float FRAC_TAU_12    = 0.523598f;         // τ / 12
constexpr float LN_2           = 0.693147f;         // ln(2)
constexpr float LN_10          = 2.30258f;          // ln(10)
constexpr float LOG2_E         = 1.44269f;          // log2(e)
constexpr float LOG10_E        = 0.434294f;         // log10(e)
constexpr float LOG10_2        = 0.301029f;         // log10(2)
constexpr float LOG2_10        = 3.32192f;          // log2(10)
constexpr float PI             = 3.14159f;          // π
constexpr float HALF_PI        = PI / 2.0f;         // π / 2
constexpr float PI_6           = PI / 6.0f;         // π / 6
constexpr float SQRT_2         = 1.41421f;          // √2
constexpr float SQRT_3         = 1.73205f;          // √3
constexpr float TWO_PI         = 6.28318f;          // τ (TAU)

constexpr float COS_45 = SQRT_2; // cos(45deg)

constexpr float sq(float x) { return x * x; }

#define HUD_ELEMENT_MAP     1
#define HUD_ELEMENT_TAGS    2
#define HUD_ELEMENT_BUTTONS 3
#define HUD_ELEMENT_HUD     4
#define HUD_ELEMENT_VOICE	5
#define HUD_ELEMENT_TEXTLABELS	6
#define HUD_ELEMENT_FPS		7


#define HUD_MAX	8

#define PLAYER_PED_SLOTS	120
#define MAX_TEXT_DRAWS	3072
#define MAX_STREAMS		1000
#define MAX_MATERIALS		16
#define MAX_REMOVE_MODELS	1200
#define MAX_ATTACHED_OBJECTS	25
#define MAX_WEAPON_MODELS 47
#define MAX_WEAPONS_SLOT 13

typedef unsigned short VEHICLEID;
typedef unsigned short PLAYERID;

#define SAMP_MAJOR_VERSION	2
#define SAMP_MINOR_VERSION	0

#define PADDING(x,y) uint8_t x[y]

#define BIT_SET(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define BIT_CLEAR(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define BIT_CHECK(byte,nbit) ((byte) &   (1<<(nbit)))

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef uint8     bool8;
typedef uint16    bool16;
typedef uint32    bool32;


enum eLights :	uint8_t
{
	// these have to correspond to their respective panels
	LEFT_HEADLIGHT = 0,
	RIGHT_HEADLIGHT,
	LEFT_TAIL_LIGHT,
	RIGHT_TAIL_LIGHT,
	/*  LEFT_BRAKE_LIGHT,
		RIGHT_BRAKE_LIGHT,
		FRONT_LEFT_INDICATOR,
		FRONT_RIGHT_INDICATOR,
		REAR_LEFT_INDICATOR,
		REAR_RIGHT_INDICATOR,*/

	MAX_LIGHTS            // MUST BE 16 OR LESS
};

#pragma pack(push, 1)
struct VehicleAudioPropertiesStruct
{
	int16_t VehicleType;		//	1: +  0
	int16_t EngineOnSound;	//  2: +  2
	int16_t EngineOffSound;	//  3: +  4
	int16_t field_4;			//  4: +  6
	float field_5;			//  5: +  8
	float field_6;			//  6: + 12
	char HornTon;				//  7: + 16
	char field_8[3];			//	8: + 17, zeros
	float HornHigh;			//  9: + 20
	char DoorSound;			// 10: + 24
	char field_11[1];			// 11: + 25, zeros
	char RadioNum;			// 12: + 26
	char RadioType;			// 13: + 27
	char field_14;			// 14: + 28
	char field_15[3];			// 15: + 29, zeros
	float field_16;			// 16: + 32
};
#pragma pack(pop)

//-----------------------------------------------------------

#define RW_FRAME_NAME_LENGTH      23
#pragma pack(push, 1)
struct RwListEntry
{
	RwListEntry* next, * prev;
};

struct RwList
{
	RwListEntry root;
};

struct RwFrame
{
	RwObject        object;                 // 0
	void* pad1, * pad2;            // 8
	RwMatrix        modelling;              // 16
	RwMatrix        ltm;                    // 32
	RwList          objects;                // 48
	struct RwFrame* child;                  // 56
	struct RwFrame* next;                   // 60
	struct RwFrame* root;                   // 64

	// Rockstar Frame extension (0x253F2FE) (24 bytes)
	unsigned char pluginData[8];                               // padding
	char          szName[RW_FRAME_NAME_LENGTH + 1];            // name (as stored in the frame extension)
};
#pragma pack(pop)

#define ATOMIC_ID_FLAG_TWO_VERSIONS_DAMAGED     2

struct SHandlingData
{
	uint8_t flag;
	float fValue;
	int iValue;
	SHandlingData(uint8_t uFlag, float fvalue, int ivalue) : flag(uFlag), fValue(fvalue), iValue(ivalue) {}
};

struct CVector2DFloat
{
	float X;
	float Y;
};


#pragma pack(push, 1)
typedef struct _ATTACHED_OBJECT_INFO_INTERNAL
{
	uint32_t 	dwModelId;
	uint32_t 	dwBone;
	CVector 	vecOffset;
	CVector 	vecRotation;
	CVector 	vecScale;
	uint32_t 	dwColor[2];

	bool 		bState;
	class CObject* pObject;
	uint32_t 	dwSampBone;
} ATTACHED_OBJECT_INFO_INTERNAL;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _ATTACHED_OBJECT_INFO
{
	uint32_t 	dwModelId;
	uint32_t 	dwBone;
	CVector 	vecOffset;
	CVector 	vecRotation;
	CVector 	vecScale;
	uint32_t 	dwColor[2];
} ATTACHED_OBJECT_INFO;
#pragma pack(pop)

struct RpHAnimHierarchy
{
	int32_t             flags;          /**< Flags for the hierarchy  */
	int32_t             numNodes;      /**< Number of nodes in the hierarchy  */

	RwMatrix* pMatrixArray;   /**< Pointer to node matrices*/
};

//-----------------------------------------------------------

enum eDoors
{
	BONNET = 0,
	BOOT,
	FRONT_LEFT_DOOR,
	FRONT_RIGHT_DOOR,
	REAR_LEFT_DOOR,
	REAR_RIGHT_DOOR,
	MAX_DOORS
};

enum eDoorStatus
{
	DT_DOOR_INTACT = 0,
	DT_DOOR_SWINGING_FREE,
	DT_DOOR_BASHED,
	DT_DOOR_BASHED_AND_SWINGING_FREE,
	DT_DOOR_MISSING
};

#pragma pack(push, 1)
typedef struct _RES_ENTRY_OBJ
{
	PADDING(_pad0, 48); 	// 0-48
	uintptr_t validate; 	//48-52
	PADDING(_pad1, 4); 		//52-56
} RES_ENTRY_OBJ;
static_assert(sizeof(_RES_ENTRY_OBJ) == 56);
#pragma pack(pop)

enum eWheelPosition
{
	FRONT_LEFT_WHEEL = 0,
	REAR_LEFT_WHEEL,
	FRONT_RIGHT_WHEEL,
	REAR_RIGHT_WHEEL,

	MAX_WHEELS

};

typedef struct _DAMAGE_MANAGER_INTERFACE            // 28 bytes due to the way its packed (24 containing actual data)
{
	float fWheelDamageEffect;
	uint8_t  bEngineStatus;            // old - wont be used
	uint8_t  Wheel[MAX_WHEELS];
	uint8_t  Door[MAX_DOORS];
	uint32_t Lights;            // 2 bits per light
	uint32_t Panels;            // 4 bits per panel
} DAMAGE_MANAGER_INTERFACE;

enum ePanels
{
	FRONT_LEFT_PANEL = 0,
	FRONT_RIGHT_PANEL,
	REAR_LEFT_PANEL,
	REAR_RIGHT_PANEL,
	WINDSCREEN_PANEL,            // needs to be in same order as in component.h
	FRONT_BUMPER,
	REAR_BUMPER,

	MAX_PANELS            // MUST BE 8 OR LESS
};

enum eComponentStatus
{
	DT_PANEL_INTACT = 0,
	//  DT_PANEL_SHIFTED,
	DT_PANEL_BASHED,
	DT_PANEL_BASHED2,
	DT_PANEL_MISSING
};

#pragma pack(push, 1)
typedef struct _AIM_SYNC_DATA
{
	uint8_t		byteCamMode;
	CVector 	vecAimf;
	CVector 	vecAimPos;
	float 		fAimZ;
	uint8_t 	byteCamExtZoom : 6;
	uint8_t 	byteWeaponState : 2;
	uint8_t 	aspect_ratio;
} AIM_SYNC_DATA;
#pragma pack(pop)
//-----------------------------------------------------------

#pragma pack(push, 1)
typedef struct _BULLET_SYNC
{
	uint8_t 	byteHitType;			// +0
	uint16_t 	PlayerID;				// +1
	CVector 	vecOrigin;				// +3
	CVector 	vecPos;					// +12
	CVector 	vecOffset;				// +20
	uint8_t 	byteWeaponID;			// +28
} BULLET_SYNC;					// size = 29

class CEntityGta;

typedef struct _BULLET_DATA {
	uint32_t 	unk;
	CVector 	vecOrigin;
	CVector 	vecPos;
	CVector 	vecOffset;
	CEntityGta* pEntity;
} BULLET_DATA;

#pragma pack(pop)

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5
#define	VEHICLE_SUBTYPE_PUSHBIKE		6
#define	VEHICLE_SUBTYPE_TRAIN			7

//-----------------------------------------------------------

#define TRAIN_PASSENGER_LOCO			538
#define TRAIN_FREIGHT_LOCO				537
#define TRAIN_PASSENGER					570
#define TRAIN_FREIGHT					569
#define TRAIN_TRAM						449
#define HYDRA							520

//-----------------------------------------------------------

#define ACTION_WASTED					55
#define ACTION_DEATH					54
#define ACTION_INCAR					50
#define ACTION_NORMAL					1
#define ACTION_SCOPE					12
#define ACTION_NONE						0 

//-----------------------------------------------------------

#define WEAPON_MODEL_BRASSKNUCKLE		331 // was 332
#define WEAPON_MODEL_GOLFCLUB			333
#define WEAPON_MODEL_NITESTICK			334
#define WEAPON_MODEL_KNIFE				335
#define WEAPON_MODEL_BAT				336
#define WEAPON_MODEL_SHOVEL				337
#define WEAPON_MODEL_POOLSTICK			338
#define WEAPON_MODEL_KATANA				339
#define WEAPON_MODEL_CHAINSAW			341
#define WEAPON_MODEL_DILDO				321
#define WEAPON_MODEL_DILDO2				322
#define WEAPON_MODEL_VIBRATOR			323
#define WEAPON_MODEL_VIBRATOR2			324
#define WEAPON_MODEL_FLOWER				325
#define WEAPON_MODEL_CANE				326
#define WEAPON_MODEL_GRENADE			342 // was 327
#define WEAPON_MODEL_TEARGAS			343 // was 328
#define WEAPON_MODEL_MOLOTOV			344 // was 329
#define WEAPON_MODEL_COLT45				346
#define WEAPON_MODEL_SILENCED			347
#define WEAPON_MODEL_DEAGLE				348
#define WEAPON_MODEL_SHOTGUN			349
#define WEAPON_MODEL_SAWEDOFF			350
#define WEAPON_MODEL_SHOTGSPA			351
#define WEAPON_MODEL_UZI				352
#define WEAPON_MODEL_MP5				353
#define WEAPON_MODEL_AK47				355
#define WEAPON_MODEL_M4					356
#define WEAPON_MODEL_TEC9				372
#define WEAPON_MODEL_RIFLE				357
#define WEAPON_MODEL_SNIPER				358
#define WEAPON_MODEL_ROCKETLAUNCHER		359
#define WEAPON_MODEL_HEATSEEKER			360
#define WEAPON_MODEL_FLAMETHROWER		361
#define WEAPON_MODEL_MINIGUN			362
#define WEAPON_MODEL_SATCHEL			363
#define WEAPON_MODEL_BOMB				364
#define WEAPON_MODEL_SPRAYCAN			365
#define WEAPON_MODEL_FIREEXTINGUISHER	366
#define WEAPON_MODEL_CAMERA				367
#define WEAPON_MODEL_NIGHTVISION		368	// newly added
#define WEAPON_MODEL_INFRARED			369	// newly added
#define WEAPON_MODEL_JETPACK			370	// newly added
#define WEAPON_MODEL_PARACHUTE			371
#define WEAPON_MODEL_PARACHUTE			371

#pragma pack(pop)