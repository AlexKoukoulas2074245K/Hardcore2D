//
//  StringUtils.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef StringUtils_h
#define StringUtils_h

#include <vector>
#include <string>
#include <sstream>

inline bool StringStartsWith(const std::string& s, const std::string& pattern)
{
    if (s.size() < pattern.size()) return false;
    
    for (auto i = 0U; i < pattern.size(); ++i)
    {
        if (s[i] != pattern[i]) return false;
    }
    
    return true;
}

inline std::vector<std::string> StringSplit(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    
    return elems;
}

#endif /* StringUtils_h */
