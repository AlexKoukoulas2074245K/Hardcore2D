//
//  LightSamuraiAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 29/01/2019.
//

#include "LightSamuraiAIComponent.h"
#include "../util/Logging.h"

void LightSamuraiAIComponent::VUpdate(const float)
{
    Log(LogType::INFO, "Updating light samurai");
}

std::string LightSamuraiAIComponent::VSerializeToString() const
{
    return "";
}

bool LightSamuraiAIComponent::VInitializeFromString(const std::string&)
{
    return true;
}
