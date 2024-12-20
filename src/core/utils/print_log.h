#ifndef _REQUESTER_UTILS_LOGS_H
#define _REQUESTER_UTILS_LOGS_H

typedef enum _LogType LogType;

enum _LogType {
    LOG_ERROR,
    LOG_INFO,
    LOG_WARN,
    LOG_DEBUG
};

void print_log(LogType type, const char* tag, const char* message);

#endif