#include "logs.h"

#include <stdio.h>
#include <string.h>

#include "data_types_aliases.h"

void log(LogType type, const byte* tag, const byte* message) {
    char type_str[20] = {0};

    switch (type) {
        case LOG_ERROR:
            strcpy(type_str, "Error");
            break;
        case LOG_INFO:
            strcpy(type_str, "Information");
            break;
        case LOG_WARN:
            strcpy(type_str, "Warning");
            break;
        case LOG_DEBUG:
            strcpy(type_str, "Debug");
            break;
    }

    fprintf(stderr, "[%s][%s] %s\n", type_str, tag, message);
}