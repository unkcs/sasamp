#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "../chatwindow.h"

#include <cmath>

extern CGame *pGame;
extern CNetGame *pNetGame;

void CEntity::Add()
{
	if (!m_pEntity || CUtil::IsGameEntityArePlaceable(m_pEntity))
	{
		return;
	}

	if (!m_pEntity->m_pMovingList)
	{
		m_pEntity->ResetMoveSpeed();

		WorldAddEntity((uintptr_t)m_pEntity);

		RwMatrix mat;
		GetMatrix(&mat);
		m_pEntity->SetPosn(mat.pos.x, mat.pos.y, mat.pos.z);
	}
}


void CEntity::UpdateRwMatrixAndFrame()
{
	if (m_pEntity && !CUtil::IsGameEntityArePlaceable(m_pEntity))
	{
		if (m_pEntity->m_pRwObject)
		{
			auto parent = (char *)m_pEntity->m_pRwObject->parent;
			auto m_pMat = m_pEntity->m_matrix;

			auto pMatrix = (RwMatrix *)(parent + 0x10);

			if (m_pMat)
				m_pMat->UpdateRwMatrix(pMatrix);
			else
				m_pEntity->m_placement.UpdateRwMatrix(pMatrix);


			m_pEntity->UpdateRwFrame();
		}
	}
}

void CEntity::UpdateMatrix(RwMatrix mat)
{
	if (m_pEntity)
	{
		if (m_pEntity->m_matrix)
		{
			// CPhysical::Remove
			((void (*)(CEntityGta*))(*(uintptr_t*)(m_pEntity->vtable + 0x10)))(m_pEntity);

			SetMatrix(mat);
			UpdateRwMatrixAndFrame();

			// CPhysical::Add
			((void (*)(CEntityGta*))(*(uintptr_t*)(m_pEntity->vtable + 0x8)))(m_pEntity);
		}
	}
}

bool CEntity::GetCollisionChecking()
{
	if(!m_pEntity || CUtil::IsGameEntityArePlaceable(m_pEntity))
		return true;

	return m_pEntity->m_bCollisionProcessed;
}

void CEntity::Render()
{
	auto pRwObject = m_pEntity->m_pRwObject;

	int iModel = GetModelIndex();
	if(iModel >= 400 && iModel <= 611 && pRwObject)
	{
		// CVisibilityPlugins::SetupVehicleVariables
		((void (*)(RwObject*))(g_libGTASA+0x55D4EC+1))(pRwObject);
	}

	// CEntity::PreRender
	(( void (*)(CEntityGta*))(*(void**)(m_pEntity->vtable + 0x48)))(m_pEntity);

	// CRenderer::RenderOneNonRoad
	(( void (*)(CEntityGta*))(g_libGTASA + 0x3B1690 + 1))(m_pEntity);
}

void CEntity::Remove()
{
    if(!m_pEntity || CUtil::IsGameEntityArePlaceable(m_pEntity) || !m_pEntity->m_pMovingList)
        return;

    CUtil::WorldRemoveEntity((uintptr_t)m_pEntity);
}

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

void CEntity::GetTurnSpeedVector(CVector* vec)
{
	if (!m_pEntity) return;
	*vec = m_pEntity->m_vecTurnSpeed;
}

void CEntity::SetTurnSpeedVector(const CVector vec)
{
	if (!m_pEntity) return;
	m_pEntity->m_vecTurnSpeed = vec;
}

// 0.3.7
uint16_t CEntity::GetModelIndex()
{
	if (!m_pEntity)
	{
		return 0;
	}
	return m_pEntity->m_nModelIndex;
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

float CEntity::GetDistanceFromCamera()
{
	RwMatrix matEnt;

	if(!m_pEntity || CUtil::IsGameEntityArePlaceable(m_pEntity))
		return 100000.0f;

	this->GetMatrix(&matEnt);
	
	float tmpX = (matEnt.pos.x - *(float*)(g_libGTASA+0x8B1134));
	float tmpY = (matEnt.pos.y - *(float*)(g_libGTASA+0x8B1138));
	float tmpZ = (matEnt.pos.z - *(float*)(g_libGTASA+0x8B113C));

	return sqrt( tmpX*tmpX + tmpY*tmpY + tmpZ*tmpZ );
}

float CEntity::GetDistanceFromLocalPlayerPed()
{
	RwMatrix	matFromPlayer;
	RwMatrix	matThis;
	float 		fSX, fSY, fSZ;

	CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();
	CLocalPlayer *pLocalPlayer  = nullptr;

	if(!pLocalPlayerPed) return 10000.0f;

	GetMatrix(&matThis);

	if(pNetGame)
	{
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		if(pLocalPlayer && (pLocalPlayer->IsSpectating() || pLocalPlayer->IsInRCMode()))
		{
			pGame->GetCamera()->GetMatrix(&matFromPlayer);
		}
		else
		{
			pLocalPlayerPed->GetMatrix(&matFromPlayer);
		}
	}
	else
	{
		pLocalPlayerPed->GetMatrix(&matFromPlayer);
	}

	fSX = (matThis.pos.x - matFromPlayer.pos.x) * (matThis.pos.x - matFromPlayer.pos.x);
	fSY = (matThis.pos.y - matFromPlayer.pos.y) * (matThis.pos.y - matFromPlayer.pos.y);
	fSZ = (matThis.pos.z - matFromPlayer.pos.z) * (matThis.pos.z - matFromPlayer.pos.z);

	return (float)sqrt(fSX + fSY + fSZ);
}

float CEntity::GetDistanceFromPoint(float X, float Y, float Z)
{
	RwMatrix	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	fSX = (matThis.pos.x - X) * (matThis.pos.x - X);
	fSY = (matThis.pos.y - Y) * (matThis.pos.y - Y);
	fSZ = (matThis.pos.z - Z) * (matThis.pos.z - Z);
	
	return (float)sqrt(fSX + fSY + fSZ);
}