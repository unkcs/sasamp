//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "../../PedIntelligence.h"
#include "../../Weapon.h"
#include "../../Enums/ePedState.h"
#include "../../Enums/eMoveState.h"
#include "../../Enums/AnimationEnums.h"
#include "../../Core/Vector2D.h"
#include "../Physical.h"
#include "../../Enums/ePedType.h"
#include "../../Enums/eWeaponSkill.h"
#include "../../Enums/eWeaponType.h"

class CVehicleGta;

enum eFightingStyle : int8 {
    STYLE_STANDARD = 4,
    STYLE_BOXING,
    STYLE_KUNG_FU,
    STYLE_KNEE_HEAD,
    // various melee weapon styles
    STYLE_GRAB_KICK = 15,
    STYLE_ELBOWS = 16,
};

enum ePedCreatedBy : uint8 {
    PED_UNKNOWN = 0,
    PED_GAME = 1,
    PED_MISSION = 2,
    PED_GAME_MISSION = 3, // used for the playbacked peds on replay
};

#pragma pack(push, 1)
struct CPedGta : CPhysical {
    uint8_t             m_PedAudioEntity[0x15C];
    uint8_t             m_PedSpeechAudioEntity[0x100];
    uint8_t             m_PedWeaponAudioEntity[0xA8];
    CPedIntelligence*   pPedIntelligence;
    uintptr_t           *m_pPlayerData;
    ePedCreatedBy       m_nCreatedBy;
    uint8_t             pad6[3];
    ePedState           m_nPedState;
    eMoveState          m_eMoveState;
    uint8_t             m_storedCollPoly[0x2c];
    float               m_distTravelledSinceLastHeightCheck;
    union {
        /* https://github.com/multitheftauto/mtasa-blue/blob/master/Client/game_sa/CPedSA.h */
        struct {
            // 1st byte starts here (m_nPedFlags)
            bool bIsStanding : 1 = false;            // is ped standing on something
            bool bWasStanding : 1 = false;           // was ped standing on something
            bool bIsLooking : 1 = false;             // is ped looking at something or in a direction
            bool bIsRestoringLook : 1 = false;       // is ped restoring head position from a look
            bool bIsAimingGun : 1 = false;           // is ped aiming gun
            bool bIsRestoringGun : 1 = false;        // is ped moving gun back to default posn
            bool bCanPointGunAtTarget : 1 = false;   // can ped point gun at target
            bool bIsTalking : 1 = false;             // is ped talking(see Chat())

            bool bInVehicle : 1 = false;             // is in a vehicle
            bool bIsInTheAir : 1 = false;            // is in the air
            bool bIsLanding : 1 = false;             // is landing after being in the air
            bool bHitSomethingLastFrame : 1 = false; // has been in a collision last frame
            bool bIsNearCar : 1 = false;             // has been in a collision last frame
            bool bRenderPedInCar : 1 = true;         // has been in a collision last frame
            bool bUpdateAnimHeading : 1 = false;     // update ped heading due to heading change during anim sequence
            bool bRemoveHead : 1 = false;            // waiting on AntiSpazTimer to remove head - TODO: See `RemoveBodyPart` - The name seems to be incorrect. It should be like `bHasBodyPartToRemove`.

            bool bFiringWeapon : 1 = false;         // is pulling trigger
            bool bHasACamera : 1;                   // does ped possess a camera to document accidents
            bool bPedIsBleeding : 1 = false;        // Ped loses a lot of blood if true
            bool bStopAndShoot : 1 = false;         // Ped cannot reach target to attack with fist, need to use gun
            bool bIsPedDieAnimPlaying : 1 = false;  // is ped die animation finished so can dead now
            bool bStayInSamePlace : 1 = false;      // when set, ped stays put
            bool bKindaStayInSamePlace : 1 = false; // when set, ped doesn't seek out opponent or cover large distances. Will still shuffle and look for cover
            bool bBeingChasedByPolice : 1 = false;  // use nodes for route find

            bool bNotAllowedToDuck : 1 = false;     // Is this ped allowed to duck at all?
            bool bCrouchWhenShooting : 1 = false;   // duck behind cars etc
            bool bIsDucking : 1 = false;            // duck behind cars etc
            bool bGetUpAnimStarted : 1 = false;     // don't want to play getup anim if under something
            bool bDoBloodyFootprints : 1 = false;   // bIsLeader
            bool bDontDragMeOutCar : 1 = false;
            bool bStillOnValidPoly : 1 = false;     // set if the polygon the ped is on is still valid for collision
            bool bAllowMedicsToReviveMe : 1 = true;

