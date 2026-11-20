#ifndef FIM_TYPES_H 
#define FIM_TYPES_H
#include <stdint.h>

#define MAX_PATH_LEN 512 //max charachter allowed for path input
#define HASH_LEN 17          // 16 hex chars for 64-bit FNV-1a + null terminator
#define MAX_FILES 1024

// 64-bit FNV-1a Standard Constants
#define FNV_OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME        1099511628211ULL

typedef enum {
    STATUS_UNCHANGED,
    STATUS_MODIFIED,
    STATUS_DELETED,
    STATUS_NEW
} FileStatus;

typedef struct {
    char filepath[MAX_PATH_LEN];
    char hash[HASH_LEN];
    FileStatus status;
    char timestamp[32];
} FileRecord;

#endif