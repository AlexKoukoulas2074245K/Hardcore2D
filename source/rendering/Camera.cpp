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
#include "../events/PlayerMeleeAttackEvent.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../util/MathUtils.h"
#include "../util/Logging.h"
#include "../game/GameConstants.h"

#include <glm/gtc/matrix_transform.hpp>

const float Camera::MAX_LOOKAHEAD_DISTANCE = 150.0f;
const float Camera::SHAKE_RADIUS = 50.0f;

Camera::Camera(const ServiceLocator& serviceLocator,
               const glm::vec2& renderableDimensions,
               const glm::vec2& levelHorBounds,
               const glm::vec2& levelVerBounds)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mLevelHorBounds(levelHorBounds)
    , mLevelVerBounds(levelVerBounds)
    , mRenderableDimensions(renderableDimensions)
    , mLookingAheadRight(true)
    , mIsShaking(false)
    , mShakeRadius(0.0f)
    , mInitShakeRandomAngle(0.0f)
{
    mTranslation = glm::vec3(-mRenderableDimensions.x * 0.5f, - mRenderableDimensions.y * 0.5f, -1.0f);
    
    mEventCommunicator = serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    mEventCommunicator->RegisterEventCallback<PlayerChangedDirectionEvent>([this](const IEvent& event)
    {
        mLookingAheadRight = static_cast<const PlayerChangedDirectionEvent&>(event).GetNewFacingDirection() == FacingDirection::RIGHT;
    });
    mEventCommunicator->RegisterEventCallback<PlayerMeleeAttackEvent>([this](const IEvent&)
    {
        mIsShaking = true;
        mShakeRadius = SHAKE_RADIUS;
        mPreShakeTranslation = mTranslation;
        const auto mInitShakeRandomAngle = RandomFloat(0.0f, 2.0f * PI);
        const auto offset = glm::vec2(Sinf(mInitShakeRandomAngle) * mShakeRadius, Cosf(mInitShakeRandomAngle) * mShakeRadius);
        mTranslation.x += offset.x;
        mTranslation.y += offset.y;
    });
}

Camera::~Camera()
{
    
}

void Camera::Update(const EntityId focusedEntityId, const float dt)
{    
    UpdateFollowingEntity(focusedEntityId, dt);    

    if (mIsShaking)
    {
        UpdateScreenShake(dt);
    }
    
    mViewMatrix = glm::lookAtLH(mTranslation,
                                glm::vec3(mTranslation.x, mTranslation.y, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4x4& Camera::GetViewMatrix() const
{
    return mViewMatrix;
}

bool Camera::IsTransformInsideViewRect(const TransformComponent& transformComponent) const
{
    const auto& translation = transformComponent.GetTranslation();
    const auto& scale = transformComponent.GetScale();

    return translation.x + scale.x >= mTranslation.x &&
        translation.y + scale.y >= mTranslation.y &&
        translation.x - scale.x <= mTranslation.x + mRenderableDimensions.x &&
        translation.y - scale.y <= mTranslation.y + mRenderableDimensions.y;
}

void Camera::UpdateFollowingEntity(const EntityId focusedEntityId, const float dt)
{
    const auto& focusedEntityTranslation = mEntityComponentManager.GetComponent<TransformComponent>(focusedEntityId).GetTranslation();
    const auto& focusedEntityVelocity = mEntityComponentManager.GetComponent<PhysicsComponent>(focusedEntityId).GetVelocity();

    // Calculate target camera end position
    const auto lookAheadDistance = mLookingAheadRight ? MAX_LOOKAHEAD_DISTANCE : -MAX_LOOKAHEAD_DISTANCE;
    const auto targetX = focusedEntityTranslation.x + lookAheadDistance - mRenderableDimensions.x * 0.5f;

    // Increase horizontal position by inverse of distance to target
    mTranslation.x += ((targetX - mTranslation.x) / 5.0f + 1.5f * focusedEntityVelocity.x) * dt;

    // Stop camera if it passes the target position
    if ((mLookingAheadRight && mTranslation.x > targetX) ||
        (!mLookingAheadRight && mTranslation.x < targetX))
    {
        mTranslation.x = targetX;
    }

    // Vertical position always matches focused entity's y
    mTranslation.y = focusedEntityTranslation.y - mRenderableDimensions.y * 0.5f;

    // Stop camera if it passes the level boundaries
    if (mTranslation.x < mLevelHorBounds.x - CELL_SIZE / 4) mTranslation.x = mLevelHorBounds.x - CELL_SIZE / 4;
    if (mTranslation.y < mLevelVerBounds.x) mTranslation.y = mLevelVerBounds.x;
    if (mTranslation.x + mRenderableDimensions.x > mLevelHorBounds.y + CELL_SIZE / 4) mTranslation.x = mLevelHorBounds.y - mRenderableDimensions.x + CELL_SIZE / 4;
    if (mTranslation.y + mRenderableDimensions.y > mLevelVerBounds.y) mTranslation.y = mLevelVerBounds.y - mRenderableDimensions.y;
}

void Camera::UpdateScreenShake(const float)
{
    mShakeRadius *= 0.9f;
    const auto randomSign = static_cast<float>(RandomSign());    
    mInitShakeRandomAngle += (PI + randomSign * RandomFloat(0.0f, PI * 0.33f));
    const auto offset = glm::vec2(Sinf(mInitShakeRandomAngle) * mShakeRadius, Cosf(mInitShakeRandomAngle) * mShakeRadius);
    mTranslation.x += offset.x;
    mTranslation.y += offset.y;

    if (mShakeRadius < 0.01f)
    {
        mIsShaking = false;
    }
}
