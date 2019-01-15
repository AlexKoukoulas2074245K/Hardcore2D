//
//  FileUtils.h
//  Ninja
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef FileUtils_h
#define FileUtils_h

#include <vector>
#include <string>
#include <dirent.h>

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
    DIR *dir;
    struct dirent *ent;
    
    std::vector<std::string> fileNames;
    if ((dir = opendir(directory.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            const std::string fileName(ent->d_name);
            
            if (fileName != "." && fileName != "..")
            {
                fileNames.push_back(fileName);
            }
        }
    }
    
    return fileNames;
}

#endif
