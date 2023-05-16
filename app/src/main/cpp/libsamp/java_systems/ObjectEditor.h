//
// Created by plaka on 26.11.2022.
//

#pragma once
#include <jni.h>
#include "game/common.h"

#define INVALID_EDITED_SLOT -1

static class CObjectEditor {
public:
    enum eEditType {
        TYPE_NONE,
        TYPE_PLAYER_ATTACH,
        TYPE_VEHICLE_ATTACH,
        TYPE_OBJECT
    };

    enum eResponse : uint32 {
        EDIT_RESPONSE_CANCEL,
        EDIT_RESPONSE_FINAL,
        EDIT_RESPONSE_UPDATE
    };

    static inline jobject   thiz        = nullptr;
    static inline jclass    clazz       = nullptr;
    static inline bool      bIsToggle   = false;
    static inline int       iEditedId   = INVALID_EDITED_SLOT;
    static inline eEditType editType    = TYPE_NONE;
    static inline bool      bPlayerObj  = false; // hueta by samp

public:
    static void startEditPlayerAttach(int slot);

    static void
    SendOnEditAttach(int response, int index, int modelid, int bone, CVector offset, CVector rot, CVector scale);

    static void showGui();

    static void startEditObject(uint16 objectId);
};