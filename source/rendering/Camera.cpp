//
//  Camera.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 25/01/2019.
//

#include "Camera.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformationComponent.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::Initialize(const glm::vec2& renderableDimensions)
{
    mRenderableDimensions = renderableDimensions;
}

void Camera::Update(const glm::vec3& focusedEntityTranslation)
{
    mViewMatrix = glm::lookAtLH(glm::vec3(focusedEntityTranslation.x - mRenderableDimensions.x * 0.5f, focusedEntityTranslation.y - mRenderableDimensions.y * 0.5f, -1.0f),
                                glm::vec3(focusedEntityTranslation.x - mRenderableDimensions.x * 0.5f, focusedEntityTranslation.y - mRenderableDimensions.y * 0.5f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4x4& Camera::GetViewMatrix() const
{
    return mViewMatrix;
}

