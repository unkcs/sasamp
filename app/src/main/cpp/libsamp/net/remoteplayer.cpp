#include "../main.h"
#include "../game/game.h"
#include "netgame.h"
#include "../chatwindow.h"

extern CGame *pGame;

CRemotePlayer::CRemotePlayer()
{
	m_PlayerID = INVALID_PLAYER_ID;
	m_VehicleID = INVALID_VEHICLE_ID;
	m_pPlayerPed = nullptr;
	m_pCurrentVehicle = nullptr;
	m_bIsNPC = false;
	m_bIsAFK = true;
	m_dwMarkerID = 0;
	m_dwGlobalMarkerID = 0;
	m_byteState = PLAYER_STATE_NONE;
	m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
	m_bShowNameTag = true;

	m_dwLastRecvTick = 0;
	m_dwUnkTime = 0;

	m_byteSpecialAction = 0;
	m_byteSeatID = 0;
	m_byteWeaponShotID = 0xFF;
	m_bKeyboardOpened = false;

	m_dwCurrentAnimation = 0;
	m_usPlayingAnimIdx = 0;
	m_bWasSettedFlag = false;
}

CRemotePlayer::~CRemotePlayer()
{
	Remove();
}
extern bool g_uiHeadMoveEnabled;
void CRemotePlayer::ProcessSpecialActions(BYTE byteSpecialAction)
{
	if (!m_pPlayerPed || !m_pPlayerPed->IsAdded()) return;

    // ����������
    if (m_pPlayerPed->IsCrouching() && byteSpecialAction != SPECIAL_ACTION_DUCK) {
        m_pPlayerPed->ResetCrouch();
    }
    if (byteSpecialAction == SPECIAL_ACTION_DUCK && !m_pPlayerPed->IsCrouching()) {
        m_pPlayerPed->ApplyCrouch();
    }
    ///

	m_pPlayerPed->ProcessSpecialAction(byteSpecialAction);

//	// headsync:always
	if (GetState() == PLAYER_STATE_ONFOOT)
	{


		//headmove?
		if ((GetTickCount() - m_dwLastHeadUpdate) > 500 && g_uiHeadMoveEnabled)
		{
			VECTOR LookAt;
			CAMERA_AIM* Aim = GameGetRemotePlayerAim(m_pPlayerPed->m_bytePlayerNumber);
			LookAt.x = Aim->pos1x + (Aim->f1x * 20.0f);
			LookAt.y = Aim->pos1y + (Aim->f1y * 20.0f);
			LookAt.z = Aim->pos1z + (Aim->f1z * 20.0f);
			ScriptCommand(&TASK_LOOK_AT_COORD, m_pPlayerPed->m_dwGTAId, LookAt.x, LookAt.y, LookAt.z, 3000);
			m_dwLastHeadUpdate = GetTickCount();
		}
	}
}

