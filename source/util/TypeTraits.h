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
#include <cfloat>

static std::hash<std::string> hashFunction;

class TypeID
{
    static size_t counter;

public:
    template<class T>
    static size_t value()
    {
        static size_t id = counter++;
        return id;
    }
};

// Compute a unique hash for a given template class
template<class T>
inline std::size_t GetTypeHash()
{
    return TypeID::value<T>();
}

// Compute a unique hash for a given string
inline std::size_t GetStringHash(const std::string& name)
{
    return hashFunction(name);
}

using EntityId = long long;
using ComponentTypeId = int;
using ResourceId = size_t;
using GLuint = unsigned int;

#endif /* TypeTraits_h */

