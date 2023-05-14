#pragma once

#include "game/Core/Vector.h"

struct RetextureMaterial
{
	RwTexture* texture;
	bool m_bUse;
};

class CObject : public CEntity
{
public:
	RwMatrix	m_matTarget;
	RwMatrix	m_matCurrent;
	bool 		m_bIsMoving {false};
	float		m_fMoveSpeed {0.f};
	bool		m_bIsPlayerSurfing;
	bool		m_bNeedRotate;

	CQuaternion m_quatTarget;
	CQuaternion m_quatStart;

	CVector m_vecAttachedOffset;
	CVector m_vecAttachedRotation;
	uint16_t m_usAttachedVehicle;
	uint8_t m_bAttachedType;

	CVector 		m_vecRot;
	CVector		m_vecTargetRot;
	CVector		m_vecTargetRotTar;
	CVector		m_vecRotationTarget;
	CVector		m_vecSubRotationTarget;
	float		m_fDistanceToTargetPoint;
	uint32_t		m_iStartMoveTick;
	bool 		bNeedReAttach = false;

	CObject(int iModel, float fPosX, float fPosY, float fPosZ, CVector vecRot, float fDrawDistance);
	~CObject();

	void Process(float fElapsedTime);
	float DistanceRemaining(RwMatrix *matPos);
	float RotaionRemaining(CVector matPos);

	void SetPos(float x, float y, float z);
	void MoveTo(float x, float y, float z, float speed, float rX, float rY, float rZ);

	void AttachToVehicle(uint16_t usVehID, CVector* pVecOffset, CVector* pVecRot);
	void ProcessAttachToVehicle(CVehicle* pVehicle);

	void InstantRotate(float x, float y, float z);
	void StopMoving();

	void GetRotation(float* pfX, float* pfY, float* pfZ);

	static float DistanceRemaining(RwMatrix *matPos, RwMatrix *m_matPositionTarget);

	float RotaionRemaining(CVector &matPos, CVector &m_vecRot);

	void InstantRotate(CVector &rad);

	void SetRot(CVector &rad);

    void SetRot(float &radX, float &radY, float &radZ);
};