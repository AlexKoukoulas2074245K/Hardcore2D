//
//  AIService.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 29/01/2019.
//

#ifndef AIService_h
#define AIService_h

#include "../IService.h"
#include "../game/GameConstants.h"

#include <vector>

class ServiceLocator;
class EntityComponentManager;

class AIService final : public IService
{
    friend class App;
    
public:
    bool VInitialize() override;
    void UpdateAIComponents(const std::vector<EntityNameIdEntry>&, const float dt);

private:
    AIService(const ServiceLocator&);

    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
};

#endif
