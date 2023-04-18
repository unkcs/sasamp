#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

extern CGame* pGame;

CStreamPool::CStreamPool() // ready
{
	m_bIsDeactivated = false;
	for (int i = 0; i < MAX_STREAMS; i++)
	{
		m_pStreams[i] = nullptr;
		m_bSlotState[i] = false;
	}
	m_hIndividualStream = NULL;
	bShutdownThread = false;
	m_bWasPaused = false;
	m_bIndividualNeedReplay = 0;
	m_szIndividualLastLink[0] = 0;
	pThread = new std::thread([this]
		{
			while (!bShutdownThread)
			{
				Process();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	);

	addStaticStream(2702.97, - 1755.96, 23.17, -1, 55.0f, "http://files.liverussia.online/sounds/lesopilka_winter.mp3");
	addStaticStream(-766.1666,-2751.3713,-21.8225, 0, 200.0f, "http://files.liverussia.online/sounds/zavod.mp3");
	addStaticStream(1803.4633,1748.1989,15.5142, 0, 75.0f, "http://files.liverussia.online/sounds/army.mp3");
	addStaticStream(-2730.0898,-1761.5973,2.3055, -1, 35.0f, "http://files.liverussia.online/sounds/hotel_sound.mp3");
	addStaticStream(-903.3563,-2041.9608,25.7742, -1, 55.0f, "http://files.liverussia.online/sounds/univermag.mp3");
	addStaticStream(-784.0, 698.0, 13.10, 0, 200.0f, "http://files.liverussia.online/sounds/oil_zavod.mp3");
}

CStreamPool::~CStreamPool() // ready
{
	for (int i = 0; i < MAX_STREAMS; i++)
	{
		DeleteStreamByID(i);
	}
	StopIndividualStream();
	bShutdownThread = true;
	pThread->join();
	delete pThread;
}

void CStreamPool::Deactivate()
{
	BUFFERED_COMMAND_STREAMPOOL* bc;
	bc = bufferedCommands.WriteLock();

	bc->iID = 1;

	m_bIsDeactivated = true;

	bc->command = BUFFERED_COMMAND_STREAMPOOL::BC_ACTIVATE;

	bufferedCommands.WriteUnlock();
}

void CStreamPool::Activate()
{
	BUFFERED_COMMAND_STREAMPOOL* bc;
	bc = bufferedCommands.WriteLock();

	bc->iID = 2;

	m_bIsDeactivated = false;

	bc->command = BUFFERED_COMMAND_STREAMPOOL::BC_ACTIVATE;

	bufferedCommands.WriteUnlock();
}

CStream* CStreamPool::GetStream(int iID)
{
	if (iID > 0 && iID <= MAX_STREAMS)
	{
		return m_pStreams[iID];
	}
	return nullptr;
}

void CStreamPool::addStaticStream(float x, float y, float z, int interior, float dist, const char* url) {
	CVector vec{x, y, z};
	m_pStaticStream.push_back( new CStream(&vec, interior, dist, url) );
}

CStream* CStreamPool::AddStream(int iID, CVector* pPos, int iVirtualWorld, int iInterior, float fDistance, const char* szUrl) // ready
{
	if (iID < 0 || iID >= MAX_STREAMS) return nullptr;
	
	CStream* pStream = new CStream(pPos, iInterior, fDistance, szUrl);

	if (m_bIsDeactivated)
	{
		pStream->SetIsDeactivated(true);
	}

	BUFFERED_COMMAND_STREAMPOOL* bc;
	bc = bufferedCommands.WriteLock();

	bc->iID = iID;
	bc->pData = (const void*)pStream;

	bc->command = BUFFERED_COMMAND_STREAMPOOL::BC_ADDSTREAM;

	bufferedCommands.WriteUnlock();
	return nullptr;
}

void CStreamPool::DeleteStreamByID(int iID) // ready
{
	if (iID < 0 || iID >= MAX_STREAMS) return;

	BUFFERED_COMMAND_STREAMPOOL* bc;
	bc = bufferedCommands.WriteLock();

	bc->iID = iID;
	bc->pData = nullptr;

	bc->command = BUFFERED_COMMAND_STREAMPOOL::BC_DELETSTREAM;

	bufferedCommands.WriteUnlock();
}

void CStreamPool::PlayIndividualStream(const char* szUrl, int type) // ready
{
	if (m_hIndividualStream)
	{
		StopIndividualStream();
	}
	m_hIndividualStream = BASS_StreamCreateURL(szUrl, 0, type, nullptr, nullptr);

	strcpy(&m_szIndividualLastLink[0], szUrl);
	m_bIndividualNeedReplay = type;

	BASS_ChannelPlay(m_hIndividualStream, false);
}

void CStreamPool::StopIndividualStream() // ready
{
	if (m_hIndividualStream)
	{
		BASS_StreamFree(m_hIndividualStream);
	}
	m_szIndividualLastLink[0] = 0;
	m_bIndividualNeedReplay = 0;
	m_hIndividualStream = NULL;
}

//void CStreamPool::PostListenerMatrix(RwMatrix* pMat)
//{
//	memcpy(&m_matListener, pMat, sizeof(RwMatrix));
//}
void CStreamPool::SetStreamVolume(int iID, float fVolume)
{
	BUFFERED_COMMAND_STREAMPOOL* bc;

	bc = bufferedCommands.WriteLock();

	float* pVolume = new float;

	*pVolume = fVolume;

	bc->iID = iID;
	bc->pData = (const void*)pVolume;

	bc->command = BUFFERED_COMMAND_STREAMPOOL::BC_VOLUME;

	bufferedCommands.WriteUnlock();
}
#include "netgame.h"
#include "game/Timer.h"
#include <jni.h>

extern CNetGame* pNetGame;

void CStreamPool::Process() // ready
{
	if (CTimer::m_UserPause)// pause
	{
		for(auto pStream : m_pStaticStream) {
			pStream->SetIsDeactivated(true);
		}
		for (int i = 0; i < MAX_STREAMS; i++)
		{
			if (m_pStreams[i] && m_bSlotState[i])
			{
				m_pStreams[i]->SetIsDeactivated(true);
			}
		}
		char temp[256];
		bool bCopied = false;
		if (m_szIndividualLastLink[0])
		{
			strcpy(&temp[0], &m_szIndividualLastLink[0]);

			if(m_bIndividualNeedReplay != BASS_STREAM_AUTOFREE)
				bCopied = true;
		}
		StopIndividualStream();
		if (bCopied)
		{
			strcpy(&m_szIndividualLastLink[0], &temp[0]);
		}

		m_bWasPaused = true;
	}
	else
	{
		if (m_bWasPaused)
		{
			for(auto pStream : m_pStaticStream) {
				pStream->SetIsDeactivated(false);
			}
			for (int i = 0; i < MAX_STREAMS; i++)
			{
				if (m_pStreams[i] && m_bSlotState[i])
				{
					m_pStreams[i]->SetIsDeactivated(false);
				}
			}
			if (m_szIndividualLastLink[0])
			{
				PlayIndividualStream(&m_szIndividualLastLink[0], m_bIndividualNeedReplay);
			}
			m_bWasPaused = false;
		}
	}

	BUFFERED_COMMAND_STREAMPOOL* bcs;
	if (!m_bWasPaused)
	{
		while ((bcs = bufferedCommands.ReadLock()) != 0)
		{
			if (bcs->command == BUFFERED_COMMAND_STREAMPOOL::BC_ADDSTREAM)
			{
				if (m_pStreams[bcs->iID] && m_bSlotState[bcs->iID])
				{
					delete m_pStreams[bcs->iID];
				}
				m_pStreams[bcs->iID] = (CStream*)bcs->pData;
				m_bSlotState[bcs->iID] = true;
			}
			if (bcs->command == BUFFERED_COMMAND_STREAMPOOL::BC_DELETSTREAM)
			{
				delete m_pStreams[bcs->iID];
				m_bSlotState[bcs->iID] = false;
			}
			if (bcs->command == BUFFERED_COMMAND_STREAMPOOL::BC_ATTACH)
			{
				if (m_pStreams[bcs->iID] && m_bSlotState[bcs->iID])
				{
					if (bcs->iType == 1)
					{
						m_pStreams[bcs->iID]->AttachToPlayer(bcs->iPlayerID);
					}
					if (bcs->iType == 2)
					{
						m_pStreams[bcs->iID]->AttachToVehicle(bcs->iPlayerID);
					}
				}
			}
			if (bcs->command == BUFFERED_COMMAND_STREAMPOOL::BC_VOLUME)
			{
				if (m_pStreams[bcs->iID] && m_bSlotState[bcs->iID])
				{
					float* pVolume = (float*)bcs->pData;

					m_pStreams[bcs->iID]->SetVolume(*pVolume);

					delete pVolume;
				}
			}
			if (bcs->command == BUFFERED_COMMAND_STREAMPOOL::BC_ACTIVATE)
			{
				for (int i = 0; i < MAX_STREAMS; i++)
				{
					if (m_pStreams[i] && m_bSlotState[i])
					{
						if (bcs->iID == 2)
						{
							m_pStreams[i]->SetIsDeactivated(false);
						}
						else if (bcs->iID == 1)
						{
							m_pStreams[i]->SetIsDeactivated(true);
						}
					}
				}
			}
			bufferedCommands.ReadUnlock();
		}
	}

	auto pLocalPed = pGame->FindPlayerPed();
//	RwMatrix matLocal;
//	pGame->FindPlayerPed()->GetMatrix(&matLocal);

	for (int i = 0; i < MAX_STREAMS; i++)
	{
		if (m_bSlotState[i] && m_pStreams[i])
		{
			m_pStreams[i]->Process(pLocalPed->m_pPed->mat);
		}
	}
	for(auto pStream : m_pStaticStream) {
		pStream->Process(pLocalPed->m_pPed->mat);
	}

	BASS_3DVECTOR pos;

	pos.x = pLocalPed->m_pPed->mat->pos.x;
	pos.y = pLocalPed->m_pPed->mat->pos.y;
	pos.z = pLocalPed->m_pPed->mat->pos.z;

//	BASS_3DVECTOR vel;
//	vel.x = pLocalPed->m_pPed->vecMoveSpeed.x;
//	vel.y = pLocalPed->m_pPed->vecMoveSpeed.y;
//	vel.z = pLocalPed->m_pPed->vecMoveSpeed.z;

	BASS_Set3DPosition(&pos, nullptr, nullptr, nullptr);

	BASS_Apply3D();
}