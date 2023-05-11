//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "Placeable.h"
#include "game/Core/Vector.h"
#include "game/Reference.h"
#include "../Core/Link.h"
#include "game/Enums/eEntityStatus.h"
#include "game/Enums/eEntityType.h"
#include "game/Enums/eSurfaceType.h"
#include "game/quaternion.h"
#include "game/Enums/eAreaCodes.h"
#include "game/Collision/ColModel.h"

#pragma pack(push, 1)
class CEntityGta : public CPlaceable
{
public:
    union {
        struct RwObject* m_pRwObject;
        struct RpClump*  m_pRwClump;
        struct RpAtomic* m_pRwAtomic;
    };

    union {
        struct {
            /* https://github.com/multitheftauto/mtasa-blue/blob/master/Client/game_sa/CEntitySA.h */
            bool m_bUsesCollision : 1;               // does entity use collision
            bool m_bCollisionProcessed : 1;          // has object been processed by a ProcessEntityCollision function
            bool m_bIsStatic : 1;                    // is entity static
            bool m_bHasContacted : 1;                // has entity processed some contact forces
            bool m_bIsStuck : 1;                     // is entity stuck
            bool m_bIsInSafePosition : 1;            // is entity in a collision free safe position
            bool m_bWasPostponed : 1;                // was entity control processing postponed
            bool m_bIsVisible : 1;                   // is the entity visible

            bool m_bIsBIGBuilding : 1;               // Set if this entity is a big building
            bool m_bRenderDamaged : 1;               // use damaged LOD models for objects with applicable damage
            bool m_bStreamingDontDelete : 1;         // Don't let the streaming remove this
            bool m_bRemoveFromWorld : 1;             // remove this entity next time it should be processed
            bool m_bHasHitWall : 1;                  // has collided with a building (changes subsequent collisions)
            bool m_bImBeingRendered : 1;             // don't delete me because I'm being rendered
            bool m_bDrawLast : 1;                    // draw object last
            bool m_bDistanceFade : 1;                // Fade entity because it is far away

            bool m_bDontCastShadowsOn : 1;           // Don't cast shadows on this object
            bool m_bOffscreen : 1;                   // offscreen flag. This can only be trusted when it is set to true
            bool m_bIsStaticWaitingForCollision : 1; // this is used by script created entities - they are static until the collision is loaded below them
            bool m_bDontStream : 1;                  // tell the streaming not to stream me
            bool m_bUnderwater : 1;                  // this object is underwater change drawing order
            bool m_bHasPreRenderEffects : 1;         // Object has a prerender effects attached to it
            bool m_bIsTempBuilding : 1;              // whether the building is temporary (i.e. can be created and deleted more than once)
            bool m_bDontUpdateHierarchy : 1;         // Don't update the animation hierarchy this frame

            bool m_bHasRoadsignText : 1;             // entity is roadsign and has some 2dEffect text stuff to be rendered
            bool m_bDisplayedSuperLowLOD : 1;
            bool m_bIsProcObject : 1;                // set object has been generated by procedural object generator
            bool m_bBackfaceCulled : 1;              // has backface culling on
            bool m_bLightObject : 1;                 // light object with directional lights
            bool m_bUnimportantStream : 1;           // set that this object is unimportant, if streaming is having problems
            bool m_bTunnel : 1;                      // Is this model part of a tunnel
            bool m_bTunnelTransition : 1;            // This model should be rendered from within and outside the tunnel
        };
        uint32 m_nFlags;
    };
    union {
        struct {
            uint16 m_nRandomSeedUpperByte : 8;
            uint16 m_nRandomSeedSecondByte : 8;
        };
        uint16 m_nRandomSeed;
    };

    uint16_t            m_nModelIndex;
    CReference          *m_pReferences;
    CLink<CEntityGta*>  *m_pLastRenderedLink;
    uint16_t            m_nScanCode;
    uint8_t             m_nIplIndex;
    eAreaCodes          m_areaCode;
    union {
        int32       m_nLodIndex; // -1 - without LOD model
        CEntityGta* m_pLod;
    };
    uint8_t         numLodChildren;
    int8_t          numLodChildrenRendered;
    eEntityType     m_nType : 3;          // Mask: & 0x7  = 7
    eEntityStatus   m_nStatus : 5;        // Mask: & 0xF8 = 248 (Remember: In the original code unless this was left shifted the value it's compared to has to be left shifted by 3!)
    uint8_t         pad_0;

