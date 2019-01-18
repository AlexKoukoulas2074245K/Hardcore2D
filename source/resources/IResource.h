//
//  IResource.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef IResource_h
#define IResource_h

#include "../util/TypeTraits.h"

class IResource
{
public:
    virtual ~IResource() = default;
    IResource(const IResource&) = delete;
    const IResource& operator = (const IResource&) = delete;
    
protected:
    IResource(){};
};

#endif /* IResource_h */
