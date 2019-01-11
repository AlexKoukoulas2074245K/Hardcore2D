//
//  IComponent.h
//  Ninja
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
    virtual std::string SerializeToString() const = 0;
    virtual bool InitializeFromString(const std::string&) = 0;
    
protected:
    IComponent() = default;
};

#endif /* IComponent_h */
