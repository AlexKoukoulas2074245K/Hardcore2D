//
//  PlayerHealthbarIUIElement.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#include "PlayerHealthbarUIElement.h"
#include "../../ServiceLocator.h"
#include "../../resources/ResourceManager.h"
#include "../../rendering/CoreRenderingService.h"
#include "../../events/EventCommunicator.h"
#include "../../events/AnnouncePlayerEntityIdEvent.h"
#include "../../events/EntityDamagedEvent.h"
#include "../../components/EntityComponentManager.h"
#include "../../components/ShaderComponent.h"
#include "../../components/HealthComponent.h"
#include "../../components/AnimationComponent.h"
#include "../../components/TransformComponent.h"

PlayerHealthbarUIElement::PlayerHealthbarUIElement(const ServiceLocator& serviceLocator)
    : mCoreRenderingService(serviceLocator.ResolveService<CoreRenderingService>())
    , mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mResourceManager(serviceLocator.ResolveService<ResourceManager>())
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
    , mPlayerId(-1)
    , mStartingHealth(0.0f)
{
    InitializeHealthbarEntities();
    RegisterEventCallbacks();
}

PlayerHealthbarUIElement::~PlayerHealthbarUIElement()
{
    
}

void PlayerHealthbarUIElement::VUpdate(const float dt)
{    
}

const std::vector<EntityId>& PlayerHealthbarUIElement::VGetEntityIds() const
{
    return mEntityIds;
}

void PlayerHealthbarUIElement::InitializeHealthbarEntities()
{
    // Initialize healthbar layout entity
    const auto healthbarFlowEntityId = mEntityComponentManager.GenerateEntity();
    const auto baseHealthbarFlowScale = 1.0f;
    
    mEntityComponentManager.AddComponent<ShaderComponent>(healthbarFlowEntityId, std::make_unique<ShaderComponent>(StringId("background")));
    mEntityComponentManager.AddComponent<AnimationComponent>(healthbarFlowEntityId, std::make_unique<AnimationComponent>("misc/player_healthbar_flow", 100.0f, mResourceManager));
    mEntityComponentManager.AddComponent<TransformComponent>(healthbarFlowEntityId, std::make_unique<TransformComponent>(glm::vec3(-0.69f, 0.82f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(baseHealthbarFlowScale/mCoreRenderingService.GetAspectRatio(), baseHealthbarFlowScale, 0.0f)));
    
    // Initialize health bar flow entity
    const auto healthbarLayoutEntityId = mEntityComponentManager.GenerateEntity();
    const auto baseHealthbarLayoutScale = 1.0f;
    
    mEntityComponentManager.AddComponent<ShaderComponent>(healthbarLayoutEntityId, std::make_unique<ShaderComponent>(StringId("background")));
    mEntityComponentManager.AddComponent<AnimationComponent>(healthbarLayoutEntityId, std::make_unique<AnimationComponent>("misc/player_healthbar_layout", 100.0f, mResourceManager));
    mEntityComponentManager.AddComponent<TransformComponent>(healthbarLayoutEntityId, std::make_unique<TransformComponent>(glm::vec3(-0.7f, 0.8f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(baseHealthbarLayoutScale/mCoreRenderingService.GetAspectRatio(), baseHealthbarLayoutScale, 0.0f)));
    
    mEntityIds.push_back(healthbarFlowEntityId);
    mEntityIds.push_back(healthbarLayoutEntityId);
}

void PlayerHealthbarUIElement::RegisterEventCallbacks()
{
    mEventCommunicator->RegisterEventCallback<AnnouncePlayerEntityIdEvent>([this](const IEvent& event)
    {
        mPlayerId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
        mStartingHealth = mEntityComponentManager.GetComponent<HealthComponent>(mPlayerId).GetHealth();
    });

    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);
        if (actualEvent.GetDamagedEntityId() == mPlayerId)
        {
            mCurrentHealth = actualEvent.GetHealthRemaining();

            const auto healthRatio = mCurrentHealth / mStartingHealth;
            mEntityComponentManager.GetComponent<TransformComponent>(mEntityIds[0]).GetScale().x = healthRatio / mCoreRenderingService.GetAspectRatio();
            mEntityComponentManager.GetComponent<TransformComponent>(mEntityIds[0]).GetTranslation().x = -0.7f - (0.220f * (1.0f - healthRatio));
        }
    });
}
