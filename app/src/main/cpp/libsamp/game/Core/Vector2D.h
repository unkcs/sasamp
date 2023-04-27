//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "../RW/rwplcore.h"
class CVector;

class CVector2D : public RwV2d {

public:
    constexpr CVector2D() = default;
    constexpr CVector2D(float XY) : RwV2d{XY, XY} {}
    constexpr CVector2D(float X, float Y) : RwV2d{ X, Y } {}
    constexpr CVector2D(const RwV2d& vec2d)     { x = vec2d.x; y = vec2d.y; }
    constexpr CVector2D(const CVector2D& vec2d) { x = vec2d.x; y = vec2d.y; }

    //! Create a vector with the given heading (0 rad is at 3 O'Clock)
    //! It is made to be compatible with `CMatrix::SetRotateZOnly` but in reality it probably should be x = sin, y = -cos instead
    //! Because the following should be true: `CVector2D::FromHeading(heading).Heading() + PI == heading` (And it isn't :D)
    //constexpr static auto FromHeading(float headingRad) { return CVector2D{ -std::sin(headingRad), std::cos(headingRad) }; }


    CVector2D(const CVector& vec3d);
};

constexpr inline bool operator==(const CVector2D& vecLeft, const CVector2D& vecRight) {
    return vecLeft.x == vecRight.x && vecLeft.y == vecRight.y;
}