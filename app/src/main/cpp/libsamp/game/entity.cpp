#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "../chatwindow.h"
#include "util/util.h"

#include <cmath>

extern CGame *pGame;
extern CNetGame *pNetGame;

// 0.3.7
void CEntity::GetMatrix(RwMatrix* Matrix)
{
	if (!m_pEntity || !m_pEntity->m_matrix) return;

	*Matrix = m_pEntity->m_matrix->ToRwMatrix();
}

// 0.3.7
void CEntity::SetMatrix(RwMatrix mat)
{
	if (!m_pEntity) return;
	if (!m_pEntity->m_matrix) return;

	m_pEntity->m_matrix->m_right = mat.right;

	m_pEntity->m_matrix->m_forward = mat.up;

	m_pEntity->m_matrix->m_up = mat.at;

	m_pEntity->m_matrix->m_pos = mat.pos;
}

// 0.3.7
bool CEntity::IsAdded()
{
	if(m_pEntity)
	{
		if(m_pEntity->vtable == g_libGTASA+0x5C7358) // CPlaceable
			return false;

		if(m_pEntity->m_pMovingList)
			return true;
	}

	return false;
}

// 0.3.7
bool CEntity::SetModelIndex(unsigned int uiModel)
{
	if(!m_pEntity) return false;

	int iTryCount = 0;
	if(!pGame->IsModelLoaded(uiModel) && !IsValidModel(uiModel))
	{
		pGame->RequestModel(uiModel);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(uiModel))
		{
			usleep(1000);
			if(iTryCount > 200)
			{
				CChatWindow::AddDebugMessage("Warning: Model %u wouldn't load in time!", uiModel);
				return false;
			}

			iTryCount++;
		}
	}

	// CEntity::DeleteRWObject()
	(( void (*)(CEntityGta*))(*(void**)(m_pEntity->vtable + 0x24)))(m_pEntity);
	m_pEntity->m_nModelIndex = uiModel;
	// CEntity::SetModelIndex()
	(( void (*)(CEntityGta*, unsigned int))(*(void**)(m_pEntity->vtable + 0x18)))(m_pEntity, uiModel);

	return true;
}