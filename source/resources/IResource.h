//
//  IResource.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef IResource_h
#define IResource_h

#include "../util/TypeTraits.h"

class IResource
{
    friend class TextureLoader;
    friend class FileLoader;
public:
    virtual ~IResource() = default;
    IResource(const IResource&) = delete;
    const IResource& operator = (const IResource&) = delete;
    
    ResourceId GetId() const { return mId; }
    
protected:
    IResource(const ResourceId resourceId): mId(resourceId){}
    
private:
    const ResourceId mId;
};

#endif /* IResource_h */
