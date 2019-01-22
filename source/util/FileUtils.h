//
//  FileUtils.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef FileUtils_h
#define FileUtils_h

#include <vector>
#include <string>
#include <algorithm>

#ifndef _WIN32
#include <dirent.h>
#else
#include <filesystem>
#endif

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

inline std::vector<std::string> GetAllFilenamesInDirectory(const std::string& directory)
{
	std::vector<std::string> fileNames;

#ifndef _WIN32
    DIR *dir;
    struct dirent *ent;
        
    if ((dir = opendir(directory.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            const std::string fileName(ent->d_name);
            
            if (fileName[0] != '.')
            {
                fileNames.push_back(fileName);
            }
        }
    }
#else
	for (const auto& entry : std::experimental::filesystem::v1::directory_iterator(directory))
	{
		fileNames.push_back(GetFileName(entry.path().string()));
	}
#endif

    std::sort(fileNames.begin(), fileNames.end());
    return fileNames;
}

#endif
