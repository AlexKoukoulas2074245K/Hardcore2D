//
//  AIService.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "AIService.h"
#include "../components/EntityComponentManager.h"
#include "../components/IAIComponent.h"
#include "../ServiceLocator.h"

AIService::AIService(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
}

bool AIService::Initialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    return true;
}

void AIService::UpdateAIComponents(const std::vector<EntityNameIdEntry>& entityIds, const float dt)
{
    for (const auto entityEntry: entityIds)
    {
        if (mEntityComponentManager->HasComponent<IAIComponent>(entityEntry.mEntityId))
        {
            mEntityComponentManager->GetComponent<IAIComponent>(entityEntry.mEntityId).VUpdate(dt);
        }
    }
}
