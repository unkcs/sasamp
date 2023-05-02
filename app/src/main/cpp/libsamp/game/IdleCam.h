//
// Created by plaka on 01.05.2023.
//

#pragma once

#include "common.h"

#pragma pack(push, 1)
class CIdleCam {
public:
    CEntityGta  *pTarget;
    CVector     positionToSlerpFrom;
    float       timeSlerpStarted;
    float       slerpDuration;
    CVector     lastIdlePos;
    float       slerpTime;
    float       timeControlsIdleForIdleToKickIn;
    float       timeIdleCamStarted;
    int32       lastFrameProcessed;
    float       timeLastTargetSelected;
    float       timeMinimumToLookAtSomething;
    float       timeTargetEntityWasLastVisible;
    float       timeToConsiderNonVisibleEntityAsOccluded;
    float       distTooClose;
    float       distStartFOVZoom;
    float       distTooFar;
    int32       targetLOSFramestoReject;
    int32       targetLOSCounter;
    int32       zoomState;
    float       zoomFrom;
    float       zoomTo;
    float       timeZoomStarted;
    float       zoomNearest;
    float       zoomFarthest;
    float       curFOV;
    float       durationFOVZoom;
    bool        bForceAZoomOut;
    bool        bHasZoomedIn;
    uint8       pad0[2];
    float       timeBeforeNewZoomIn;
    float       timeLastZoomIn;
    float       increaseMinimumTimeFactorforZoomedIn;
    float       degreeShakeIdleCam;
    float       shakeBuildUpTime;
    int32       lastTimePadTouched;
    int32       idleTickerFrames;
    uintptr     *pCam; // CCam

public:
    static void InjectHooks();
};
#pragma pack(pop)

VALIDATE_SIZE(CIdleCam, 0x9C);

extern CIdleCam gIdleCam;