#define OUTCOMING_KEY 0x87
#define OUTCOMING_NUM 0x80
void CRemotePlayer::Process()
{
	CPlayerPool *pPool = pNetGame->GetPlayerPool();

	RwMatrix matPlayer, matVehicle;
	CVector vecMoveSpeed;

	if(IsActive())
	{
		m_bIsAFK = false;
		if((GetTickCount() - m_dwLastRecvTick) > 2000)
			m_bIsAFK = true;

		// ---- ONFOOT NETWORK PROCESSING ----
		if(GetState() == PLAYER_STATE_ONFOOT &&
			m_byteUpdateFromNetwork == UPDATE_TYPE_ONFOOT && !m_pPlayerPed->IsInVehicle())
		{
			if (GetPlayerPed()->GetCurrentWeapon() != m_ofSync.byteCurrentWeapon)
			{
				GetPlayerPed()->GiveWeapon(m_ofSync.byteCurrentWeapon, 9999);
				GetPlayerPed()->SetArmedWeapon(m_ofSync.byteCurrentWeapon);
			}
			UpdateOnFootPositionAndSpeed(&m_ofSync.vecPos, &m_ofSync.vecMoveSpeed);
			UpdateOnFootTargetPosition();
		}

		else if(GetState() == PLAYER_STATE_DRIVER &&
			m_byteUpdateFromNetwork == UPDATE_TYPE_INCAR && m_pPlayerPed->IsInVehicle())
		{
			if(!m_pCurrentVehicle || !GamePool_Vehicle_GetAt(m_pCurrentVehicle->m_dwGTAId))
				return;

			m_icSync.quat.Normalize();
			m_icSync.quat.GetMatrix(&matVehicle);
			matVehicle.pos = m_icSync.vecPos;

			UpdateInCarMatrixAndSpeed(&matVehicle, &m_icSync.vecPos, &m_icSync.vecMoveSpeed);
			UpdateInCarTargetPosition();
		}

		m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;

		// ------ PROCESSED FOR ALL FRAMES ----- 
		if(GetState() == PLAYER_STATE_ONFOOT && !m_pPlayerPed->IsInVehicle())
		{
			if ((GetTickCount() - m_dwLastHeadUpdate) > 500 /*&& pSettings->GetReadOnly().szHeadMove*/)
			{
				VECTOR LookAt;
				CAMERA_AIM* Aim = GameGetRemotePlayerAim(m_pPlayerPed->m_bytePlayerNumber);
				LookAt.x = Aim->pos1x + (Aim->f1x * 20.0f);
				LookAt.y = Aim->pos1y + (Aim->f1y * 20.0f);
				LookAt.z = Aim->pos1z + (Aim->f1z * 20.0f);
				m_pPlayerPed->ApplyCommandTask("FollowPedSA", 0, 2000, -1, &LookAt, 0, 0.1f, 500, 3, 0);
				m_dwLastHeadUpdate = GetTickCount();
			}
			ProcessSpecialActions(m_ofSync.byteSpecialAction);
			SlerpRotation();
		//	HandleAnimations();

			if (m_byteWeaponShotID != 0xFF)
			{
				//weapon sync
				//RwMatrix localMat;
				//pPool->GetLocalPlayer()->GetPlayerPed()->GetMatrix(&localMat);

				m_ofSync.byteCurrentWeapon = m_byteWeaponShotID;
				m_pPlayerPed->SetArmedWeapon((int)m_byteWeaponShotID);
				//ScriptCommand(&task_shoot_at_coord, m_pPlayerPed->m_dwGTAId, localMat.pos.x, localMat.pos.y, localMat.pos.z, 10);
				m_pPlayerPed->SetCurrentAim(pPool->GetLocalPlayer()->GetPlayerPed()->GetCurrentAim());
				m_pPlayerPed->SetKeys((uint16_t)4, m_ofSync.lrAnalog, m_ofSync.udAnalog);

				//unknown weapon
				m_byteWeaponShotID = 0xFF;
			}
			else
				m_pPlayerPed->SetKeys(m_ofSync.wKeys, m_ofSync.lrAnalog, m_ofSync.udAnalog);

			if (RemotePlayerKeys[GetPlayerPed()->m_bytePlayerNumber].bKeys[ePadKeys::KEY_HANDBRAKE])
			{
				GetPlayerPed()->SetPlayerAimState();
			}
			else
			{
				GetPlayerPed()->ClearPlayerAimState();
			}
			if( m_ofSync.vecMoveSpeed.x == 0.0f &&
				m_ofSync.vecMoveSpeed.y == 0.0f &&
				m_ofSync.vecMoveSpeed.z == 0.0f)
			{
				m_pPlayerPed->SetMoveSpeedVector(m_ofSync.vecMoveSpeed);
			}

			if(m_bIsAFK)
			{
				m_ofSync.lrAnalog = 0;
				m_ofSync.udAnalog = 0;

				vecMoveSpeed = 0.0f;

				m_pPlayerPed->SetMoveSpeedVector(vecMoveSpeed);

				m_pPlayerPed->GetMatrix(&matPlayer);
				matPlayer.pos = m_ofSync.vecPos;

				m_pPlayerPed->SetMatrix(matPlayer);
			}
		}
		if(GetState() == PLAYER_STATE_DRIVER && m_pPlayerPed->IsInVehicle())
		{
			if (!m_pCurrentVehicle)
			{
				return;
			}
			if (!GamePool_Vehicle_GetAt(m_pCurrentVehicle->m_dwGTAId))
			{
				return;
			}
			UpdateVehicleRotation();

			if(	m_icSync.vecMoveSpeed.x == 0.0f &&
				m_icSync.vecMoveSpeed.y == 0.0f &&
				m_icSync.vecMoveSpeed.z == 0.0f)
			{
				m_pCurrentVehicle->SetMoveSpeedVector(m_icSync.vecMoveSpeed);
			}

			m_pPlayerPed->SetKeys(m_icSync.wKeys, m_icSync.lrAnalog, m_icSync.udAnalog);

		}

		if(m_byteState != PLAYER_STATE_WASTED)
			m_pPlayerPed->SetHealth(1000.0f);

	}
	else
	{
		if(m_pPlayerPed)
		{
			//ResetAllSyncAttributes();
			pGame->RemovePlayer(m_pPlayerPed);
			m_pPlayerPed = nullptr;
		}
	}
}

