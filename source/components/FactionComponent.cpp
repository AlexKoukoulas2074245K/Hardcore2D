//
//  FactionComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/02/2019.
//

#include "FactionComponent.h"

FactionComponent::FactionComponent(const FactionGroup factionGroup)
    : mFactionGroup(factionGroup)
{

}

const FactionGroup FactionComponent::GetFactionGroup() const
{
    return mFactionGroup;
}
