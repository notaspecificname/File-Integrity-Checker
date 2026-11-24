#include <stdio.h>
#include <string.h>
#include "fim_types.h"

/* Prototypes from cli.c */
void init_cli_state(CliState *state);
void display_menu_and_results(const FileRecord *report, int report_count);
void run_periodic_scanner(CliState *state,
                          const FileRecord *baseline,
                          int base_count,
                          FileRecord *report,
                          int *report_count);

/* Prototypes from scanner.c (Araiz) */
int scan_directory(const char *dir_path, FileRecord *records, int *record_count);

/* Prototypes from baseline.c (Moosa) */
int save_baseline(const char *baseline_path, const FileRecord *records, int count);
int load_baseline(const char *baseline_path, FileRecord *records, int *count);
int compare_with_baseline(const FileRecord *baseline,
                          int base_count,
                          FileRecord *current,
                          int curr_count,
                          FileRecord *report,
                          int *report_count);
int export_report(const char *report_path, const FileRecord *report, int report_count);

static void trim_newline(char *str) {
    if (str != NULL) {
        str[strcspn(str, "\r\n")] = '\0';
    }
}

int main(void) {
    FileRecord baseline_records[MAX_FILES];
    FileRecord current_records[MAX_FILES];
    FileRecord report_records[MAX_FILES];

    int baseline_count = 0;
    int current_count = 0;
    int report_count = 0;

    CliState cli_state;
    char input_buf[MAX_PATH_LEN];
    int choice = 0;

    init_cli_state(&cli_state);

    if (load_baseline(cli_state.baseline_file, baseline_records, &baseline_count) == 0 && baseline_count > 0) {
    printf("Loaded %d baseline record(s) from '%s'.\n",
           baseline_count, cli_state.baseline_file);
    } 
    else {
    baseline_count = 0;
    printf("No existing baseline found at '%s'. Create one via Option 2.\n",
           cli_state.baseline_file);
    }

    while (cli_state.is_running) {
        display_menu_and_results(report_records, report_count);

        if (fgets(input_buf, sizeof(input_buf), stdin) == NULL) {
            break;
        }
        trim_newline(input_buf);

        if (sscanf(input_buf, "%d", &choice) != 1) {
            printf("[WARN] Please enter a valid number (1-6).\n");
            continue;
        }

        switch (choice) {
            case 1: {
                printf("Enter new target directory path: ");
                if (fgets(input_buf, sizeof(input_buf), stdin) != NULL) {
                    trim_newline(input_buf);
                    if (input_buf[0] != '\0') {
                        snprintf(cli_state.target_dir, MAX_PATH_LEN, "%s", input_buf);
                        printf("Target directory updated to '%s'.\n", cli_state.target_dir);
                    } else {
                        printf("[WARN] Empty path ignored.\n");
                    }
                }
                break;
            }
            case 2: {
                baseline_count = 0;
                if (scan_directory(cli_state.target_dir, baseline_records, &baseline_count) == 0) {
                    if (save_baseline(cli_state.baseline_file, baseline_records, baseline_count) == 0) {
                        printf("Baseline saved (%d file(s)) to '%s'.\n",
                               baseline_count, cli_state.baseline_file);
                    } else {
                        printf("[ERROR] Failed to write baseline to '%s'.\n",
                               cli_state.baseline_file);
                    }
                } else {
                    printf("[ERROR] Failed to scan target directory '%s'.\n",
                           cli_state.target_dir);
                }
                break;
            }
            case 3: {
                current_count = 0;
                report_count = 0;
                if (scan_directory(cli_state.target_dir, current_records, &current_count) == 0) {
                    compare_with_baseline(baseline_records, baseline_count,
                                          current_records, current_count,
                                          report_records, &report_count);
                    printf("Manual scan complete (%d report entry/entries).\n", report_count);
                } else {
                    printf("[ERROR] Failed to scan target directory '%s'.\n",
                           cli_state.target_dir);
                }
                break;
            }
            case 4: {
                run_periodic_scanner(&cli_state,
                                     baseline_records, baseline_count,
                                     report_records, &report_count);
                break;
            }
            case 5: {
                if (export_report(cli_state.report_file, report_records, report_count) == 0) {
                    printf("Report exported to '%s'.\n", cli_state.report_file);
                } else {
                    printf("[ERROR] Failed to export report to '%s'.\n",
                           cli_state.report_file);
                }
                break;
            }
            case 6: {
                cli_state.is_running = 0;
                printf("Exiting File Integrity Monitor.\n");
                break;
            }
            default: {
                printf("[WARN] Invalid choice '%d'. Please select 1-6.\n", choice);
                break;
            }
        }
    }

    return 0;
}