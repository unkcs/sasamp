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
		CVector vec = { 0.0f,0.0f,0.0f };
		SetMoveSpeedVector(vec);
		SetTurnSpeedVector(vec);

		WorldAddEntity((uintptr_t)m_pEntity);

		RwMatrix mat;
		GetMatrix(&mat);
		TeleportTo(mat.pos.x, mat.pos.y, mat.pos.z);
	}
}

void CEntity::SetGravityProcessing(bool bProcess)
{
}

void CEntity::UpdateRwMatrixAndFrame()
{
	if (m_pEntity && !CUtil::IsGameEntityArePlaceable(m_pEntity))
	{
		if (m_pEntity->m_pRwObject)
		{
			if (m_pEntity->mat)
			{
				uintptr_t pRwMatrix = *(uintptr_t*)(m_pEntity->m_pRwObject + 4) + 0x10;
				// CMatrix::UpdateRwMatrix
				((void (*) (RwMatrix*, uintptr_t))(g_libGTASA + 0x3E862C + 1))(m_pEntity->mat, pRwMatrix);

				// CEntity::UpdateRwFrame
				((void (*) (ENTITY_TYPE*))(g_libGTASA + 0x39194C + 1))(m_pEntity);
			}
		}
	}
}

void CEntity::RemovePhysical()
{
	((void (*)(ENTITY_TYPE*))(*(void**)(m_pEntity->vtable + 16)))(m_pEntity); // CPhysical::Remove
}

void CEntity::AddPhysical()
{
	((void (*)(ENTITY_TYPE*))(*(void**)(m_pEntity->vtable + 8)))(m_pEntity); // CPhysical::Add
}

void CEntity::UpdateMatrix(RwMatrix mat)
{
	if (m_pEntity)
	{
		if (m_pEntity->mat)
		{
			// CPhysical::Remove
			((void (*)(ENTITY_TYPE*))(*(uintptr_t*)(m_pEntity->vtable + 0x10)))(m_pEntity);

			SetMatrix(mat);
			UpdateRwMatrixAndFrame();

			// CPhysical::Add
			((void (*)(ENTITY_TYPE*))(*(uintptr_t*)(m_pEntity->vtable + 0x8)))(m_pEntity);
		}
	}
}

bool CEntity::GetCollisionChecking()
{
	if(!m_pEntity || CUtil::IsGameEntityArePlaceable(m_pEntity))
		return true;

	return m_pEntity->nEntityFlags.m_bCollisionProcessed;
}

void CEntity::SetCollisionChecking(bool bCheck)
{
	if(!m_pEntity || CUtil::IsGameEntityArePlaceable(m_pEntity))
		return;

	m_pEntity->nEntityFlags.m_bCollisionProcessed = bCheck;
}

void CEntity::Render()
{
	uintptr_t pRwObject = m_pEntity->m_pRwObject;

	int iModel = GetModelIndex();
	if(iModel >= 400 && iModel <= 611 && pRwObject)
	{
		// CVisibilityPlugins::SetupVehicleVariables
		((void (*)(uintptr_t))(g_libGTASA+0x55D4EC+1))(pRwObject);
	}

	// CEntity::PreRender
	(( void (*)(ENTITY_TYPE*))(*(void**)(m_pEntity->vtable+0x48)))(m_pEntity);

	// CRenderer::RenderOneNonRoad
	(( void (*)(ENTITY_TYPE*))(g_libGTASA+0x3B1690+1))(m_pEntity);
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
	if (!m_pEntity || !m_pEntity->mat) return;

	Matrix->right.X = m_pEntity->mat->right.X;
	Matrix->right.Y = m_pEntity->mat->right.Y;
	Matrix->right.Z = m_pEntity->mat->right.Z;

	Matrix->up.X = m_pEntity->mat->up.X;
	Matrix->up.Y = m_pEntity->mat->up.Y;
	Matrix->up.Z = m_pEntity->mat->up.Z;

	Matrix->at.X = m_pEntity->mat->at.X;
	Matrix->at.Y = m_pEntity->mat->at.Y;
	Matrix->at.Z = m_pEntity->mat->at.Z;

	Matrix->pos = m_pEntity->mat->pos;
}

// 0.3.7
void CEntity::SetMatrix(RwMatrix Matrix)
{
	if (!m_pEntity) return;
	if (!m_pEntity->mat) return;

	m_pEntity->mat->right.X = Matrix.right.X;
	m_pEntity->mat->right.Y = Matrix.right.Y;
	m_pEntity->mat->right.Z = Matrix.right.Z;

	m_pEntity->mat->up.X = Matrix.up.X;
	m_pEntity->mat->up.Y = Matrix.up.Y;
	m_pEntity->mat->up.Z = Matrix.up.Z;

	m_pEntity->mat->at.X = Matrix.at.X;
	m_pEntity->mat->at.Y = Matrix.at.Y;
	m_pEntity->mat->at.Z = Matrix.at.Z;

	m_pEntity->mat->pos = Matrix.pos;
}

// 0.3.7
void CEntity::GetMoveSpeedVector(CVector *vec)
{
	if (!m_pEntity) return;
	*vec = m_pEntity->vecMoveSpeed;
}

// 0.3.7
void CEntity::SetMoveSpeedVector(CVector Vector)
{
	if (!m_pEntity) return;
	m_pEntity->vecMoveSpeed = Vector;
}

void CEntity::GetTurnSpeedVector(CVector* vec)
{
	if (!m_pEntity) return;
	*vec = m_pEntity->vecTurnSpeed;
}

void CEntity::SetTurnSpeedVector(const CVector vec)
{
	if (!m_pEntity) return;
	m_pEntity->vecTurnSpeed = vec;
}

// 0.3.7
uint16_t CEntity::GetModelIndex()
{
	if (!m_pEntity)
	{
		return 0;
	}
	return m_pEntity->nModelIndex;
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
	(( void (*)(ENTITY_TYPE*))(*(void**)(m_pEntity->vtable+0x24)))(m_pEntity);
	m_pEntity->nModelIndex = uiModel;
	// CEntity::SetModelIndex()
	(( void (*)(ENTITY_TYPE*, unsigned int))(*(void**)(m_pEntity->vtable+0x18)))(m_pEntity, uiModel);

	return true;
}

// 0.3.7
void CEntity::TeleportTo(float fX, float fY, float fZ)
{
	if(m_pEntity && m_pEntity->vtable != (g_libGTASA+0x5C7358)) /* CPlaceable */
	{
		uint16_t modelIndex = m_pEntity->nModelIndex;
		if(	modelIndex != TRAIN_PASSENGER_LOCO &&
			modelIndex != TRAIN_FREIGHT_LOCO &&
			modelIndex != TRAIN_TRAM)
			(( void (*)(ENTITY_TYPE*, float, float, float, bool))(*(void**)(m_pEntity->vtable+0x3C)))(m_pEntity, fX, fY, fZ, 0);
		else
			ScriptCommand(&put_train_at, m_dwGTAId, fX, fY, fZ);
	}
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