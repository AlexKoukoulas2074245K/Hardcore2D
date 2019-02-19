//
//  UIElementManager.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef UIElementManager_h
#define UIElementManager_h

#include "../../IService.h"
#include "../../util/TypeTraits.h"

#include <memory>
#include <vector>

class ServiceLocator;
class IUIElement;

class UIElementManager final : public IService
{
    friend class App;
    
public:
    ~UIElementManager();
    
    bool VInitialize() override;
    
    void InitializeUIElements();
    void Update(const float dt);
    std::vector<EntityId> GetUIEntities() const;
    
private:
    UIElementManager(const ServiceLocator&);

    const ServiceLocator& mServiceLocator;

    std::vector<std::unique_ptr<IUIElement>> mUIElements;
};

#endif /* UIElementManager_h */