uint32_t CRemotePlayer::GetPlayerColorAsARGB()
{
	return (TranslateColorCodeToRGBA(m_PlayerID) >> 8) | 0xFF000000;
}


void CRemotePlayer::SlerpRotation()
{
	RwMatrix mat;
	CQuaternion quatPlayer, quatResult;

	if(m_pPlayerPed)
	{
		m_pPlayerPed->GetMatrix(&mat);

		quatPlayer.SetFromMatrix(mat);

		quatResult.Slerp(&m_ofSync.quat, &quatPlayer, 0.75f);
		quatResult.GetMatrix(&mat);
		m_pPlayerPed->SetMatrix(mat);

		double fZ = atan2(-mat.up.x, mat.up.y) * 57.295776; /* rad to deg */
		if(fZ > 360.0f) fZ -= 360.0f;
		if(fZ < 0.0f) fZ += 360.0f;
		m_pPlayerPed->SetRotation((float)fZ);
	}
}


void CRemotePlayer::Remove()
{
	if(m_dwMarkerID)
	{
		pGame->DisableMarker(m_dwMarkerID);
		m_dwMarkerID = 0;
	}

	if(m_dwGlobalMarkerID)
	{
		pGame->DisableMarker(m_dwGlobalMarkerID);
		m_dwGlobalMarkerID = 0;
	}

	if(m_pPlayerPed)
	{
		pGame->RemovePlayer(m_pPlayerPed);
		m_pPlayerPed = nullptr;
	}
}

void CRemotePlayer::UpdateInCarMatrixAndSpeed(RwMatrix* mat, CVector* pos, CVector* speed)
{
	m_InCarQuaternion.SetFromMatrix(*mat);

	m_vecInCarTargetPos = *pos;
	m_vecInCarTargetSpeed = *speed;

	m_pCurrentVehicle->SetMoveSpeedVector(*speed);
}

void CRemotePlayer::UpdateInCarTargetPosition()
{
	RwMatrix matEnt;
	CVector vec = { 0.0f, 0.0f, 0.0f };

	float delta = 0.0f;

	if(!m_pCurrentVehicle) return;

	m_pCurrentVehicle->GetMatrix(&matEnt);

	if(m_pCurrentVehicle->IsAdded())
	{
		m_vecPosOffset.x = FloatOffset(m_vecInCarTargetPos.x, matEnt.pos.x);
		m_vecPosOffset.y = FloatOffset(m_vecInCarTargetPos.y, matEnt.pos.y);
		m_vecPosOffset.z = FloatOffset(m_vecInCarTargetPos.z, matEnt.pos.z);

		if(m_vecPosOffset.x > 0.05f || m_vecPosOffset.y > 0.05f || m_vecPosOffset.z > 0.05f)
		{
			delta = 0.5f;
			if( m_pCurrentVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_BOAT ||
				m_pCurrentVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_PLANE ||
				m_pCurrentVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_HELI)
			{
				delta = 2.0f;
			}

			if(m_vecPosOffset.x > 8.0f || m_vecPosOffset.y > 8.0f || m_vecPosOffset.z > delta)
			{
				matEnt.pos = m_vecInCarTargetPos;

				m_pCurrentVehicle->SetMatrix(matEnt);
				m_pCurrentVehicle->SetMoveSpeedVector(m_vecInCarTargetSpeed);
			}
			else
			{
				m_pCurrentVehicle->GetMoveSpeedVector(&vec);
				if(m_vecPosOffset.x > 0.05f)
					vec.x += (m_vecInCarTargetPos.x - matEnt.pos.x) * 0.06f;
				if(m_vecPosOffset.y > 0.05f)
					vec.y += (m_vecInCarTargetPos.y - matEnt.pos.y) * 0.06f;
				if(m_vecPosOffset.z > 0.05f)
					vec.z += (m_vecInCarTargetPos.z - matEnt.pos.z) * 0.06f;

				if( FloatOffset(vec.x, 0.0f) > 0.01f ||
					FloatOffset(vec.y, 0.0f) > 0.01f ||
					FloatOffset(vec.z, 0.0f) > 0.01f)
				{
					m_pCurrentVehicle->SetMoveSpeedVector(vec);
				}
			}
		}
	}
	else
	{
		matEnt.pos = m_vecInCarTargetPos;

		m_pCurrentVehicle->SetMatrix(matEnt);
	}
}

