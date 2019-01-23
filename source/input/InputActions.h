//
//  InputActions.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#ifndef InputActions_h
#define InputActions_h

#include <utility>

enum class ActionType
{
    MOVE_LEFT, MOVE_RIGHT, JUMP, ATTACK
};

enum class ActionState
{
    START, CONTINUE, STOP
};

using InputAction = std::pair<ActionType, ActionState>;

#endif /* InputActions_h */
