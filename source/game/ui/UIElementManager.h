//
//  UIElementManager.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef UIElementManager_h
#define UIElementManager_h

#include "../../IService.h"

#include <memory>
#include <vector>

class ServiceLocator;
class EntityComponentManager;
class IUIElement;

class UIElementManager final : public IService
{
    friend class App;
    
public:
    bool VInitialize() override;
    void UpdateUIElements(const std::vector<std::unique_ptr<IUIElement>>&, const float dt);

private:
    UIElementManager(const ServiceLocator&);

    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
};

#endif /* UIElementManager_h */
