//
//  SDLCommon.h
//  Ninja
//
//  Created by Alex Koukoulas on 08/01/2019.
//

#ifndef SDLCommon_h
#define SDLCommon_h

#include <SDL.h>
#include <string>

inline void ShowMessageBox(const SDL_MessageBoxFlags messageBoxType, const std::string& title, const std::string& description)
{
    SDL_ShowSimpleMessageBox(messageBoxType, title.c_str(), description.c_str(), NULL);
}

#endif /* SDLCommon_h */
