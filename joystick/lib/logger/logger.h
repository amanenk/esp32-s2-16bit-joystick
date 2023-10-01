#pragma once
#include <Stream.h>
#include <vector>

void Log(const char *format, ...);
void Log(String str);

extern std::vector<Stream *> loggers;