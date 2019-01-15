//
//  FileUtils.h
//  Ninja
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef FileUtils_h
#define FileUtils_h

#include <string>
#include <filesystem>
#include <functional>

using dir_iter_entry = std::experimental::filesystem::directory_entry;

inline std::string GetFileExtension(const std::string& filePath)
{
	std::string pathExt;

	auto reverseIter = filePath.rbegin();
	
	while (reverseIter != filePath.rend() && (*reverseIter != '.'))
	{
		pathExt = *reverseIter + pathExt;
		reverseIter++;
	}

	return pathExt;
}

inline std::string GetFileName(const std::string& filePath)
{	
	std::string fileName;

	auto reverseIter = filePath.rbegin();

	while (reverseIter != filePath.rend() && (*reverseIter != '\\' && *reverseIter != '/'))
	{
		fileName = *reverseIter + fileName;
		reverseIter++;
	}

	return fileName;
}

inline std::string GetFileNameWithoutExtension(const std::string& filePath)
{
	std::string fileName = "";

	auto isRecordingFileName = false;
	auto reverseIter = filePath.rbegin();
		
	while (reverseIter != filePath.rend() && (*reverseIter != '\\' && *reverseIter != '/'))
	{

		if (!isRecordingFileName)
		{
			isRecordingFileName = *reverseIter == '.';
		}
		else
		{
			fileName = *reverseIter + fileName;
		}		
		
		reverseIter++;
	}

	return fileName;
}

inline void ApplyFunctionToEachFileRecursively(const std::string& rootDirectory, std::function<void(const dir_iter_entry&)> func)
{
	using dir_iter = std::experimental::filesystem::directory_iterator;
	for (const auto& entry : dir_iter(rootDirectory))
	{
		func(entry);
	}
}

#endif