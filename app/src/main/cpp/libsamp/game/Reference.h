/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

//struct CEntityGta;

class CReference {
public:
    class CReference*       m_pNext;
    class CEntityGta**      m_ppEntity;
};

static_assert(sizeof(CReference) == 0x8, "Invalid size CReference");
