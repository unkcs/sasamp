#pragma once

#include "CVector.h"

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
	uint8_t		m_byteMoving;
	float		m_fMoveSpeed;
	bool		m_bIsPlayerSurfing;
	bool		m_bNeedRotate;

	CQuaternion m_quatTarget;
	CQuaternion m_quatStart;

	VECTOR m_vecAttachedOffset;
	VECTOR m_vecAttachedRotation;
	uint16_t m_usAttachedVehicle;
	uint8_t m_bAttachedType;

	VECTOR 		m_vecRot;
	VECTOR		m_vecTargetRot;
	VECTOR		m_vecTargetRotTar;
	VECTOR		m_vecRotationTarget;
	VECTOR		m_vecSubRotationTarget;
	float		m_fDistanceToTargetPoint;
	uint32_t		m_dwMoveTick;

	CObject(int iModel, float fPosX, float fPosY, float fPosZ, CVector vecRot, float fDrawDistance);
	~CObject();

	void Process(float fElapsedTime);
	float DistanceRemaining(RwMatrix *matPos);
	float RotaionRemaining(VECTOR matPos);

	void SetPos(float x, float y, float z);
	void MoveTo(float x, float y, float z, float speed, float rX, float rY, float rZ);

	void AttachToVehicle(uint16_t usVehID, CVector* pVecOffset, CVector* pVecRot);
	void ProcessAttachToVehicle(CVehicle* pVehicle);

	void InstantRotate(float x, float y, float z);
	void StopMoving();

	void ApplyMoveSpeed();
	void GetRotation(float* pfX, float* pfY, float* pfZ);
};