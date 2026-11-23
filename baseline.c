#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fim_types.h"

int save_baseline(const char *baseline_path, const FileRecord *records,int count){

    FILE *fptr;
    fptr = fopen(baseline_path,"w");       //Creates new file which stores hashes and its data

    if(fptr == NULL){       //If file isnt made, it returns -1
        return -1;     
    }


    for(int x = 0; x < count; x++){
        fprintf(fptr, "%s,%s,%s\n", records[x].filepath, records[x].hash, records[x].timestamp);    //Adds each record in increasng order to file. Starts from line 0, index 0
    }

    fclose(fptr);

    return 0;   //File saved successfully
}

int load_baseline(const char *baseline_path, FileRecord *records, int *count){

    *count = 0;
    char line[1024];
    FILE *fptr;
    fptr = fopen(baseline_path,"r");

    if(fptr == NULL){
        return -1;
    }

    while (fgets(line, sizeof(line), fptr) != NULL) {
        char temp_path[MAX_PATH_LEN];
        char temp_hash[HASH_LEN];
        char temp_time[32];

        int loadedCount = sscanf(line," %511[^,],%16[^,],%31[^,\n]", temp_path, temp_hash, temp_time); //Whenever it encounters a comma it removes the , and puts the values before it into the variable.

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

int compare_with_baseline(const FileRecord *baseline, int base_count, FileRecord *current, int curr_count, FileRecord *report, int *report_count){
    
    *report_count = 0;      //Ensures it starts from 0 regardless of parameter

    //Checks current files against base files and assigns modified if changes in hash found
    for(int x = 0; x < curr_count && *report_count < MAX_FILES; x++){        //Iterates through every record in current struct
        FileStatus status = STATUS_NEW;

        for(int y = 0; y < base_count; y++){        //Iterates through every record in base struct
            if(strcmp(current[x].filepath, baseline[y].filepath) == 0){
                if(strcmp(current[x].hash, baseline[y].hash) == 0){
                    status = STATUS_UNCHANGED;
                }
                else{
                    status = STATUS_MODIFIED;
                }
                break;
            }
        }

        report[*report_count] = current[x];
        report[*report_count].status = status;
        (*report_count)++;      //Passes back updated number of records
    }

    //Checks for deleted files in current record and adds them to report
    for(int x = 0; x < base_count && *report_count < MAX_FILES; x++){
        int found = 0;

        for(int y = 0; y < curr_count; y++){
            if(strcmp(baseline[x].filepath, current[y].filepath) == 0){
                found = 1;
                break;
            }
        }

        if(!found){
            report[*report_count] = baseline[x];
            report[*report_count].status = STATUS_DELETED;
            (*report_count)++;
        }
    }

    return 0;
}

int export_report(const char *report_path, const FileRecord *report, int report_count){

    FILE *fptr;
    fptr = fopen(report_path,"w");

    if(fptr == NULL){
        return -1;
    }

    for(int x = 0; x < report_count; x++){

        char *status_string = "placeholder";

        switch(report[x].status){       //Converts enum to their respective strings
            case STATUS_UNCHANGED:
                status_string = "UNCHANGED";
                break;    

            case STATUS_MODIFIED:
                status_string = "MODIFIED";
                break;
            
            case STATUS_NEW:
                status_string = "NEW";
                break;
            
            case STATUS_DELETED:
                status_string = "DELETED";
                break;
            
            default:
                status_string = "unknown";
                break;
        }


        fprintf(fptr, "STATUS: %s PATH: %s HASH: %s TIME: %s\n", status_string, report[x].filepath, report[x].hash, report[x].timestamp);
    }

    fclose(fptr);
    return 0;
}

