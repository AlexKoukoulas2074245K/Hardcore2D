//
//  UIElementManager.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#include "UIElementManager.h"
#include "PlayerHealthbarUIElement.h"
#include "../../ServiceLocator.h"

UIElementManager::UIElementManager(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
}

UIElementManager::~UIElementManager()
{    
}

bool UIElementManager::VInitialize()
{
    return true;
}

void UIElementManager::Update(const float dt)
{
    for (const auto& uiElement: mUIElements)
    {
        uiElement->VUpdate(dt);
    }
}

std::vector<EntityId> UIElementManager::GetUIEntities() const
{
    std::vector<EntityId> uiEntities;
    for (const auto& uiElement: mUIElements)
    {
        uiEntities.insert(uiEntities.end(), uiElement->VGetEntityIds().begin(), uiElement->VGetEntityIds().end());
    }
    
    return uiEntities;
}

void UIElementManager::InitializeUIElements()
{
    mUIElements.emplace_back(std::make_unique<PlayerHealthbarUIElement>(mServiceLocator));
}