void CRemotePlayer::UpdateVehicleRotation()
{
	CQuaternion quat, qresult;
	RwMatrix matEnt;
	CVector vec = { 0.0f, 0.0f, 0.0f };


	if(!m_pCurrentVehicle) return;

	m_pCurrentVehicle->GetTurnSpeedVector(&vec);
	if(vec.x <= 0.02f)
	{
		if(vec.x < -0.02f) vec.x = -0.02f;
	}
	else vec.x = 0.02f;

	if(vec.y <= 0.02f)
	{
		if(vec.y < -0.02f) vec.y = -0.02f;
	}
	else vec.y = 0.02f;

	if(vec.z <= 0.02f)
	{
		if(vec.z < -0.02f) vec.z = -0.02f;
	}
	else vec.z = 0.02f;

	m_pCurrentVehicle->SetTurnSpeedVector(vec);

	m_pCurrentVehicle->GetMatrix(&matEnt);
	quat.SetFromMatrix(matEnt);
	qresult.Slerp(&m_InCarQuaternion, &quat, 0.75f);
	qresult.Normalize();
	qresult.GetMatrix(&matEnt);
	m_pCurrentVehicle->SetMatrix(matEnt);
}

bool CRemotePlayer::Spawn(uint8_t byteTeam, unsigned int iSkin, VECTOR *vecPos, float fRotation, 
	uint32_t dwColor, uint8_t byteFightingStyle, bool bVisible)
{
	if(m_pPlayerPed)
	{
		pGame->RemovePlayer(m_pPlayerPed);
		m_pPlayerPed = nullptr;
	}

	CPlayerPed *pPlayer = pGame->NewPlayer(iSkin, vecPos->x, vecPos->y, vecPos->z, fRotation);

	if(pPlayer)
	{
		if(dwColor != 0) SetPlayerColor(dwColor);

		if(m_dwMarkerID)
		{
			pGame->DisableMarker(m_dwMarkerID);
			m_dwMarkerID = 0;
		}

		if(pNetGame->m_iShowPlayerMarkers) 
			pPlayer->ShowMarker(m_PlayerID);

		m_pPlayerPed = pPlayer;

		if(byteFightingStyle != 4)
			m_pPlayerPed->SetFightingStyle(byteFightingStyle);

		if(byteTeam != ANIM_GROUP_NONE && byteTeam != 255)
			m_pPlayerPed->SetMoveAnim(byteTeam);

		SetState(PLAYER_STATE_SPAWNED);
		return true;
	}

	SetState(PLAYER_STATE_NONE);
	return false;
}

void CRemotePlayer::EnterVehicle(VEHICLEID VehicleID, bool bPassenger)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if( m_pPlayerPed &&
		pVehiclePool->GetAt(VehicleID) &&
		!m_pPlayerPed->IsInVehicle())
	{
		int iGtaVehicleID = pVehiclePool->FindGtaIDFromID(VehicleID);
		if(iGtaVehicleID && iGtaVehicleID != INVALID_VEHICLE_ID)
		{
			m_pPlayerPed->SetKeys(0, 0, 0);
			m_pPlayerPed->EnterVehicle(iGtaVehicleID, bPassenger);
		}
	}
}

void CRemotePlayer::ExitVehicle()
{
	if(m_pPlayerPed && m_pPlayerPed->IsInVehicle())
	{
		m_pPlayerPed->SetKeys(0, 0, 0);
		m_pPlayerPed->ExitCurrentVehicle();
	}
}

void CRemotePlayer::UpdateOnFootPositionAndSpeed(CVector* vecPos, CVector* vecMove)
{
	if(!m_pPlayerPed)return;
	m_vecOnFootTargetPos = *vecPos;
	m_vecOnFootTargetSpeed = *vecMove;

	m_pPlayerPed->SetMoveSpeedVector(m_vecOnFootTargetSpeed);
}

void CRemotePlayer::StoreTrailerFullSyncData(TRAILER_SYNC_DATA* trSync)
{
	VEHICLEID TrailerID = trSync->trailerID;
	if (!TrailerID) return;

	CVehicle* pVehicle = pNetGame->GetVehiclePool()->GetAt(TrailerID);

	if (pVehicle)
	{
		RwMatrix matWorld;

		trSync->quat.GetMatrix(&matWorld);
		matWorld.pos = trSync->vecPos;

		pVehicle->SetMatrix(matWorld);
		pVehicle->SetMoveSpeedVector(trSync->vecMoveSpeed);
		pVehicle->SetTurnSpeedVector(trSync->vecTurnSpeed);
	}
}

