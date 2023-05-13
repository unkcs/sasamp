//
// Created by plaka on 11.01.2023.
//

#pragma once

#include <cassert>
#include "constants.h"
#include "common.h"
#include "CdStreamInfo.h"
#include "entity.h"
#include "game/Core/LinkList.h"
#include "StreamingInfo.h"

enum class eChannelState
{
    // Doing nothing
    IDLE = 0,

    // Currently reading model(s)
    READING = 1,

    // A big model (also called a large file) is loaded in steps:
    // First, the variable `ms_bLoadingBigModel` is set to `true` in `RequestModelStream`.
    // When the first half has finished loading, and `ProcessLoadingChannel` is called
    // `ConvertBufferToObject` will be called by it, which will set the model's load state to
    // `LOADSTATE_FINISHING`. When the latter function returns the former checks if
    // the model's loadstate is `FINISHING`, if it is the channel's state is set to
    // `STARTED` to indicate a large model's loading has started and is yet to be finished.
    // Loading a large model is finished when `ProcessLoadingChannel`.
    // (In which case it's state still should be `STARTED`)
    STARTED = 2,

    // Also called ERROR, but that's a `windgi.h` macro
    ERR = 3,
};

enum eResourceFirstID : int32_t {
    // First ID of the resource
    RESOURCE_ID_DFF                = 0,                                     // default: 0
    RESOURCE_ID_TXD                = RESOURCE_ID_DFF + TOTAL_DFF_MODEL_IDS, // default: 20000
    RESOURCE_ID_COL                = RESOURCE_ID_TXD + TOTAL_TXD_MODEL_IDS, // default: 25000
    RESOURCE_ID_IPL                = RESOURCE_ID_COL + TOTAL_COL_MODEL_IDS, // default: 25255
    RESOURCE_ID_DAT                = RESOURCE_ID_IPL + TOTAL_IPL_MODEL_IDS, // default: 25511
    RESOURCE_ID_IFP                = RESOURCE_ID_DAT + TOTAL_DAT_MODEL_IDS, // default: 25575
    RESOURCE_ID_RRR                = RESOURCE_ID_IFP + TOTAL_IFP_MODEL_IDS, // default: 25755   (vehicle recordings)
    RESOURCE_ID_SCM                = RESOURCE_ID_RRR + TOTAL_RRR_MODEL_IDS, // default: 26230   (streamed scripts)

    // Used for CStreaming lists, just search for xrefs (VS: shift f12)
    RESOURCE_ID_LOADED_LIST_START  = RESOURCE_ID_SCM + TOTAL_SCM_MODEL_IDS, // default: 26312
    RESOURCE_ID_LOADED_LIST_END    = RESOURCE_ID_LOADED_LIST_START + 1,     // default: 26313

    RESOURCE_ID_REQUEST_LIST_START = RESOURCE_ID_LOADED_LIST_END + 1,       // default: 26314
    RESOURCE_ID_REQUEST_LIST_END   = RESOURCE_ID_REQUEST_LIST_START + 1,    // default: 26315
    RESOURCE_ID_TOTAL                                               // default: 26316
};

enum class eModelType {
    DFF,
    TXD,
    COL,
    IPL,
    DAT,
    IFP,
    RRR,
    SCM,

    INTERNAL_1,
    INTERNAL_2,
    INTERNAL_3,
    INTERNAL_4
};

// Helper functions to deal with modelID's


inline bool IsModelDFF(int32 model) { return RESOURCE_ID_DFF <= model && model < RESOURCE_ID_TXD; }
inline bool IsModelTXD(int32 model) { return RESOURCE_ID_TXD <= model && model < RESOURCE_ID_COL; }
inline bool IsModelCOL(int32 model) { return RESOURCE_ID_COL <= model && model < RESOURCE_ID_IPL; }
inline bool IsModelIPL(int32 model) { return RESOURCE_ID_IPL <= model && model < RESOURCE_ID_DAT; }
inline bool IsModelDAT(int32 model) { return RESOURCE_ID_DAT <= model && model < RESOURCE_ID_IFP; }
inline bool IsModelIFP(int32 model) { return RESOURCE_ID_IFP <= model && model < RESOURCE_ID_RRR; }
inline bool IsModelRRR(int32 model) { return RESOURCE_ID_RRR <= model && model < RESOURCE_ID_SCM; }
inline bool IsModelSCM(int32 model) { return RESOURCE_ID_SCM <= model && model < RESOURCE_ID_LOADED_LIST_START; }

