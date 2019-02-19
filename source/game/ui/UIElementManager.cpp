//
//  UIElementManager.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#include "UIElementManager.h"
#include "IUIElement.h"
#include "../../components/EntityComponentManager.h"
#include "../../ServiceLocator.h"

UIElementManager::UIElementManager(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
}

bool UIElementManager::VInitialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    return true;
}

void UIElementManager::UpdateUIElements(const std::vector<std::unique_ptr<IUIElement>>& uiElements, const float dt)
{
    for (const auto& uiElement: uiElements)
    {
        uiElement->VUpdate(dt);
    }
}
