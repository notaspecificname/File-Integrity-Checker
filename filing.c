#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int filePass(char *filePath){

    FILE *fptr;
    fptr = fopen(filePath, "r");

    if(fptr == NULL){
        return -1;
    }
    else{
        fclose(fptr);
        return 1;
    }
}
