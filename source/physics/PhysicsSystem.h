//
//  PhysicsSystem.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/01/2019.
//

#ifndef PhysicsSystem_h
#define PhysicsSystem_h

#include "../IService.h"
#include "../util/TypeTraits.h"

#include <vector>

class ServiceLocator;
class PhysicsSystem final: public IService
{
    friend class App;
public:
    void UpdateEntities(const std::vector<EntityId>& entityIds, const float dt);
    
private:
    PhysicsSystem(const ServiceLocator&);
    
    const ServiceLocator& mServiceLocator;
};

#endif /* PhysicsSystem_h */
