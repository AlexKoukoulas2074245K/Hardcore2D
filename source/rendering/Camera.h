//
//  Camera.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 25/01/2019.
//

#ifndef Camera_h
#define Camera_h

#include "../util/TypeTraits.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <memory>

class EntityComponentManager;
class EventCommunicator;
class ServiceLocator;

class Camera final
{
public:
    Camera(const ServiceLocator& serviceLocator,
           const glm::vec2& renderableDimensions,
           const glm::vec2& levelHorBounds,
           const glm::vec2& levelVerBounds);
    ~Camera();
    Camera(const Camera&) = delete;
    const Camera& operator = (const Camera&) = delete;

    void Update(const EntityId focusedEntityId, const float dt);
    const glm::mat4x4& GetViewMatrix() const;

private:
    const EntityComponentManager& mEntityComponentManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    const glm::vec2& mLevelHorBounds;
    const glm::vec2& mLevelVerBounds;
    const glm::vec2& mRenderableDimensions;
    
    glm::vec3 mCameraTranslation;
    glm::mat4x4 mViewMatrix;
    bool mLookingAheadRight;
};

#endif /* Camera_h */
