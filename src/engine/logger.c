#include "engine/logger.h"

void Engine_VLog(const Log_Level level, const char *ns, const char *module, const char *fmt, va_list args) {
    const time_t now = time(NULL);
    const struct tm *t = localtime(&now);
    char timebuf[9];
    strftime(timebuf, sizeof(timebuf), "%H:%M:%S", t);
    const char *color = COLOR_RESET;
    const char *tag = "[INFO]";
    switch (level) {
        case LOG_INFO:  color = COLOR_CYAN;     tag = "[INFO]"; break;
        case LOG_OK:    color = COLOR_GREEN;    tag = "[ OK ]"; break;
        case LOG_WARN:  color = COLOR_YELLOW;   tag = "[WARN]"; break;
        case LOG_ERROR: color = COLOR_RED;      tag = "[ERR!]"; break;
    }
    char srcbuf[16];
    snprintf(srcbuf, sizeof(srcbuf), "%s::%s", ns, module);
    fprintf(stdout, "%s [%s] %s %-15s: ", color, timebuf, tag, srcbuf);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "%s\n", COLOR_RESET);
}

void Engine_Log(const Log_Level level, const char *ns, const char *module, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Engine_VLog(level, ns, module, fmt, args);
    va_end(args);
}
