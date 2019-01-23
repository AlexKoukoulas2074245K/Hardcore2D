//
//  DebugInputActionConsumer.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#include "DebugInputActionConsumer.h"
#include "InputActions.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicationService.h"
#include "../events/EventCommunicator.h"
#include "../events/DebugToggleHitboxDisplayEvent.h"

DebugInputActionConsumer::DebugInputActionConsumer(const ServiceLocator& serviceLocator)
    : mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{

}

DebugInputActionConsumer::~DebugInputActionConsumer()
{

}

bool DebugInputActionConsumer::VConsumeInputAction(const InputAction& inputAction) const
{
    if (inputAction.mActionType == InputAction::ActionType::DEBUG_TOGGLE_HITBOX_DISPLAY && 
        inputAction.mActionState == InputAction::ActionState::START)
    {
        mEventCommunicator->DispatchEvent(std::make_unique<DebugToggleHitboxDisplayEvent>());
    }

    return false;
}
