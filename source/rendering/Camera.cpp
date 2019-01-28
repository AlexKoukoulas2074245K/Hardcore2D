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
#include "../util/MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>

static const float LOOKAHEAD_DISTANCE = 150.0f;

Camera::Camera()
    : mRenderableDimensions(0.0f, 0.0f)
    , mCameraTranslation(0.0f, 0.0f, 0.0f)
    , mLookingAheadRight(true)
{
    
}

void Camera::Initialize(const ServiceLocator& serviceLocator, const glm::vec2& renderableDimensions)
{
    mEventCommunicator = serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    mRenderableDimensions = renderableDimensions;
    mCameraTranslation = glm::vec3(-mRenderableDimensions.x * 0.5f, - mRenderableDimensions.y * 0.5f, -1.0f);
    
    mEventCommunicator->RegisterEventCallback<PlayerChangedDirectionEvent>([this](const IEvent& event)
    {
        mLookingAheadRight = static_cast<const PlayerChangedDirectionEvent&>(event).GetNewFacingDirection() == FacingDirection::RIGHT;
    });
}

void Camera::Update(const glm::vec3& focusedEntityTranslation, const glm::vec3& focusedEntityVelocity, const float dt)
{
    // Calculate target camera end position
    const auto lookAheadDistance = mLookingAheadRight ? LOOKAHEAD_DISTANCE : -LOOKAHEAD_DISTANCE;
    const auto targetX = focusedEntityTranslation.x + lookAheadDistance - mRenderableDimensions.x * 0.5f;
    
    // Increase horizontal position by inverse of distance to target
    mCameraTranslation.x += (2.0f * (targetX - mCameraTranslation.x) + focusedEntityVelocity.x) * dt;
    
    if (mLookingAheadRight && mCameraTranslation.x > targetX)
    {
        mCameraTranslation.x = targetX;
    }
    else if (!mLookingAheadRight && mCameraTranslation.x < targetX)
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

