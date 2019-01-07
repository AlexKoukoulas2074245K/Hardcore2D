//
//  TypeTraits.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef TypeTraits_h
#define TypeTraits_h

#include <type_traits>
#include <string>
#include <utility>

// Compute a unique hash for a given template class
template<class T>
std::size_t GetTypeHash()
{
    std::hash<std::string> hashFunction;
    return hashFunction(typeid(T).name());
}

#endif /* TypeTraits_h */

