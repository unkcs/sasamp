//
// Created by plaka on 30.04.2023.
//

#pragma once

struct CGarage {
    uint8_t pad[0xd8];
};
VALIDATE_SIZE(CGarage, 0xD8);