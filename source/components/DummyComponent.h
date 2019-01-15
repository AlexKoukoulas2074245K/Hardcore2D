//
//  DummyComponent.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef DummyComponent_h
#define DummyComponent_h

#include "IComponent.h"

class DummyComponent final: public IComponent
{
public:
    std::string VSerializeToString() const override { return std::string(); };
    bool VInitializeFromString(const std::string&) override { return true; }
};

#endif /* DummyComponent_h */
