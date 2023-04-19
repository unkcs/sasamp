//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "../RW/rwplcore.h"

class CVector2D : public RwV2d {

public:
    constexpr CVector2D() = default;
    constexpr CVector2D(float XY) : RwV2d{XY, XY} {}
    constexpr CVector2D(float X, float Y) : RwV2d{ X, Y } {}
    constexpr CVector2D(const RwV2d& vec2d)     { x = vec2d.x; y = vec2d.y; }
    constexpr CVector2D(const CVector2D& vec2d) { x = vec2d.x; y = vec2d.y; }
};