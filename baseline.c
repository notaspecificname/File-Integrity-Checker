#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_PATH_LEN 512
#define HASH_LEN 17      // 16 hex chars for 64-bit FNV-1a + null terminator
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


int save_baseline(const char *baseline_path, const FileRecord *records,int count){

    FILE *fptr;
    fptr = fopen(baseline_path,"w");       //Creates new file which stores hashes and its data

    if(fptr == NULL){       //If file isnt made, it returns -1
        return -1;     
    }


    for(int x = 0; x < count; x++){
        fprintf(fptr, "%s,%s,%s", records[x].filepath, records[x].hash, records[x].timestamp);    //Adds each record in increasng order to file. Starts from line 0, index 0
    }

    fclose(fptr);

    return 0;   //File saved successfully
}

