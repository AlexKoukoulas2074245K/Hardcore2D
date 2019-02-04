//
//  InputActions.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#ifndef InputActions_h
#define InputActions_h

#include <utility>

struct InputAction
{
    enum class ActionType
    {
        MOVE_LEFT, MOVE_RIGHT, JUMP, ATTACK, RANGED, DEBUG_TOGGLE_HITBOX_DISPLAY
    };

    enum class ActionState
    {
        START, CONTINUE, STOP
    };

    InputAction(const ActionType actionType, const ActionState actionState)
        : mActionType(actionType)
        , mActionState(actionState)
    {
    }

    const ActionType mActionType;
    const ActionState mActionState;
};

#endif /* InputActions_h */
