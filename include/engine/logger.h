#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_CYAN    "\x1b[36m"

typedef enum {
    LOG_INFO,
    LOG_OK,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

void Engine_Log(LogLevel level, const char *fmt, ...);

#endif // LOGGER_H
