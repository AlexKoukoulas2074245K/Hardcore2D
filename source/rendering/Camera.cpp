//
//  Camera.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 25/01/2019.
//

#include "Camera.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/PlayerChangedDirectionEvent.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../util/MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>

static const float LOOKAHEAD_DISTANCE = 150.0f;

Camera::Camera(const ServiceLocator& serviceLocator,
               const glm::vec2& renderableDimensions,
               const glm::vec2& levelHorBounds,
               const glm::vec2& levelVerBounds)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mLevelHorBounds(levelHorBounds)
    , mLevelVerBounds(levelVerBounds)
    , mRenderableDimensions(renderableDimensions)
    , mLookingAheadRight(true)
{
    mCameraTranslation = glm::vec3(-mRenderableDimensions.x * 0.5f, - mRenderableDimensions.y * 0.5f, -1.0f);
    
    mEventCommunicator = serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    mEventCommunicator->RegisterEventCallback<PlayerChangedDirectionEvent>([this](const IEvent& event)
    {
        mLookingAheadRight = static_cast<const PlayerChangedDirectionEvent&>(event).GetNewFacingDirection() == FacingDirection::RIGHT;
    });
}

Camera::~Camera()
{
    
}

auto x = 0.0f;
auto y = 0.0f;

void Camera::Update(const EntityId focusedEntityId, const float dt)
{
    x = mLevelHorBounds.x;
    y = mLevelVerBounds.y;
    
    const auto& focusedEntityTranslation = mEntityComponentManager.GetComponent<TransformComponent>(focusedEntityId).GetTranslation();
    const auto& focusedEntityVelocity = mEntityComponentManager.GetComponent<PhysicsComponent>(focusedEntityId).GetVelocity();
    
    // Calculate target camera end position
    const auto lookAheadDistance = mLookingAheadRight ? LOOKAHEAD_DISTANCE : -LOOKAHEAD_DISTANCE;
    const auto targetX = focusedEntityTranslation.x + lookAheadDistance - mRenderableDimensions.x * 0.5f;
    
    // Increase horizontal position by inverse of distance to target
    mCameraTranslation.x += (2.0f * (targetX - mCameraTranslation.x) + focusedEntityVelocity.x) * dt;
    
    // Stop camera if its passed the target position
    if ((mLookingAheadRight && mCameraTranslation.x > targetX) ||
        (!mLookingAheadRight && mCameraTranslation.x < targetX))
    {
        mCameraTranslation.x = targetX;
    }

    // Vertical position always matches focused entity's y
    mCameraTranslation.y = focusedEntityTranslation.y - mRenderableDimensions.y * 0.5f;
    mViewMatrix = glm::lookAtLH(mCameraTranslation,
                                glm::vec3(mCameraTranslation.x, mCameraTranslation.y, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4x4& Camera::GetViewMatrix() const
{
    return mViewMatrix;
}

