#ifndef _REQUESTER_UTILS_POINTERS_H
#define _REQUESTER_UTILS_POINTERS_H

#include <stddef.h>
#include <stdlib.h>

#define RETURN_IF_NULL(pointer) \
    if (pointer == NULL) return

#define RETURN_IF_ZERO(var) \
    if (var == 0) return

#define RELEASE_IF_NOT_NULL(pointer) \
    if (pointer != NULL) free(pointer)

#define RETURN_NULL_IF_NULL(pointer) \
    if ((pointer) == NULL) return NULL

#define RETURN_NULL_IF_ZERO(var) \
    if ((var) == 0) return NULL

#define RETURN_ZERO_IF_NULL(pointer) \
    if ((pointer) == NULL) return 0

#define RETURN_ZERO_IF_ZERO(var) \
    if ((var) == 0) return 0

#define CONTINUE_IF_NULL(pointer) \
    if ((pointer) == NULL) continue

#define BREAK_IF_NULL(pointer) \
    if ((pointer) == NULL) continue

#define CONTINUE_IF_ZERO(var) \
    if ((var) == 0) break

#define BREAK_IF_ZERO(var) \
    if ((var) == 0) break

#endif