void CRemotePlayer::UpdateOnFootTargetPosition()
{
	RwMatrix mat;
	CVector vec;

	if(!m_pPlayerPed) return;
	m_pPlayerPed->GetMatrix(&mat);

	if(!m_pPlayerPed->IsAdded())
	{
		mat.pos = m_vecOnFootTargetPos;

		m_pPlayerPed->SetMatrix(mat);
		return;
	}

	m_vecPosOffset.x = FloatOffset(m_vecOnFootTargetPos.x, mat.pos.x);
	m_vecPosOffset.y = FloatOffset(m_vecOnFootTargetPos.y, mat.pos.y);
	m_vecPosOffset.z = FloatOffset(m_vecOnFootTargetPos.z, mat.pos.z);

	if(m_vecPosOffset.x > 0.00001f || m_vecPosOffset.y > 0.00001f || m_vecPosOffset.z > 0.00001f)
	{
		if(m_vecPosOffset.x > 2.0f || m_vecPosOffset.y > 2.0f || m_vecPosOffset.z > 1.0f)
		{
			mat.pos = m_vecOnFootTargetPos;
			m_pPlayerPed->SetMatrix(mat);
			return;
		}

		m_pPlayerPed->GetMoveSpeedVector(&vec);
		if(m_vecPosOffset.x > 0.00001f)
			vec.x += (m_vecOnFootTargetPos.x - mat.pos.x) * 0.1f;
		if(m_vecPosOffset.y > 0.00001f)
			vec.y += (m_vecOnFootTargetPos.y - mat.pos.y) * 0.1f;
		if(m_vecPosOffset.z > 0.00001f)
			vec.z += (m_vecOnFootTargetPos.z - mat.pos.z) * 0.1f;

		m_pPlayerPed->SetMoveSpeedVector(vec);
	}
}
float                    m_fWeaponDamages[43 + 1]
= {
5.0f, /* Fist */
5.0f, /* Brass Knuckles */
5.0f, /* Golf Club */
5.0f, /* Nightstick */
5.0f, /* Knife */
5.0f, /* Baseball Bat */
5.0f, /* Shovel */
5.0f, /* Pool Cue */
5.0f, /* Katana */
5.0f, /* Chainsaw */
5.0f, /* Purple Dildo */
5.0f, /* Dildo */
5.0f, /* Vibrator */
5.0f, /* Silver Vibrator */
5.0f, /* Flowers */
5.0f, /* Cane */
75.0f, /* Grenade */
0.0f, /* Tear Gas */
75.0f, /* Molotov Cocktail */
0.0f, /* +EMPTY+ */
0.0f, /* +EMPTY+ */
0.0f, /* +EMPTY+ */
25.0f, /* 9mm (pistol) */
10.0f, /* Silenced 9mm */
45.0f, /* Desert Eagle */
40.0f, /* Shotgun */
10.0f, /* Sawnoff Shotgun */
10.0f, /* Combat Shotgun */
10.0f, /* Micro SMG/Uzi */
10.0f, /* MP5 */
10.0f, /* AK-47 */
10.0f, /* M4 */
20.0f, /* Tec-9 */
45.0f, /* Country Rifle */
45.0f, /* Sniper Rifle */
75.0f, /* RPG */
75.0f, /* HS Rocket */
5.0f, /* Flamethrower */
70.0f, /* Minigun */
75.0f, /* Satchel Charge */
0.0f, /* Detonator */
1.0f, /* Spraycan */
5.0f, /* Fire Extinguisher */
0.0f /* Camera */
};
void CRemotePlayer::StoreBulletSyncData(BULLET_SYNC* pBulletSync)
{
	if(!m_pPlayerPed || !m_pPlayerPed->IsAdded()) return;

	BULLET_DATA btData;
	memset(&btData, 0, sizeof(BULLET_DATA));

	btData.vecOrigin.x = pBulletSync->vecOrigin.x;
	btData.vecOrigin.y = pBulletSync->vecOrigin.y;
	btData.vecOrigin.z = pBulletSync->vecOrigin.z;

	btData.vecPos.x = pBulletSync->vecPos.x;
	btData.vecPos.y = pBulletSync->vecPos.y;
	btData.vecPos.z = pBulletSync->vecPos.z;

	btData.vecOffset.x = pBulletSync->vecOffset.x;
	btData.vecOffset.y = pBulletSync->vecOffset.y;
	btData.vecOffset.z = pBulletSync->vecOffset.z;

	if(pBulletSync->byteHitType != 0)
	{

		if(pBulletSync->byteHitType == 1)
		{
			CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
			if(pPlayerPool)
			{
				if(pBulletSync->PlayerID == pPlayerPool->GetLocalPlayerID())
				{
					btData.pEntity = pGame->FindPlayerPed()->m_pPed;
				}
				else if(pBulletSync->PlayerID == m_PlayerID)
				{
					return;
				}
				else
				{
					CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(pBulletSync->PlayerID);
					if(pRemotePlayer)
					{
						CPlayerPed *pPlayerPed = pRemotePlayer->GetPlayerPed();
						if(pPlayerPed)
							btData.pEntity = pPlayerPed->m_pPed;
					}
				}
			}
		}
		else if(pBulletSync->byteHitType == 2)
		{
			CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
			if(pVehiclePool)
			{
				CVehicle *pVehicle = pVehiclePool->GetAt(pBulletSync->PlayerID);
				if(pVehicle)
				{
					btData.pEntity = pVehicle->m_pVehicle;
				}
			}
		}
	}

	if(m_pPlayerPed->IsAdded())
	{
		uint8_t byteWeapon = pBulletSync->byteWeaponID;
		if(m_pPlayerPed->GetCurrentWeapon() != byteWeapon)
		{
			m_pPlayerPed->SetArmedWeapon(byteWeapon);
			if(m_pPlayerPed->GetCurrentWeapon() != byteWeapon)
			{
				m_pPlayerPed->GiveWeapon(byteWeapon, 9999);
				m_pPlayerPed->SetArmedWeapon(byteWeapon);
			}
		}
	}

	m_byteWeaponShotID = pBulletSync->byteWeaponID;

	m_pPlayerPed->ProcessBulletData(&btData);
	m_pPlayerPed->FireInstant();
}

