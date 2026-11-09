#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "filing.h"

int main(){
    char path[100] = "/home/notaspecificname/Pictures/Screenshots/";
    scanDirectoryAndLoadFiles(path);
}

void scanDirectoryAndLoadFiles(char folderPath[100]){

    int fileCount = 0;
    char fullPath[290];

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
        
        sprintf(fullPath,290,"%s/%s",folderPath,entry->d_name); //Appends the file name to directory path to create file path
        
        FILE *fptr = fopen(fullPath,"rb");
        


        
                                       
    }

    
    



}
