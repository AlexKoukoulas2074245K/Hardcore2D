//
//  MathUtils.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef MathUtils_h
#define MathUtils_h

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <cmath>
#include <ctime>
#include <random>
#include <functional>

static constexpr float PI = 3.141592f;

template<class T>
inline T Min(T a, T b)
{
    return a < b ? a : b;
}

template<class T>
inline T Max(T a, T b)
{
    return a > b ? a : b;
}

template<class T>
inline T Abs(const T val)
{
    return val < 0 ? -val : val;
}

template <class T>
inline T Lerp(const T x, const T y, const float t)
{
    return (T)(x * (1.0f - t) + y * t);
}

inline glm::vec3 ClampToMax(const glm::vec3& vec, const glm::vec3& maxVec)
{
    return glm::vec3(Min(vec.x, maxVec.x), Min(vec.y, maxVec.y), Min(vec.z, maxVec.z));
}

inline glm::vec3 ClampToMin(const glm::vec3& vec, const glm::vec3& minVec)
{
    return glm::vec3(Max(vec.x, minVec.x), Max(vec.y, minVec.y), Max(vec.z, minVec.z));
}

inline glm::vec2 Vec3ToVec2(const glm::vec3& vec)
{
    return glm::vec2(vec.x, vec.y);
}

inline glm::vec3 Vec2ToVec3(const glm::vec2& vec)
{
    return glm::vec3(vec.x, vec.y, 0.0f);
}

enum class TweeningMode
{
    EASE_IN, EASE_OUT, EASE_IN_OUT
};

inline float LinearFunction(const float t)
{
    return t;
}

inline float QuadFunction(const float t)
{
    return std::powf(t, 2.0f);
}

inline float CubicFunction(const float t)
{
    return std::powf(t, 3.0f);
}

inline float QuartFunction(const float t)
{
    return std::powf(t, 4.0f);
}

inline float QuintFunction(const float t)
{
    return std::powf(t, 5.0f);
}

inline float BackFunction(const float t)
{
    return std::powf(t, 2.0f) * (2.70158f * t - 1.70158f);
}

inline float BounceFunction(const float t)
{
    if (t < 4.0f/11.0f)
    {
        return (121.0f * t * t)/16.0f;
    }
    else if (t < 8.0f/11.0f)
    {
        return (363.0f/40.0f * t * t) - (99.0f/10.0f * t) + 17.0f/5.0f;
    }
    else if (t < 9.0f/10.0f)
    {
        return (4356.0f/361.0f * t * t) - (35442.0f/1805.0f * t) + 16061.0f/1805.0f;
    }
    else
    {
        return (54.0f/5.0f * t * t) - (513.0f/25.0f * t) + 268.0f/25.0f;
    }
}

inline float ElasticFunction(const float t)
{
    return std::sinf(13.0f * PI * 0.5f * t) * std::powf(2.0f, 10.0f * (t - 1));
}

inline float TweenValue(const float val, const std::function<float(const float)> tweeningFunc = LinearFunction , const TweeningMode tweeningMode = TweeningMode::EASE_OUT)
{
    switch (tweeningMode)
    {
        case TweeningMode::EASE_IN: return tweeningFunc(val); break;
        case TweeningMode::EASE_OUT: return 1.0f - tweeningFunc(1.0f - val); break;
        case TweeningMode::EASE_IN_OUT: return (val < 0.5f) ? TweenValue(val * 2.0f, tweeningFunc, TweeningMode::EASE_IN)/2.0f : 0.5f + (TweenValue((val - 0.5f) * 2.0f, tweeningFunc, TweeningMode::EASE_OUT)/2.0f);
    }
    
    return 0.0f;
}

static bool hasInitializedSeed = false;
inline int RandomInt(const int min = 0, const int max = RAND_MAX)
{
    if (!hasInitializedSeed)
    {
        std::srand(static_cast<unsigned int>(time(nullptr)));
        hasInitializedSeed = true;
    }
    
    if (max == RAND_MAX)
    {
        return (std::rand() % (max - min)) + min;
    }
    else
    {
        return (std::rand() % (max - min + 1)) + min;
    }
}

inline float RandomFloat(const float min = 0.0f, const float max = 1.0f)
{
    if (!hasInitializedSeed)
    {
        std::srand(static_cast<unsigned int>(time(nullptr)));
        hasInitializedSeed = true;
    }

    return min + static_cast <float> (RandomInt()) / (static_cast <float> (RAND_MAX / (max - min)));
}

inline int RandomSign()
{
    return (RandomInt(0, 2)) ? 1 : -1;
}

inline float Sinf(const float val)
{
    return std::sinf(val);
}

inline float Cosf(const float val)
{
    return std::cosf(val);
}

inline float Arctan2(const float x, const float y)
{
    return std::atan2(x, y);
}

#endif /* MathUtils_h */