void CRemotePlayer::SetPlayerColor(uint32_t dwColor)
{
	SetRadarColor(m_PlayerID, dwColor);
}

void CRemotePlayer::Say(unsigned char* szText)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if (pPlayerPool) 
	{
		char * szPlayerName = pPlayerPool->GetPlayerName(m_PlayerID);
		CChatWindow::AddChatMessage(szPlayerName,GetPlayerColor(), (char*)szText);
	}
}

void calculateAimVector(VECTOR* vec1, VECTOR* vec2)
{
	float f1;
	float f2;
	float f3;

	f1 = atan2(vec1->x, vec1->y) - 1.570796370506287;
	f2 = sin(f1);
	f3 = cos(f1);
	vec2->x = vec1->y * 0.0 - f3 * vec1->z;
	vec2->y = f2 * vec1->z - vec1->x * 0.0;
	vec2->z = f3 * vec1->x - f2 * vec1->y;
}

void CRemotePlayer::UpdateAimFromSyncData(AIM_SYNC_DATA * pAimSync)
{
	if (!m_pPlayerPed) return;
	m_pPlayerPed->SetCameraMode(pAimSync->byteCamMode);

	CAMERA_AIM Aim;

	Aim.f1x = pAimSync->vecAimf.x;
	Aim.f1y = pAimSync->vecAimf.y;
	Aim.f1z = pAimSync->vecAimf.z;
	Aim.pos1x = pAimSync->vecAimPos.x;
	Aim.pos1y = pAimSync->vecAimPos.y;
	Aim.pos1z = pAimSync->vecAimPos.z;
	Aim.pos2x = pAimSync->vecAimPos.x;
	Aim.pos2y = pAimSync->vecAimPos.y;
	Aim.pos2z = pAimSync->vecAimPos.z;

	VECTOR vec1;
	vec1.x = Aim.f1x;
	vec1.y = Aim.f1y;
	vec1.z = Aim.f1z;

	VECTOR vec2;
	vec2.x = 0.0f;
	vec2.y = 0.0f;
	vec2.z = 0.0f;

	calculateAimVector(&vec1, &vec2);

	Aim.f2x = vec2.x;
	Aim.f2y = vec2.y;
	Aim.f2z = vec2.z;

	m_pPlayerPed->SetCurrentAim(&Aim);
	m_pPlayerPed->SetAimZ(pAimSync->fAimZ);

	float fAspect = pAimSync->aspect_ratio * 0.0039215689f;
	float fExtZoom = (pAimSync->byteCamExtZoom) * 0.015873017f;

	m_pPlayerPed->SetCameraExtendedZoom(fExtZoom, fAspect);

	CWeapon* pwstWeapon = m_pPlayerPed->GetCurrentWeaponSlot();
	if (pAimSync->byteWeaponState == WS_RELOADING)
		pwstWeapon->dwState = 2;		// Reloading
	else
	if (pAimSync->byteWeaponState != WS_MORE_BULLETS)
		pwstWeapon->dwAmmoInClip = (uint32_t)pAimSync->byteWeaponState;
	else
	if (pwstWeapon->dwAmmoInClip < 2)
		pwstWeapon->dwAmmoInClip = 2;
}

