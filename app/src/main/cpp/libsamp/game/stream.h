#include "..//vendor/bass/bass.h"
class CStream
{
	HSTREAM m_hStream;
	CVector m_vPos;
	int m_iInterior;
	float m_fDistance;
	char m_szUrl[255];

	bool m_bIsAttached;
	int m_iAttachType;
	int m_iAttachedTo;

	void CreateStream();
	void DestroyStream();
	void ProcessAttached();

	bool m_bIsDeactivated;

	//friend void CALLBACK MyDownloadProc(const void* buffer, DWORD length, void* user);
public:
	CStream(CVector* pPos, int iInterior, float fDistance, const char* szUrl);
	~CStream();

	void AttachToVehicle(int iVehicleID);
	void AttachToPlayer(int iPlayerID);

	void SetVolume(float fValue);

	void SetIsDeactivated(bool bIsDeactivated);

	void Process(RwMatrix* pMatListener);

	void SetPosition(CVector vec);
};