#pragma once

#include "game/Core/Vector.h"
#include "Entity/Physical.h"
class CEntity
{
public:
	CEntity() {};
	virtual ~CEntity() {};

	virtual void Add();
	//virtual void Remove();
	void Render();
	void UpdateMatrix(RwMatrix mat);
	void SetGravityProcessing(bool bProcess);
	void UpdateRwMatrixAndFrame();

	// 0.3.7
	void GetMatrix(RwMatrix* Matrix);
	// 0.3.7
	void SetMatrix(RwMatrix mat);

	// 0.3.7
	uint16_t GetModelIndex();

	bool SetModelIndex(unsigned int uiModel);
	// 0.3.7
	bool IsAdded();
	// 0.3.7
	float GetDistanceFromCamera();
	float GetDistanceFromLocalPlayerPed();
	float GetDistanceFromPoint(float x, float y, float z);

public:
	CPhysical		*m_pEntity;
	uint32_t		m_dwGTAId;

    bool GetCollisionChecking();

	void Remove();
};