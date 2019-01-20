//
//  InputHandler.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef InputHandler_h
#define InputHandler_h

#include "IService.h"

#include <vector>
#include <utility>
#include <unordered_map>
#include <SDL_scancode.h>

class InputHandler final: public IService
{
    friend class App;
public:
    enum class ActionType
    {
        NO_ACTION, MOVE_LEFT, MOVE_RIGHT, JUMP, ATTACK
    };
    
    enum class ActionState
    {
        START, CONTINUE, STOP
    };
    
    using InputAction = std::pair<ActionType, ActionState>;
    
    std::vector<InputAction> TranslateInputToActions();
    
private:
    InputHandler();
    
    void SetDefaultKeybindings();
    
    std::unordered_map<SDL_Scancode, ActionType> mKeybindingsToActions;
    std::vector<unsigned char> mPreviousKeyboardState;
};

#endif /* InputHandler_h */
