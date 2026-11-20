
#ifndef FIM_TYPES_H
#define FIM_TYPES_H

#include <stdint.h>

// Maximum sizes
#define MAX_PATH_LEN 512
#define HASH_LEN 17
#define MAX_FILES 1024

// FNV-1a hashing constants
#define FNV_OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME 1099511628211ULL

// Possible file statuses
typedef enum {
    STATUS_UNCHANGED,
    STATUS_MODIFIED,
    STATUS_DELETED,
    STATUS_NEW
} FileStatus;

// Information stored about each file
typedef struct {
    char filepath[MAX_PATH_LEN];
    char hash[HASH_LEN];
    FileStatus status;
    char timestamp[32];
} FileRecord;

#endif
