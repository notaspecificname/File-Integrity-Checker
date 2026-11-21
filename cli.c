#include <stdio.h>
#include <string.h>
#include <fim_types.h>

void init_cli_state(CliState *state) {
    if (state == NULL) {
        return;
    }

    memset(state, 0, sizeof(*state));

    snprintf(state->target_dir, MAX_PATH_LEN, "./monitored_dir");
    snprintf(state->baseline_file, MAX_PATH_LEN, "baseline.csv");
    snprintf(state->report_file, MAX_PATH_LEN, "scan_report.txt");

    state->scan_interval_sec = 5;
    state->is_running = 1;
}

static const char *status_to_label(FileStatus status) {
    switch (status) {
        case STATUS_UNCHANGED:
            return "[UNCHANGED]";
        case STATUS_MODIFIED:
            return "[MODIFIED]";
        case STATUS_NEW:
            return "[NEW]";
        case STATUS_DELETED:
            return "[DELETED]";
        default:
            return "[UNKNOWN]";
    }
}

void display_menu_and_results(const FileRecord *report, int report_count) {
    int i;

    printf("\n==================== SCAN RESULTS ====================\n");
    if (report != NULL && report_count > 0) {
        printf("%-13s %-40s %-18s %-19s\n", "STATUS", "FILEPATH", "HASH", "TIMESTAMP");
        printf("-----------------------------------------------------------------------------------------------\n");
        for (i = 0; i < report_count; i++) {
            printf("%-13s %-40s %-18s %-19s\n",
                   status_to_label(report[i].status),
                   report[i].filepath,
                   report[i].hash,
                   report[i].timestamp);
        }
    } else {
        printf("No scan results to display yet.\n");
    }
    printf("======================================================\n\n");

    printf("--- File Integrity Monitor Menu ---\n");
    printf("1. Set Target Directory\n");
    printf("2. Create/Update Baseline\n");
    printf("3. Run Manual Scan\n");
    printf("4. Start Periodic Auto-Scan\n");
    printf("5. Export Report\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
}
