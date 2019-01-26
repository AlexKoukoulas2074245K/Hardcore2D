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

class ServiceLocator;
class EventCommunicator;

class Camera final
{
public:
    Camera() = default;
    ~Camera() = default;
    Camera(const Camera&) = delete;
    const Camera& operator = (const Camera&) = delete;
    
    void Initialize(const ServiceLocator&, const glm::vec2& renderableDimensions);
    void Update(const glm::vec3& focusedEntityTranslation);
    const glm::mat4x4& GetViewMatrix() const;

private:
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    glm::vec2 mRenderableDimensions;
    glm::mat4x4 mViewMatrix;
};

#endif /* Camera_h */
