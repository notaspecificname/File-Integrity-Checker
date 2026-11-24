#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "fim_types.h"
//size_t is unsigned long long used for counting
void gettime(char *buffer, size_t max_len){
    time_t now =time(NULL); //writing raw seconds
    struct tm *t= localtime(&now); //converting to localtime using builtin struct from time.h
    if (t!=NULL){
        strftime(buffer, max_len, "%Y-%m-%d %H:%M:%S", t); //converting raw seconds to a time string 
    } 
}

int compute_file_hash(const char *filepath, char *output_hash){

    FILE *file=fopen(filepath,"rb");//opening file in raw byte mode
    if (file==NULL){
        return 1;
    }

    uint64_t hash=FNV_OFFSET_BASIS;//making 64bit hash with intial hash value from header
    unsigned char buffer[4096]; //making a buffer for 4KB bytes
    size_t bytes_read; //making a variable to store the bytes already read

    while((bytes_read = fread(buffer,1,sizeof(buffer),file))>0){ //fread(buffer,1,sizeof(buffer),file) reads 4096 items from file and stores them one by one into buffer //aslong as bytes read is greater then 0 bytes the loop is run//
        for(size_t i=0;i<bytes_read;i++){
            hash ^=buffer[i]; //bitise xor to integrate every byte to the hash
            hash *=FNV_PRIME; //to make the final hash using the base prime multiplier fnv_prime
        }
    }
    fclose(file);
    snprintf(output_hash,HASH_LEN,"%016llx",(unsigned long long)hash); //writing text into output_hash and formatting 64bit hash into hexadecimal with padding to match 16 characters long
    return 0;
}


int scan_directory(const char *dir_path,FileRecord *record,int *recordcount){
    DIR *dir = opendir(dir_path);
    if (dir==NULL){
        return -1;
    }

    struct dirent *entry;
    struct stat path_stat;
    char full_path[MAX_PATH_LEN];

    while((entry=readdir(dir)) != NULL){
        if(*recordcount>= MAX_FILES){
            break;
        }
        if(strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name,"..")==0){
            continue;
        }
        size_t len=strlen(dir_path);
        if(len > 0 && (dir_path[len - 1] == '/' || dir_path[len - 1] == '\\')){
            snprintf(full_path,sizeof(full_path),"%s%s",dir_path,entry->d_name);
        }else{
            snprintf(full_path,sizeof(full_path),"%s/%s",dir_path,entry->d_name);
        }
        if(stat(full_path,&path_stat)!=0){
            continue;
        }

        if(S_ISDIR(path_stat.st_mode)){
            scan_directory(full_path,record,recordcount);
        }else if(S_ISREG(path_stat.st_mode)){
            if(compute_file_hash(full_path,record[*recordcount].hash)==0){
                snprintf(record[*recordcount].filepath,MAX_PATH_LEN,"%s",full_path);
                record[*recordcount].status=STATUS_UNCHANGED;
                gettime(record[*recordcount].timestamp,sizeof(record[*recordcount].timestamp));
                (*recordcount)++;
            }
        }
    }

    closedir(dir);
    return 0;
}