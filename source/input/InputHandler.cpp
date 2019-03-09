//
//  InputHandler.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "InputHandler.h"

#include <SDL_keyboard.h>

static const int DEFAULT_NUM_KEYS = 512;

InputHandler::InputHandler()
    : mPreviousKeyboardState(DEFAULT_NUM_KEYS, 0)
{
    SetDefaultKeybindings();
}

std::vector<InputAction> InputHandler::TranslateInputToActions()
{
    std::vector<InputAction> inputActions;
    
    int keyboardStateLength = 0;
    const auto* keyboardState = SDL_GetKeyboardState(&keyboardStateLength);
    
    for (const auto entry: mKeybindingsToActions)
    {
        // Key down this frame but not last frame (tap)
        if (keyboardState[entry.first] && !mPreviousKeyboardState[entry.first])
        {
            inputActions.emplace_back(InputAction(entry.second, InputAction::ActionState::START));
        }
        // Key down this frame and last frame (continuation)
        else if (keyboardState[entry.first] && mPreviousKeyboardState[entry.first])
        {
            inputActions.emplace_back(InputAction(entry.second, InputAction::ActionState::CONTINUE));
        }
        // Key up this frame, but down last frame
        else if (!keyboardState[entry.first] && mPreviousKeyboardState[entry.first])
        {
            inputActions.emplace_back(InputAction(entry.second, InputAction::ActionState::STOP));
        }
    }
    
    mPreviousKeyboardState.assign(keyboardState, keyboardState + keyboardStateLength);
    return inputActions;
}

void InputHandler::SetDefaultKeybindings()
{
    mKeybindingsToActions[SDL_SCANCODE_LEFT] = InputAction::ActionType::MOVE_LEFT;
    mKeybindingsToActions[SDL_SCANCODE_RIGHT] = InputAction::ActionType::MOVE_RIGHT;
    mKeybindingsToActions[SDL_SCANCODE_Z] = InputAction::ActionType::JUMP;
    mKeybindingsToActions[SDL_SCANCODE_X] = InputAction::ActionType::ATTACK;
    mKeybindingsToActions[SDL_SCANCODE_C] = InputAction::ActionType::FLAME_BREATH;
    mKeybindingsToActions[SDL_SCANCODE_R] = InputAction::ActionType::RESPAWN;
    mKeybindingsToActions[SDL_SCANCODE_H] = InputAction::ActionType::DEBUG_TOGGLE_HITBOX_DISPLAY;
    mKeybindingsToActions[SDL_SCANCODE_Q] = InputAction::ActionType::DEBUG_TOGGLE_SCENEGRAPH_DISPLAY;
}
