//
//  StringUtils.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef StringId_h
#define StringId_h

#include "TypeTraits.h"

#include <utility>
#include <string>

class StringId final
{
public:
	StringId(const std::string& str)
		: mString(str)
		, mStringId(GetTypeHash(str))
	{
	}

	operator size_t () { return mStringId; }

	const std::string& GetString() const { return mString; }
	size_t GetStringId() const { return mStringId; }

private:
	std::string mString;
	size_t mStringId;
};

inline bool operator < (const StringId& lhs, const StringId& rhs)
{
    return lhs.GetStringId() < rhs.GetStringId();
}

#endif /* StringId_h */