inline eModelType GetModelType(int32 model) {
    if (IsModelDFF(model))
        return eModelType::DFF;

    else if (IsModelTXD(model))
        return eModelType::TXD;

    else if (IsModelCOL(model))
        return eModelType::COL;

    else if (IsModelIPL(model))
        return eModelType::IPL;

    else if (IsModelDAT(model))
        return eModelType::DAT;

    else if (IsModelIFP(model))
        return eModelType::IFP;

    else if (IsModelRRR(model))
        return eModelType::RRR;

    else if (IsModelSCM(model))
        return eModelType::SCM;

    else {
        assert(0); // NOTSA
        return (eModelType)-1;
    }
}

// Turn relative IDs into absolute ones.
inline int32 DFFToModelId(int32 relativeId) { return RESOURCE_ID_DFF + relativeId; }
inline int32 TXDToModelId(int32 relativeId) { return RESOURCE_ID_TXD + relativeId; }
inline int32 COLToModelId(int32 relativeId) { return RESOURCE_ID_COL + relativeId; }
inline int32 IPLToModelId(int32 relativeId) { return RESOURCE_ID_IPL + relativeId; }
inline int32 DATToModelId(size_t relativeId) { return (size_t)RESOURCE_ID_DAT + relativeId; }
inline int32 IFPToModelId(int32 relativeId) { return RESOURCE_ID_IFP + relativeId; }
inline int32 RRRToModelId(int32 relativeId) { return RESOURCE_ID_RRR + relativeId; }
inline int32 SCMToModelId(int32 relativeId) { return RESOURCE_ID_SCM + relativeId; }

// Turn absolute IDs into relative ones
inline int32 ModelIdToDFF(int32 absId) { return absId - RESOURCE_ID_DFF; }
inline int32 ModelIdToTXD(int32 absId) { return absId - RESOURCE_ID_TXD; }
inline int32 ModelIdToCOL(int32 absId) { return absId - RESOURCE_ID_COL; }
inline int32 ModelIdToIPL(int32 absId) { return absId - RESOURCE_ID_IPL; }
inline int32 ModelIdToDAT(int32 absId) { return absId - RESOURCE_ID_DAT; }
inline int32 ModelIdToIFP(int32 absId) { return absId - RESOURCE_ID_IFP; }
inline int32 ModelIdToRRR(int32 absId) { return absId - RESOURCE_ID_RRR; }
inline int32 ModelIdToSCM(int32 absId) { return absId - RESOURCE_ID_SCM; }

#pragma pack(push, 1)
struct tRwStreamInitializeData {
    uint8* m_pBuffer;
    uint32 m_uiBufferSize;
};
#pragma pack(pop)
static_assert(sizeof(tRwStreamInitializeData) == 0x8);

#pragma pack(push, 1)
struct tStreamingFileDesc {
    tStreamingFileDesc() = default;

    tStreamingFileDesc(const char* name, bool bNotPlayerImg) :
            m_bNotPlayerImg(bNotPlayerImg),
            m_StreamHandle(CdStreamOpen(name))
    {
        strncpy(m_szName, name, std::size(m_szName));
    }

    [[nodiscard]] bool IsInUse() const noexcept { return m_szName[0]; }

    char  m_szName[40]{}; // If this string is empty (eg.: first elem in array is NULL) the entry isnt in use
    bool  m_bNotPlayerImg{};
    uint8 pad[3];
    int32 m_StreamHandle{-1};
};
#pragma pack(pop)
VALIDATE_SIZE(tStreamingFileDesc, 0x30);


class CStreaming {
public:
    static inline size_t ms_memoryUsed;
    static inline size_t ms_memoryAvailable;
    static inline uint32 desiredNumVehiclesLoaded;
    static inline bool ms_bLoadVehiclesInLoadScene;
    static inline tStreamingFileDesc ms_files[TOTAL_IMG_ARCHIVES];
    static inline CLinkList<CEntityGta*> ms_rwObjectInstances;
    static inline CStreamingInfo ms_aInfoForModel[RESOURCE_ID_TOTAL];

public:
    static void InjectHooks();
    static int AddImageToList(const char *name, bool b);

    static void InitImageList();
    static void RemoveAllUnusedModels();

    static void RemoveModel(int32 modelId);
    static void RequestModel(int32 modelId, int32 flags = STREAMING_GAME_REQUIRED);

    static CStreamingInfo& GetInfo(int32 modelId) { assert(modelId >= 0); return ms_aInfoForModel[modelId]; }
    static bool IsModelLoaded(int32 model) { return ms_aInfoForModel[model].m_nLoadState == eStreamingLoadState::LOADSTATE_LOADED; }

    static void LoadAllRequestedModels(bool bPriorityRequestsOnly);
};
