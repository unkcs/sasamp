//
// Created by plaka on 26.11.2022.
//

#include "ObjectEditor.h"
#include <jni.h>
#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "keyboard.h"
#include "CSettings.h"
#include "chatwindow.h"
#include "util/CJavaWrapper.h"

extern CJavaWrapper *g_pJavaWrapper;

void CObjectEditor::startEditPlayerAttach(int slot)
{
    if(!pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->IsValidAttach(slot)){
        CChatWindow::AddDebugMessage("Invalid attach slot %d", slot);
        return;
    }
    CObjectEditor::iEditedId = slot;
    CObjectEditor::editType = TYPE_PLAYER_ATTACH;

    CObjectEditor::showGui();
}

void CObjectEditor::startEditObject(uint16 objectId)
{
    auto pObject = pNetGame->GetObjectPool()->GetAt(objectId);
    if(!pObject) {
        CChatWindow::AddDebugMessage("Invalid object id %d", objectId);
        return;
    }
    CObjectEditor::iEditedId = objectId;

    if(pObject->m_bAttachedType == 1) // attached to veh?
        CObjectEditor::editType = TYPE_VEHICLE_ATTACH;
    else
        CObjectEditor::editType = TYPE_OBJECT;

    CObjectEditor::showGui();
}

void CObjectEditor::showGui() {
    JNIEnv* env = g_pJavaWrapper->GetEnv();

    if(CObjectEditor::thiz == nullptr)
    {
        jmethodID constructor = env->GetMethodID(CObjectEditor::clazz, "<init>", "(Landroid/app/Activity;)V");
        CObjectEditor::thiz = env->NewObject(CObjectEditor::clazz, constructor, g_pJavaWrapper->activity);
        CObjectEditor::thiz = env->NewGlobalRef(CObjectEditor::thiz);
    }

    bIsToggle = true;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_AttachEdit_Exit(JNIEnv *env, jobject thiz) {
    CPlayerPed* pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
    int slot = CObjectEditor::iEditedId;

    CObjectEditor::iEditedId = INVALID_EDITED_SLOT;
    CObjectEditor::SendOnEditAttach(
            0,
            slot,
            pPlayer->m_aAttachedObjects[slot].dwModelId,
            pPlayer->m_aAttachedObjects[slot].dwSampBone,
            pPlayer->m_aAttachedObjects[slot].vecOffset,
            pPlayer->m_aAttachedObjects[slot].vecRotation,
            pPlayer->m_aAttachedObjects[slot].vecScale
            );

    CObjectEditor::bIsToggle = false;
    env->DeleteGlobalRef(CObjectEditor::thiz );
    CObjectEditor::thiz = nullptr;
}

void CObjectEditor::SendOnEditAttach(int response, int index, int modelid, int bone, CVector offset, CVector rot, CVector scale){

    RakNet::BitStream bsSend;

    bsSend.Write((uint32_t)response);
    bsSend.Write((uint32_t)index);
    bsSend.Write((uint32_t)modelid);
    bsSend.Write((uint32_t)bone);
    bsSend.Write(offset);
    bsSend.Write(rot);
    bsSend.Write(scale);
    bsSend.Write((uint32_t)0);
    bsSend.Write((uint32_t)0);

    pNetGame->GetRakClient()->RPC(&RPC_EditAttachedObject, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_AttachEdit_AttachClick(JNIEnv *env, jobject thiz, jint button_type,
                                                  jboolean button_id) {
    auto pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
    int slot = CObjectEditor::iEditedId;

    CObject* pObject = nullptr;
    if(CObjectEditor::editType != CObjectEditor::TYPE_PLAYER_ATTACH) {
        pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);
        pObject->bNeedReAttach = true;
    }
    if(button_type == 0) { // право/лево
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecOffset.z += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
            pObject->m_vecAttachedOffset.x += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            auto pos = pObject->m_pEntity->GetPosition();
            pos.x += value;

            pObject->m_pEntity->SetPosn(pos);
        }
    }

    if(button_type == 1) { // вверх/низ
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecOffset.x += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
            pObject->m_vecAttachedOffset.z += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            auto pos = pObject->m_pEntity->GetPosition();
            pos.z += value;

            pObject->m_pEntity->SetPosn(pos);
        }
    }
    if(button_type == 2) { // Y
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecOffset.y += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
            pObject->m_vecAttachedOffset.y += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            auto pos = pObject->m_pEntity->GetPosition();
            pos.y += value;

            pObject->m_pEntity->SetPosn(pos);
            pObject->m_pEntity->UpdateRW();
            pObject->m_pEntity->UpdateRwFrame();
        }
    }
    if(button_type == 3) { // scale
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecScale += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
          //  pObject->m_vec += value;
        }
    }
    if(button_type == 4) { // rot x
        float value = (button_id)?(0.1f):(-0.1f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecRotation.x += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
            pObject->m_vecAttachedRotation.x += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            float x, y, z;
            pObject->m_pEntity->GetOrientation(x, y, z);
            x += value;

            pObject->InstantRotate(x, y, z);
        }
    }
    if(button_type == 5) { // rot y
        float value = (button_id)?(0.1f):(-0.1f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecRotation.y += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
            pObject->m_vecAttachedRotation.y += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            float x, y, z;
            pObject->m_pEntity->GetOrientation(x, y, z);
            y += value;

            pObject->InstantRotate(x, y, z);
        }
    }
    if(button_type == 6) { // rot z
        float value = (button_id)?(1.0f):(-1.0f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_aAttachedObjects[slot].vecRotation.z += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH) {
            pObject->m_vecAttachedRotation.z += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {

            float x, y, z;
            pObject->m_pEntity->GetOrientation(x, y, z);
            z += value;

            pObject->InstantRotate(x, y, z);
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_AttachEdit_Save(JNIEnv *env, jobject thiz) {
    CPlayerPed* pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
    int slot = CObjectEditor::iEditedId;

    if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
        CObjectEditor::SendOnEditAttach(
                1,
                slot,
                pPlayer->m_aAttachedObjects[slot].dwModelId,
                pPlayer->m_aAttachedObjects[slot].dwSampBone,
                pPlayer->m_aAttachedObjects[slot].vecOffset,
                pPlayer->m_aAttachedObjects[slot].vecRotation,
                pPlayer->m_aAttachedObjects[slot].vecScale
        );
    }
    if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT)
    {
        auto pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);
        if(!pObject)
            goto exit;

        auto pos = pObject->m_pEntity->GetPosition();
        CVector rot;
        pObject->m_pEntity->GetOrientationDeg(rot.x, rot.y, rot.z);

        RakNet::BitStream bsSend;

        bsSend.Write((bool)     CObjectEditor::bPlayerObj);
        bsSend.Write((uint16_t) CObjectEditor::iEditedId);
        bsSend.Write((uint32_t) CObjectEditor::eResponse::EDIT_RESPONSE_FINAL);
        bsSend.Write((float)    pos.x);
        bsSend.Write((float)    pos.y);
        bsSend.Write((float)    pos.z);
        bsSend.Write((float)    rot.x);
        bsSend.Write((float)    rot.y);
        bsSend.Write((float)    rot.z);

        pNetGame->GetRakClient()->RPC(&RPC_EditObject, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
    }
    if(CObjectEditor::editType == CObjectEditor::TYPE_VEHICLE_ATTACH)
    {
        auto pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);
        if(!pObject)
            goto exit;

        auto pos = pObject->m_vecAttachedOffset;
        auto rot = pObject->m_vecAttachedRotation;

        RakNet::BitStream bs;

        bs.Write((uint8_t)  ID_CUSTOM_RPC);
        bs.Write((uint8_t)  RPC_VEHICLE_ATTACH);
        bs.Write((uint16_t) CObjectEditor::iEditedId);
        bs.Write((uint8_t)  CObjectEditor::eResponse::EDIT_RESPONSE_FINAL);
        bs.Write((float)    pos.x);
        bs.Write((float)    pos.y);
        bs.Write((float)    pos.z);
        bs.Write((float)    rot.x);
        bs.Write((float)    rot.y);
        bs.Write((float)    rot.z);

        pNetGame->GetRakClient()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
    }



    exit:
        CObjectEditor::bIsToggle = false;

        env->DeleteGlobalRef(CObjectEditor::thiz );
        CObjectEditor::thiz = nullptr;
}