//
// Created by plaka on 17.02.2023.
//

#ifndef LIVERUSSIA_EPEDSTATE_H
#define LIVERUSSIA_EPEDSTATE_H

#pragma once

enum ePedState
{
    PEDSTATE_NONE = 0,
    PEDSTATE_IDLE,
    PEDSTATE_LOOK_ENTITY,
    PEDSTATE_LOOK_HEADING,
    PEDSTATE_WANDER_RANGE,
    PEDSTATE_WANDER_PATH,
    PEDSTATE_SEEK_POSITION,
    PEDSTATE_SEEK_ENTITY,
    PEDSTATE_FLEE_POSITION,
    PEDSTATE_FLEE_ENTITY,
    PEDSTATE_PURSUE,
    PEDSTATE_FOLLOW_PATH,
    PEDSTATE_SNIPER_MODE,
    PEDSTATE_ROCKETLAUNCHER_MODE,
    PEDSTATE_DUMMY,
    PEDSTATE_PAUSE,
    PEDSTATE_ATTACK,
    PEDSTATE_FIGHT,
    PEDSTATE_FACE_PHONE,
    PEDSTATE_MAKE_PHONECALL,
    PEDSTATE_CHAT,
    PEDSTATE_MUG,
    PEDSTATE_AIMGUN,
    PEDSTATE_AI_CONTROL,
    PEDSTATE_SEEK_CAR,
    PEDSTATE_SEEK_BOAT_POSITION,
    PEDSTATE_FOLLOW_ROUTE,
    PEDSTATE_CPR,
    PEDSTATE_SOLICIT,
    PEDSTATE_BUY_ICE_CREAM,
    PEDSTATE_INVESTIGATE_EVENT,
    PEDSTATE_EVADE_STEP,
    PEDSTATE_ON_FIRE,
    PEDSTATE_SUNBATHE,
    PEDSTATE_FLASH,
    PEDSTATE_JOG,
    PEDSTATE_ANSWER_MOBILE,
    PEDSTATE_HANG_OUT,
    PEDSTATE_STATES_NO_AI,
    PEDSTATE_ABSEIL_FROM_HELI,
    PEDSTATE_SIT,
    PEDSTATE_JUMP,
    PEDSTATE_FALL,
    PEDSTATE_GETUP,
    PEDSTATE_STAGGER,
    PEDSTATE_EVADE_DIVE,
    PEDSTATE_STATES_CAN_SHOOT,
    PEDSTATE_ENTER_TRAIN,
    PEDSTATE_EXIT_TRAIN,
    PEDSTATE_ARREST_PLAYER,
    PEDSTATE_DRIVING,
    PEDSTATE_PASSENGER,
    PEDSTATE_TAXI_PASSENGER,
    PEDSTATE_OPEN_DOOR,
    PEDSTATE_DIE,
    PEDSTATE_DEAD,
    PEDSTATE_DIE_BY_STEALTH,
    PEDSTATE_CARJACK,
    PEDSTATE_DRAGGED_FROM_CAR,
    PEDSTATE_ENTER_CAR,
    PEDSTATE_STEAL_CAR,
    PEDSTATE_EXIT_CAR,
    PEDSTATE_HANDS_UP,
    PEDSTATE_ARRESTED,
    PEDSTATE_DEPLOY_STINGER
};

#endif //LIVERUSSIA_EPEDSTATE_H
