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

void AnimationService::UpdateAnimations(const std::vector<EntityId>& entityIds, const  float dt)
{
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    
    for (const auto entityId : entityIds)
    {
        if (entityComponentManager.HasComponent<AnimationComponent>(entityId))
        {
            auto& animationComponent = entityComponentManager.GetComponent<AnimationComponent>(entityId);
            animationComponent.SetAnimationTimer(animationComponent.GetAnimationTimer() + dt);
            if (animationComponent.GetAnimationTimer() > animationComponent.GetAnimationDuration())
            {
                animationComponent.SetAnimationTimer(0.0f);
                animationComponent.AdvanceFrame();
            }
        }
    }
}