void CRemotePlayer::StoreOnFootFullSyncData(ONFOOT_SYNC_DATA *pofSync, uint32_t dwTime)
{
	memcpy(&m_ofSync, pofSync, sizeof(ONFOOT_SYNC_DATA));

	m_byteSpecialAction = pofSync->byteSpecialAction;
	m_byteUpdateFromNetwork = UPDATE_TYPE_ONFOOT;

	if(m_pPlayerPed)
	{
		m_fCurrentHealth = pofSync->byteHealth;
		m_fCurrentArmor = pofSync->byteArmour;

		if(m_pPlayerPed->IsInVehicle())
		{
			RwMatrix mat;
			m_pPlayerPed->GetMatrix(&mat);
			m_pPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.x, mat.pos.y, mat.pos.z);
		}
	}
	m_dwLastRecvTick = GetTickCount();
	SetState(PLAYER_STATE_ONFOOT);
}
void CRemotePlayer::StoreInCarFullSyncData(INCAR_SYNC_DATA *picSync, uint32_t dwTime)
{
	if(!m_pPlayerPed)return;

	memcpy(&m_icSync, picSync, sizeof(INCAR_SYNC_DATA));
	m_VehicleID = picSync->VehicleID;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if (!pVehiclePool) return;
	if (!pVehiclePool->GetSlotState(m_VehicleID)) return;

	m_pCurrentVehicle = pVehiclePool->GetAt(m_VehicleID);
	if (!m_pCurrentVehicle)return;

	if(!m_pPlayerPed->IsInVehicle())
	{
		m_pPlayerPed->PutDirectlyInVehicle(m_pCurrentVehicle, 0);
	}
	else if (m_pPlayerPed->GetCurrentVehicle() != m_pCurrentVehicle) {
        RemoveFromVehicle();
	}

	// -------------- TRAILER
	if(m_icSync.TrailerID  == INVALID_VEHICLE_ID || m_icSync.TrailerID  == 0 )
	{//
		if(m_pCurrentVehicle->m_pTrailer) {
			m_pCurrentVehicle->DetachTrailer();
		}
	}
	else {
		CVehicle *pTrailer = pVehiclePool->GetAt(m_icSync.TrailerID);
		if(pTrailer) {
			if (m_pCurrentVehicle->m_pTrailer) {
				if ((CVehicleGta*)(m_pCurrentVehicle->m_pVehicle->dwTrailer) != pTrailer->m_pVehicle) {
					m_pCurrentVehicle->DetachTrailer();
				}
			} else {
				if (GamePool_Vehicle_GetAt(pTrailer->m_dwGTAId)) {
					m_pCurrentVehicle->SetTrailer(pTrailer);
					m_pCurrentVehicle->AttachTrailer();
				}
			}
		}
	}
	// -----------------------------------------------------------

	m_byteSeatID = 0;
	m_fCurrentHealth = picSync->bytePlayerHealth;
	m_fCurrentArmor = picSync->bytePlayerArmour;

	m_byteUpdateFromNetwork = UPDATE_TYPE_INCAR;
	m_dwLastRecvTick = GetTickCount();

	m_byteSpecialAction = 0;
	m_pCurrentVehicle->SetHealth(picSync->fCarHealth);
	m_pCurrentVehicle->m_bIsSirenOn = picSync->byteSirenOn;

	m_pCurrentVehicle->m_iTurnState = static_cast<eTurnState>(picSync->HydraThrustAngle);

	SetState(PLAYER_STATE_DRIVER);
}

void CRemotePlayer::StorePassengerFullSyncData(PASSENGER_SYNC_DATA *ppsSync)
{
	if(!m_pPlayerPed) return;

	memcpy(&m_psSync, ppsSync, sizeof(PASSENGER_SYNC_DATA));
	m_VehicleID = ppsSync->VehicleID;

	auto pVehiclePool = pNetGame->GetVehiclePool();

	if (!pVehiclePool->GetSlotState(m_VehicleID))
        return;

	m_byteSeatID = ppsSync->byteSeatFlags;
	m_pCurrentVehicle = pVehiclePool->GetAt(m_VehicleID);

	if(!m_pCurrentVehicle)return;
	if(!m_pPlayerPed->IsInVehicle()){
		m_pPlayerPed->PutDirectlyInVehicle(m_pCurrentVehicle, m_byteSeatID);
	}
	else if (m_pPlayerPed->GetCurrentVehicle() != m_pCurrentVehicle) {
        RemoveFromVehicle();
	}

	m_fCurrentHealth = ppsSync->bytePlayerHealth;
	m_fCurrentArmor = ppsSync->bytePlayerArmour;

	m_byteUpdateFromNetwork = UPDATE_TYPE_PASSENGER;
	m_dwLastRecvTick = GetTickCount();

	SetState(PLAYER_STATE_PASSENGER);
}

