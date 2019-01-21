//
//  MathUtils.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef MathUtils_h
#define MathUtils_h

#include <glm/vec3.hpp>

template<class T>
T Min(T a, T b)
{
    return a < b ? a : b;
}

template<class T>
T Max(T a, T b)
{
    return a > b ? a : b;
}

template<class T>
T Abs(const T val)
{
    return val < 0 ? -val : val;
}

glm::vec3 ClampToMax(const glm::vec3& vec, const glm::vec3& maxVec)
{
    return glm::vec3(Min(vec.x, maxVec.x), Min(vec.y, maxVec.y), Min(vec.z, maxVec.z));
}

glm::vec3 ClampToMin(const glm::vec3& vec, const glm::vec3& minVec)
{
    return glm::vec3(Max(vec.x, minVec.x), Max(vec.y, minVec.y), Max(vec.z, minVec.z));
}

#endif /* MathUtils_h */
