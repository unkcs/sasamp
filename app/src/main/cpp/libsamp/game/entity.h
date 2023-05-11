#pragma once

#include "game/Core/Vector.h"
#include "Entity/Physical.h"
class CEntity
{
public:
	CEntity() {};
	virtual ~CEntity() {};

	// 0.3.7
	void GetMatrix(RwMatrix* Matrix);
	// 0.3.7
	void SetMatrix(RwMatrix mat);

	bool SetModelIndex(unsigned int uiModel);
	// 0.3.7
	bool IsAdded();

public:
	CPhysical		*m_pEntity;
	uint32_t		m_dwGTAId;
};