//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "Entity.h"
#include "../PedIntelligence.h"
#include "../Weapon.h"
#include "../Enums/ePedState.h"
#include "game/Enums/eMoveState.h"
#include "game/common.h"
#include "game/Enums/AnimationEnums.h"
#include "game/Core/Vector2D.h"

#pragma pack(push, 1)
typedef struct _PED_TYPE
{
    ENTITY_TYPE entity; 		// 0000-0184	;entity
    uint8_t _pad106[108];
    uint32_t _pad107;			// 0358-0362	;dwPedType
    uint8_t _pad101[722];
    CPedIntelligence* pPedIntelligence; // 1084-1088
    uint8_t _pad100[8];
    ePedState m_nPedState;			// 1096-1100	;Action
    eMoveState m_eMoveState;
    uint8_t m_storedCollPoly[0x2c];
    float m_distTravelledSinceLastHeightCheck;
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

    uintptr_t *m_apBones[19];
    AssocGroupId m_motionAnimGroup;
    CVector2D m_extractedVelocity;
    uint8_t m_acquaintances[0x14];
    uintptr_t *m_pWeaponClump;
    uintptr_t *m_pWeaponFlashFrame;
    uintptr_t *m_pGogglesClump;
    bool *m_pbGogglesEffect;
    uint16_t m_nGunFlashBlendAmount;
    uint16_t m_nGunFlashBlendOutRate;
    uint16_t m_nGunFlashBlendAmount2;
    uint16_t m_nGunFlashBlendOutRate2;
    uint8_t m_ik[0x20];
    uint32_t m_nAntiSpazTimer;
    eMoveState m_eMoveStateAnim;
    eMoveState m_eStoredMoveState;
    float fHealth;		 		// 1344-1348	;Health
    float fMaxHealth;			// 1348-1352	;MaxHealth
    float fArmour;				// 1352-1356	;Armour
    float fAim;
    uint8_t _pad104[8];
    float m_fCurrentRotation;			// 1368-1372	;Rotation1
    float m_fAimingRotation;			// 1372-1376	;Rotation2
    uint8_t _pad105[44];
    uint32_t pVehicle;			// 1420-1424	;pVehicle
    uint8_t _pad108[8];
    uint32_t dwPedType;			// 1432-1436	;dwPedType
    uint32_t dwUnk1;	 // 1436-1440
    CWeapon WeaponSlots[13]; // 1440-1804
    uint8_t _pad270[12];
    uint8_t byteCurWeaponSlot; // 1816-1817
    uint8_t _pad280[23];
    uint32_t pFireObject;	 // 1840-1844
    uint8_t _pad281[44];
    uint32_t  dwWeaponUsed; // 1888-1892
    uintptr_t pdwDamageEntity; // 1892-1896
} PED_TYPE;
#pragma pack(pop)
