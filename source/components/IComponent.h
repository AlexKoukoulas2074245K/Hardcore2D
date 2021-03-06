//
//  IComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef IComponent_h
#define IComponent_h

#include <string>

class IComponent
{
public:
    virtual ~IComponent() = default;
	IComponent(const IComponent&) = delete;
	const IComponent& operator = (const IComponent&) = delete;

protected:
    IComponent() = default;
};

#endif /* IComponent_h */
