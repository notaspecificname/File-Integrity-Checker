#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "filing.h"

typedef struct{
    char fileName[128];
    char hash[9];
} fileRecord;

int main(){
    char path[100] = "/home/notaspecificname/Pictures/Screenshots/";
    scanDirectoryAndLoadFiles(path);
}

void scanDirectoryAndLoadFiles(char folderPath[100]){

    int fileCount = 0;
    char fullPath[290];
    char fileContent[9000];
    char hash[9];  //Will store intermediate hash value

    fileRecord baseline[100];

    DIR *dir = opendir(folderPath);     //Like FILE variable type we have set a pointer dir to folderPath
    
    if(dir == NULL){
        return;      //Folder doesnot exist
    }
    
    struct dirent *entry;       //Built in structure which holds information about the next file in the folder
    while((entry = readdir(dir))!= NULL){
        
        //Skips the "." and ".." directory names
        if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0){
            continue;
        }
        
        snprintf(fullPath,sizeof(fullPath),"%s/%s",folderPath,entry->d_name); //Appends the file name to directory path to create file path
        
        FILE *fptr = fopen(fullPath,"rb");
        if(fptr != NULL){
            size_t bytesRead = fread(fileContent, 1, sizeof(fileContent) -1, fptr);    //Copies binary data directly to fileContent. No need for complex string functions
            fileContent[bytesRead] =  '\0';

            //Pass fileContent and number of bytes to hashing function. hashing function will return a 32 character hash value.
            //hash = hash_func()
            //strncpy(baseline[fileCount].hash ,hash ,sizeof(baseline[fileCount].hash) -1);   //Store the hash into the struct

            fclose(fptr);

            strncpy(baseline[fileCount].fileName, entry->d_name,sizeof(baseline[fileCount].fileName) -1);   //Store the File Name into the struct
            fileCount++;
        }


    if(fileCount> 99){
        break;
    }
    }
    closedir(dir);
}

    
    




