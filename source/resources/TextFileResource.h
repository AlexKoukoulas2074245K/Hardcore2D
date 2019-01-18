//
//  TextFileResource.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 14/01/2019.
//

#ifndef TextFileResource_h
#define TextFileResource_h

#include "IResource.h"

#include <string>

class TextFileResource final: public IResource
{
    friend class TextFileLoader;
public:
    const std::string& GetContents() const;
    
private:
    TextFileResource(const std::string& contents);
    
    const std::string mContents;
};

#endif /* TextFileResource_h */
