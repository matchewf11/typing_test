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
  case STATS:
    printf("This is the stats menu\n");
    return 0;
  case ERROR:
    perror(cli_opts.err_msg);
    return 1;
  }
}
