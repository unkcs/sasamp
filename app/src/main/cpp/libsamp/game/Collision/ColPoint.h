//
// Created by plaka on 21.04.2023.
//

#pragma once

#include "../common.h"

struct tColLighting {
    union {
        struct {
            uint8 day : 4;
            uint8 night : 4;
        };
        uint8 value;
    };

    tColLighting() = default;
    constexpr explicit tColLighting(const uint8 ucLighting) {
        day = ucLighting & 0xF;
        night = (ucLighting >> 4) & 0xF;
    }

    [[nodiscard]] float GetCurrentLighting(float fScale = 0.5F) const;
};
VALIDATE_SIZE(tColLighting, 0x1);