    static void InjectHooks();

public:
    void UpdateRwFrame();
    void UpdateRpHAnim();
    bool HasPreRenderEffects();
    bool DoesNotCollideWithFlyers();
    void ModifyMatrixForPoleInWind();
    RwMatrix* GetModellingMatrix();
    void ResolveReferences();
    void PruneReferences();
    void RegisterReference(CEntityGta** entity);
    bool LivesInThisNonOverlapSector(int32 sectorX, int32 sectorY);
    void CleanUpOldReference(CEntityGta** entity); // See helper SafeCleanUpOldReference
    // is entity touching entity
    bool GetIsTouching(CEntityGta* entity);
    CVector* GetBoundCentre(CVector* pOutCentre);
    void GetBoundCentre(CVector& outCentre);
    CVector GetBoundCentre();
    CColModel* GetColModel() const;
    CVector TransformFromObjectSpace(const CVector& offset);
    CVector* TransformFromObjectSpace(CVector& outPos, const CVector& offset);
    void UpdateRW();

    void SetCollisionChecking(bool bCheck);

    // Wrapper around the mess called `CleanUpOldReference`
    // Takes in `ref` (which is usually a member variable),
    // calls `CleanUpOldReference` on it, then sets it to `nullptr`
    // Used often in the code.
    template<typename T>
    static void ClearReference(T*& ref) requires std::is_base_of_v<CEntityGta, T> {
        if (ref) {
            ref->CleanUpOldReference(reinterpret_cast<CEntityGta**>(&ref));
            ref = nullptr;
        }
    }

    // Wrapper around the mess called "entity references"
    // This one sets the given `inOutRef` member variable to `entity`
    // + clears the old entity (if any)
    // + set the new entity (if any)
    template<typename T, typename Y>
    requires std::is_base_of_v<CEntityGta, T> && std::is_base_of_v<CEntityGta, Y>
    static void ChangeEntityReference(T*& inOutRef, Y* entity) {
        ClearReference(inOutRef); // Clear old
        if (entity) { // Set new (if any)
            inOutRef = entity;
            inOutRef->RegisterReference(reinterpret_cast<CEntityGta**>(&inOutRef));
        }
    }

    // Similar to `ChangeEntityReference`, but doesn't clear the old reference
    template<typename T, typename Y>
    requires std::is_base_of_v<CEntityGta, T> && std::is_base_of_v<CEntityGta, Y>
    static void SetEntityReference(T*& inOutRef, Y* entity) {
        inOutRef = entity;
        inOutRef->RegisterReference(reinterpret_cast<CEntityGta**>(&inOutRef));
    }

    // Register a reference to the entity that is stored in that given reference
    template<typename T>
    static void RegisterReference(T*& ref) requires std::is_base_of_v<CEntityGta, T> {
        ref->RegisterReference(reinterpret_cast<CEntityGta**>(&ref));
    }

    template<typename T>
    static void CleanUpOldReference(T*& ref) requires std::is_base_of_v<CEntityGta, T> {
        ref->CleanUpOldReference(reinterpret_cast<CEntityGta**>(&ref));
    }

    template<typename T>
    static void SafeRegisterRef(T*& e) requires std::is_base_of_v<CEntityGta, T> {
        if (e) {
            e->RegisterReference(reinterpret_cast<CEntityGta**>(&e));
        }
    }

    template<typename T>
    static void SafeCleanUpRef(T*& e) requires std::is_base_of_v<CEntityGta, T> {
        if (e) {
            e->CleanUpOldReference(reinterpret_cast<CEntityGta**>(&e));
        }
    }

    void Teleport(float fX, float fY, float fZ)
    {
        (( void (*)(CEntityGta*, float, float, float, bool))(*(void**)(vtable+0x3C)))(this, fX, fY, fZ, 0);
    }

public:
    [[nodiscard]] bool IsPhysical() const { return m_nType > ENTITY_TYPE_BUILDING && m_nType < ENTITY_TYPE_DUMMY; }
    [[nodiscard]] bool IsNothing()  const { return m_nType == ENTITY_TYPE_NOTHING; }
    [[nodiscard]] bool IsVehicle()  const { return m_nType == ENTITY_TYPE_VEHICLE; }
    [[nodiscard]] bool IsPed()      const { return m_nType == ENTITY_TYPE_PED; }
    [[nodiscard]] bool IsObject()   const { return m_nType == ENTITY_TYPE_OBJECT; }
    [[nodiscard]] bool IsBuilding() const { return m_nType == ENTITY_TYPE_BUILDING; }
    [[nodiscard]] bool IsDummy()    const { return m_nType == ENTITY_TYPE_DUMMY; }

    float GetDistanceFromCamera();

    float GetDistanceFromLocalPlayerPed() const;

    float GetDistanceFromPoint(float X, float Y, float Z) const;
};
static_assert(sizeof(CEntityGta) == 0x38);
#pragma pack(pop)
