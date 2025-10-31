#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_INFO,
    LOG_OK,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

void Engine_Log(LogLevel level, const char *ns, const char *module, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_LOGGER_H
