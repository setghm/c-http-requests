#ifndef _REQUESTER_UTILS_LOGS_H
#define _REQUESTER_UTILS_LOGS_H

#include "data_types_aliases.h"

typedef enum _LogType LogType;

enum _LogType {
    LOG_ERROR,
    LOG_INFO,
    LOG_WARN,
    LOG_DEBUG
};

void log(LogType type, const byte* tag, const byte* message);

#endif