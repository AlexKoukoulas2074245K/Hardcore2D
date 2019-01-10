//
//  Logging.h
//  Ninja
//
//  Created by Alex Koukoulas on 09/01/2019.
//

#ifndef Logging_h
#define Logging_h


#include <stdio.h>
#include <string>

enum class LogType
{
    INFO, WARNING, ERROR
};

#ifndef NDEBUG
inline void Log(const LogType logType, const char* message, ...)
{

    switch(logType)
    {
        case LogType::INFO: printf("[INFO] "); break;
        case LogType::WARNING: printf("[WARNING] "); break;
        case LogType::ERROR: printf("[ERROR] "); break;
    }
    
    va_list args;
    va_start (args, message);
    vprintf (message, args);
    va_end (args);
    
    printf("\n");
}
#else
inline void Log(const LogType, const char*, ...) {}
#endif /* not NDEBUG */

#endif /* Logging_h */
