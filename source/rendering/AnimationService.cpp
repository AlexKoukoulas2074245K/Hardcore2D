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
    , mAnimationTimer(0.0f)
{
}

void AnimationService::UpdateAnimations(const std::vector<EntityId>& entityIds, const  float dt)
{
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    mAnimationTimer += dt;
    
    if (mAnimationTimer > 0.05f)
    {
        for (const auto entityId : entityIds)
        {
            if (entityComponentManager.HasComponent<AnimationComponent>(entityId))
            {
                entityComponentManager.GetComponent<AnimationComponent>(entityId).AdvanceFrame();
            }            
        }
        mAnimationTimer = 0.0f;
    }        
}