//
// Created by plaka on 02.05.2023.
//

#pragma once

#include "../RW/RenderWare.h"

class Sprite {
public:
    static float m_f2DNearScreenZ;
    static float m_f2DFarScreenZ;
    static float m_fRecipNearClipPlane;

public:
    static void InjectHooks();

    static void Initialise();
    static void InitSpriteBuffer();
    static void InitSpriteBuffer2D();
    static void FlushSpriteBuffer();

    static void Draw3DSprite(float, float, float, float, float, float, float, float, float);

    static bool  CalcScreenCoors(const RwV3d* posn, RwV3d* out, float* w, float* h, bool checkMaxVisible, bool checkMinVisible);
    static float CalcHorizonCoors();
};
