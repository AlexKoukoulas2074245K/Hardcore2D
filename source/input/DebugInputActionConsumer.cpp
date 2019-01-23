//
//  DebugInputActionConsumer.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#include "DebugInputActionConsumer.h"
#include "InputActions.h"

bool DebugInputActionConsumer::VConsumeInputAction(const InputAction& inputAction) const
{
    if (inputAction.mActionType == InputAction::ActionType::DEBUG_TOGGLE_HITBOX_DISPLAY && 
        inputAction.mActionState == InputAction::ActionState::START)
    {

    }

    return false;
}
