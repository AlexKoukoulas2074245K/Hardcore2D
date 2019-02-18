//
//  AnimationService.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "AnimationService.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"

AnimationService::AnimationService(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
}

void AnimationService::UpdateAnimations(const std::vector<EntityNameIdEntry>& entityIds, const  float dt)
{
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    
    for (const auto entityEntry : entityIds)
    {
        const auto entityId = entityEntry.mEntityId;
        if (entityComponentManager.HasComponent<AnimationComponent>(entityId))
        {
            auto& animationComponent = entityComponentManager.GetComponent<AnimationComponent>(entityId);
            animationComponent.SetAnimationTimer(animationComponent.GetAnimationTimer() + dt);
            if (animationComponent.GetAnimationTimer() > animationComponent.GetAnimationFrameDuration())
            {
                animationComponent.SetAnimationTimer(0.0f);
                animationComponent.AdvanceFrame();
            }
        }
    }
}