void CRemotePlayer::RemoveFromVehicle()
{
	RwMatrix mat;

	if(m_pPlayerPed)
	{
		if(m_pPlayerPed->IsInVehicle())
		{
			m_pPlayerPed->GetMatrix(&mat);
			m_pPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.x, mat.pos.y, mat.pos.z+1.0);
		}
	}
}

void CRemotePlayer::HandleDeath()
{
	if (GetPlayerPed())
	{
		GetPlayerPed()->SetKeys(0, 0, 0);
		GetPlayerPed()->SetDead();
		m_fCurrentHealth = 0;
	}

	SetState(PLAYER_STATE_WASTED);

	memset(&m_ofSync, 0, sizeof(m_ofSync));
	memset(&m_icSync, 0, sizeof(m_icSync));
	memset(&m_psSync, 0, sizeof(m_psSync));

	HideGlobalMarker();
}

void CRemotePlayer::HandleAnimations()
{
	if (!m_pPlayerPed)
	{
		return;
	}

	int flag = m_ofSync.dwAnimation >> 31;
	//pChatWindow->AddDebugMessage("FLAG REMOTE %d", flag);

	if (!flag)
	{
		uint16_t newAnim = (uint16_t)m_ofSync.dwAnimation;
		if (newAnim != m_usPlayingAnimIdx)
		{
			//pChatWindow->AddDebugMessage("applying %hu anium", newAnim);
			m_usPlayingAnimIdx = newAnim;
			m_pPlayerPed->PlayAnimByIdx(newAnim, 4.0f);
		}
		if (m_usPlayingAnimIdx)
		{
			if (!m_pPlayerPed->IsPlayingAnim(m_usPlayingAnimIdx))
			{
				m_pPlayerPed->PlayAnimByIdx(newAnim, 4.0f);
			}
		}
		m_bWasSettedFlag = true;
	}
	if (flag && m_bWasSettedFlag)
	{
		m_usPlayingAnimIdx = 0;
		// we have to reset anim
		m_pPlayerPed->PlayAnimByIdx(0, 4.0f);
		m_bWasSettedFlag = false;
	}
}


uint32_t CRemotePlayer::GetPlayerColor()
{
	return TranslateColorCodeToRGBA(m_PlayerID);
}

void CRemotePlayer::ShowGlobalMarker(short sX, short sY, short sZ)
{
	if(m_dwGlobalMarkerID)
	{
		pGame->DisableMarker(m_dwGlobalMarkerID);
		m_dwGlobalMarkerID = 0;
	}

	if(!m_pPlayerPed)
	{
		m_dwGlobalMarkerID = pGame->CreateRadarMarkerIcon(0, (float)sX, (float)sY, (float)sZ, m_PlayerID, 0);
	}
}

void CRemotePlayer::HideGlobalMarker()
{
	if(m_dwGlobalMarkerID)
	{
		pGame->DisableMarker(m_dwGlobalMarkerID);
		m_dwGlobalMarkerID = 0;
	}
}


void CRemotePlayer::StateChange(BYTE byteNewState, BYTE byteOldState)
{
	if(byteNewState == PLAYER_STATE_DRIVER && byteOldState == PLAYER_STATE_ONFOOT)
	{
		// If their new vehicle is the one the local player
		// is driving, we'll have to kick the local player out
		CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();
		VEHICLEID LocalVehicle=0xFFFF;
		RwMatrix mat;

		if(pLocalPlayerPed && pLocalPlayerPed->IsInVehicle() && !pLocalPlayerPed->IsAPassenger())
		{
			LocalVehicle = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(pLocalPlayerPed->GetGtaVehicle());
			if(LocalVehicle == m_VehicleID) {
				pLocalPlayerPed->GetMatrix(&mat);
				pLocalPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.x,mat.pos.y,mat.pos.z + 1.0f);
				pGame->DisplayGameText("~r~Car Jacked~w~!",1000,5);
			}
		}
	}
}