//
//  AnimationService.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef AnimationService_h
#define AnimationService_h

#include "../IService.h"
#include "../util/TypeTraits.h"
#include "../game/GameConstants.h"

#include <vector>

class ServiceLocator;

class AnimationService final : public IService
{
    friend class App;
public:
    void UpdateAnimations(const std::vector<EntityNameIdEntry>&, const float dt);

private:
    AnimationService(const ServiceLocator&);

    const ServiceLocator& mServiceLocator;
};

#endif
