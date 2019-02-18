//
//  FactionComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/02/2019.
//

#ifndef FactionComponent_h
#define FactionComponent_h

#include "IComponent.h"
#include "../game/GameConstants.h"

class FactionComponent final: public IComponent
{
public:
    FactionComponent(const FactionGroup factionGroup);

    FactionGroup GetFactionGroup() const;
    
private:
    const FactionGroup mFactionGroup;
};

#endif /* FactionComponent_h */
