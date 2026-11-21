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

