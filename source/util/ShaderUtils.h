//
//  ShaderUtils.h
//  Ninja
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#ifndef ShaderUtils_h
#define ShaderUtils_h

#include "StringUtils.h"

static std::vector<std::string> GetUniformNames(const std::string& shaderContents)
{
    const auto shaderContentsSplitByNewLine = StringSplit(shaderContents, '\n');
    
    std::vector<std::string> uniformNames;
    for (const auto line: shaderContentsSplitByNewLine)
    {
        if (StringStartsWith(line, "uniform"))
        {
            const auto lineSplitBySpace = StringSplit(line, ' ');
            const auto uniformNameIncludingSemicolumn = lineSplitBySpace[lineSplitBySpace.size() - 1];
            uniformNames.emplace_back(uniformNameIncludingSemicolumn.begin(), uniformNameIncludingSemicolumn.end() - 1);
        }
    }
    return uniformNames;
}

#endif /* ShaderUtils_h */
