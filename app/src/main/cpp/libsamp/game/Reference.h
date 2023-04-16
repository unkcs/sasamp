/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

//struct ENTITY_TYPE;

class CReference {
public:
    class CReference*       m_pNext;
    struct ENTITY_TYPE**    m_ppEntity;
};

static_assert(sizeof(CReference) == 0x8, "Invalid size CReference");
