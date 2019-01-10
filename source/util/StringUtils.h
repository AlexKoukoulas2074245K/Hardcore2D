//
//  StringUtils.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef StringUtils_h
#define StringUtils_h

#include <string>

inline std::string GetFileExtension(const std::string& path)
{
    auto reverseIter = path.rbegin();
    std::string pathExt;
    
    while (reverseIter != path.rend() && (*reverseIter != '.'))
    {
        pathExt = *reverseIter + pathExt;
        reverseIter++;
    }
    
    return pathExt;
}

#endif /* StringUtils_h */
