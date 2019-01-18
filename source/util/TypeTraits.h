//
//  TypeTraits.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef TypeTraits_h
#define TypeTraits_h

#include <type_traits>
#include <string>
#include <utility>

static std::hash<std::string> hashFunction;

// Compute a unique hash for a given template class
template<class T>
inline std::size_t GetTypeHash()
{
    return hashFunction(typeid(T).name());
}

inline std::size_t GetTypeHash(const std::string& name)
{
    return hashFunction(name);
}

using EntityId = long long;
using ComponentTypeId = int;
using ResourceId = size_t;
using GLuint = unsigned int;

#endif /* TypeTraits_h */

