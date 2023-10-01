

#include "logger.h"
#include <cstddef>
#include <cstdarg>
#include <cstdio>
#include <cstring>

std::vector<Stream *> loggers;

void Log(const char *format, ...)
{
    char log_str[12000];

    va_list args;
    va_start(args, format);

    int n = vsnprintf(log_str, sizeof(log_str), format, args);
    va_end(args);

    for (auto logger : loggers)
    {
        logger->print(log_str);
    }
}

void Log(String  str){
    for (auto logger : loggers)
    {
        logger->println(str.c_str());
    }
}