            // 5th byte starts here (m_nSecondPedFlags)
            bool bResetWalkAnims : 1 = false;
            bool bOnBoat : 1 = false;               // flee but only using nodes
            bool bBusJacked : 1 = false;            // flee but only using nodes
            bool bFadeOut : 1 = false;              // set if you want ped to fade out
            bool bKnockedUpIntoAir : 1 = false;     // has ped been knocked up into the air by a car collision
            bool bHitSteepSlope : 1 = false;        // has ped collided/is standing on a steep slope (surface type)
            bool bCullExtraFarAway : 1 = false;     // special ped only gets culled if it's extra far away (for roadblocks)
            bool bTryingToReachDryLand : 1 = false; // has ped just exited boat and trying to get to dry land

            bool bCollidedWithMyVehicle : 1 = false;
            bool bRichFromMugging : 1 = false;        // ped has lots of cash cause they've been mugging people
            bool bChrisCriminal : 1 = false;          // Is a criminal as killed during Chris' police mission (should be counted as such)
            bool bShakeFist : 1 = false;              // test shake hand at look entity
            bool bNoCriticalHits : 1 = false;         // ped cannot be killed by a single bullet
            bool bHasAlreadyBeenRecorded : 1 = false; // Used for replays
            bool bUpdateMatricesRequired : 1 = false; // if PedIK has altered bones so matrices need updated this frame
            bool bFleeWhenStanding : 1 = false;       //

            bool bMiamiViceCop : 1 = false;
            bool bMoneyHasBeenGivenByScript : 1 = false;
            bool bHasBeenPhotographed : 1 = false;
            bool bIsDrowning : 1 = false;
            bool bDrownsInWater : 1 = true;
            bool bHeadStuckInCollision : 1 = false;
            bool bDeadPedInFrontOfCar : 1 = false;
            bool bStayInCarOnJack : 1 = false;

            bool bDontFight : 1 = false;
            bool bDoomAim : 1 = true;
            bool bCanBeShotInVehicle : 1 = true;
            bool bPushedAlongByCar : 1 = false; // ped is getting pushed along by car collision (so don't take damage from horz velocity)
            bool bNeverEverTargetThisPed : 1 = false;
            bool bThisPedIsATargetPriority : 1 = false;
            bool bCrouchWhenScared : 1 = false;
            bool bKnockedOffBike : 1 = false; // TODO: Maybe rename to `bIsJumpingOut` or something similar, see x-refs

            // 9th byte starts here (m_nThirdPedFlags)
            bool bDonePositionOutOfCollision : 1 = false;
            bool bDontRender : 1 = false;
            bool bHasBeenAddedToPopulation : 1 = false;
            bool bHasJustLeftCar : 1 = false;
            bool bIsInDisguise : 1 = false;
            bool bDoesntListenToPlayerGroupCommands : 1 = false;
            bool bIsBeingArrested : 1 = false;
            bool bHasJustSoughtCover : 1 = false;

            bool bKilledByStealth : 1 = false;
            bool bDoesntDropWeaponsWhenDead : 1 = false;
            bool bCalledPreRender : 1 = false;
            bool bBloodPuddleCreated : 1 = false; // Has a static puddle of blood been created yet
            bool bPartOfAttackWave : 1 = false;
            bool bClearRadarBlipOnDeath : 1 = false;
            bool bNeverLeavesGroup : 1 = false;        // flag that we want to test 3 extra spheres on col model
            bool bTestForBlockedPositions : 1 = false; // this sets these indicator flags for various positions on the front of the ped

            bool bRightArmBlocked : 1 = false;
            bool bLeftArmBlocked : 1 = false;
            bool bDuckRightArmBlocked : 1 = false;
            bool bMidriffBlockedForJump : 1 = false;
            bool bFallenDown : 1 = false;
            bool bUseAttractorInstantly : 1 = false;
            bool bDontAcceptIKLookAts : 1 = false;
            bool bHasAScriptBrain : 1 = false;

            bool bWaitingForScriptBrainToLoad : 1 = false;
            bool bHasGroupDriveTask : 1 = false;
            bool bCanExitCar : 1 = true;
            bool CantBeKnockedOffBike : 2 = false; // (harder for mission peds)   normal(also for mission peds)
            bool bHasBeenRendered : 1 = false;
            bool bIsCached : 1 = false;
            bool bPushOtherPeds : 1 = false;   // GETS RESET EVERY FRAME - SET IN TASK: want to push other peds around (eg. leader of a group or ped trying to get in a car)

            // 13th byte starts here (m_nFourthPedFlags)
            bool bHasBulletProofVest : 1 = false;
            bool bUsingMobilePhone : 1 = false;
            bool bUpperBodyDamageAnimsOnly : 1 = false;
            bool bStuckUnderCar : 1 = false;
            bool bKeepTasksAfterCleanUp : 1 = false; // If true ped will carry on with task even after cleanup
            bool bIsDyingStuck : 1 = false;
            bool bIgnoreHeightCheckOnGotoPointTask : 1 = false; // set when walking round buildings, reset when task quits
            bool bForceDieInCar : 1 = false;

