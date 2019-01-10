//
//  BaseResource.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef BaseResource_h
#define BaseResource_h

#include "../util/TypeTraits.h"

class BaseResource
{
    friend class TextureLoader;
    friend class FileLoader;
public:
    virtual ~BaseResource() = default;
    
    ResourceId GetId() const { return mId; }
    
protected:
    BaseResource(const ResourceId resourceId): mId(resourceId){}
    
private:
    const ResourceId mId;
};

#endif /* BaseResource_h */
