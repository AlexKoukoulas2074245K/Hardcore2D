//
//  InputHandler.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef InputHandler_h
#define InputHandler_h

#include "InputActions.h"
#include "../IService.h"

#include <vector>
#include <unordered_map>
#include <SDL_scancode.h>

class InputHandler final: public IService
{
    friend class App;
public:
    std::vector<InputAction> TranslateInputToActions();
    
private:
    InputHandler();
    
    void SetDefaultKeybindings();
    
    std::unordered_map<SDL_Scancode, InputAction::ActionType> mKeybindingsToActions;
    std::vector<unsigned char> mPreviousKeyboardState;
};

#endif /* InputHandler_h */