            bool bCheckColAboveHead : 1 = false;
            bool bIgnoreWeaponRange : 1 = false;
            bool bDruggedUp : 1 = false;
            bool bWantedByPolice : 1 = false; // if this is set, the cops will always go after this ped when they are doing a KillCriminal task
            bool bSignalAfterKill : 1 = true;
            bool bCanClimbOntoBoat : 1 = false;
            bool bPedHitWallLastFrame : 1 = false; // useful to store this so that AI knows (normal will still be available)
            bool bIgnoreHeightDifferenceFollowingNodes : 1 = false;

            bool bMoveAnimSpeedHasBeenSetByTask : 1 = false;
            bool bGetOutUpsideDownCar : 1 = true;
            bool bJustGotOffTrain : 1 = false;
            bool bDeathPickupsPersist : 1 = false;
            bool bTestForShotInVehicle : 1 = false;
            bool bUsedForReplay : 1 = false; // This ped is controlled by replay and should be removed when replay is done.
        };
        uint8_t m_nPedFlags[0x10];
    };

    uintptr_t       *m_apBones[19];
    AssocGroupId    m_motionAnimGroup;
    CVector2D       m_extractedVelocity;
    uint8_t         m_acquaintances[0x14];
    uintptr_t       *m_pWeaponClump;
    uintptr_t       *m_pWeaponFlashFrame;
    uintptr_t       *m_pGogglesClump;
    bool            *m_pbGogglesEffect;
    uint16_t        m_nGunFlashBlendAmount;
    uint16_t        m_nGunFlashBlendOutRate;
    uint16_t        m_nGunFlashBlendAmount2;
    uint16_t        m_nGunFlashBlendOutRate2;
    uint8_t         m_ik[0x20];
    uint32_t        m_nAntiSpazTimer;
    eMoveState      m_eMoveStateAnim;
    eMoveState      m_eStoredMoveState;
    float           fHealth;
    float           fMaxHealth;
    float           fArmour;
    float           fAim;
    uint8_t         _pad104[8];
    float           m_fCurrentRotation;
    float           m_fAimingRotation;
    uint8_t         _pad105[44];
    CVehicleGta*    pVehicle;
    CVehicleGta*    m_VehDeadInFrontOf;
    uintptr_t       *m_pAccident;
    ePedType        m_nPedType;
    uintptr_t*      m_pStats;
    CWeapon         WeaponSlots[13];
    eWeaponType     m_nSavedWeapon;   // when we need to hide ped weapon, we save it temporary here
    eWeaponType     m_nDelayedWeapon; // 'delayed' weapon is like an additional weapon, f.e., simple cop has a nitestick as current and pistol as delayed weapons
    uint32          m_nDelayedWeaponAmmo;
    uint8_t         byteCurWeaponSlot;
    uint8           m_nWeaponShootingRate;
    uint8           m_nWeaponAccuracy;
    uint8           pad10;
    CEntityGta      *m_pEntLockOnTarget;
    CEntityGta      *m_pEntMagnetizeTarget;
    CVector         m_vecWeaponPrevPos;
    eWeaponSkill    m_nWeaponSkill;
    eFightingStyle  m_nFightingStyle;
    char            m_nAllowedAttackMoves;
    uint8           field_72F;
    uintptr_t       *m_pFire;
    float           m_fireDmgMult;
    CEntityGta*     m_pLookTarget;
    float           m_fLookDirection; // In RAD
    int32           m_nWeaponModelId;
    uint32          m_nUnconsciousTimer;
    uint32          m_nLookTimer;
    uint32          m_nAttackTimer;
    uint32          m_nTimeOfDeath;
    int8            m_nLimbRemoveIndex;
    int8            pad9;
    uint16          m_MoneyCarried;
    float           m_wobble;
    float           m_wobbleSpeed;
    uint8           m_nLastDamagedWeaponType; // See eWeaponType
    uint8           pad8[3];
    CEntityGta*     m_pLastEntityDamage;
    uint32          m_nLastDamagedTime;
    CVector         m_vecAttachOffset;
    uint16          m_nAttachLookDirn;
    uint16          pad7;
    float           m_fAttachHeadingLimit;
    float           m_fAttachVerticalLimit;
    int32           m_nOriginalWeaponAmmo;
    uintptr_t       *m_pCoverPoint;
    uintptr_t       *m_pLastEntryExit;
    float           m_fRemoveRangeMultiplier;
    int16           StreamedScriptBrainToLoad;
    uint16          pad11;
    uint32          LastTalkSfx;
};
#pragma pack(pop)

VALIDATE_SIZE(CPedGta, 0x7A4-4); // -4 2.10