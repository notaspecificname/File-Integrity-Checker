
#ifndef CLI_H
#define CLI_H

#include "fim_types.h"

typedef struct {
    char target_dir[MAX_PATH_LEN];
    char baseline_file[MAX_PATH_LEN];
    char report_file[MAX_PATH_LEN];
    int scan_interval_sec;
    int is_running;
} CliState;

void init_cli_state(CliState *state);

void display_menu_and_results(const FileRecord *report, int report_count);

void run_periodic_scanner(CliState *state,
                          const FileRecord *baseline,
                          int base_count,
                          FileRecord *report,
                          int *report_count);

#endif
