#include <sqlite3.h>
#include <unistd.h>

#include "cli.h"
#include "db.h"
#include "term.h"
#include "typing.h"

// fix err handling, and use fprintf(stderr, "%s\n", foo); instead of perror
// keep it from the term.c stuff

int main(int argc, char *argv[]) {
  CliOptions cli_opts = parse_cli(argv, argc);

  switch (cli_opts.cli_cmd) {
  case HELP:
    printf("The valid args are 'help', 'run', and 'stats'\n");
    return 0;
  case RUN: {
    sqlite3 *db = build_db();
    if (db == NULL) {
      perror("could not open db");
      return 1;
    }

    enable_raw_mode();

    int out_len;
    char **str_list = get_phrases(db, 2, &out_len); // gets 2 phrases
    if (str_list == NULL) {
      sqlite3_close(db);
      perror("could not open db");
      return 1;
    }

    TestInfo test_info = start_typing_test(str_list, out_len);
    print_test_info(test_info);

    // do I need to handle error?
    int _rc = store_results(db, test_info);

    free_phrases(str_list, out_len);
    sqlite3_close(db);
    return 0;
  }
  case STATS: {
    sqlite3 *db = build_db();
    if (db == NULL) {
      perror("could not open db");
      return 1;
    }
    TestAvgStats stats;
    int rc = get_results(db, &stats);
    if (rc == -1) {
      sqlite3_close(db);
      perror("something bad happened");
      return 1;
    }
    sqlite3_close(db);
    printf("Test Average Stats:\n");
    printf("  Accuracy (last 5): %.2f\n", stats.acc_5);
    printf("  CPS (last 5): %.2f\n", stats.cps_5);
    printf("  Accuracy (last 12): %.2f\n", stats.acc_12);
    printf("  CPS (last 12): %.2f\n", stats.cps_12);
    printf("  Accuracy (all): %.2f\n", stats.acc_all);
    printf("  CPS (all): %.2f\n", stats.cps_all);
    return 0;
  }

  case ERROR:
    perror(cli_opts.err_msg);
    return 1;
  }
}
