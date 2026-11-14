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

int load_baseline(const char *baseline_path, FileRecord *records, int *count){

    int loadedCount = 0;        //Stores the number of records successfully loaded in struct
    int line[1024];
    FILE *fptr;
    fptr = fopen(baseline_path,"r");

    if(fptr == NULL){
        return -1;
    }

    while (fgets(line, sizeof(line), fptr) != NULL) {
        char temp_path[MAX_PATH_LEN];
        char temp_hash[HASH_LEN];
        char temp_time[32];

        int loadedCount = sscanf(line," %511[^,], %16[^,], 31[^,]", temp_path, temp_hash, temp_time); //Whenever it encounters a comma it removes the , and puts the values before it into the variable.

        if(loadedCount == 3){

            snprintf(records[*count].filepath, MAX_PATH_LEN, "%s", temp_path);  //Copies over path to actual struct
            snprintf(records[*count].hash,HASH_LEN,"%s",temp_hash);     //Copies over hash to actual struct
            snprintf(records[*count].timestamp,32,"%s",temp_time);      //Copies over time to actual struct
            records[*count].status = STATUS_UNCHANGED;      //Default status

            (*count)++;

            if((*count) >= MAX_FILES){
                break;
            }
        }
    
    }
    fclose(fptr);
    return 0;
}

