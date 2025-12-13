#include <sqlite3.h>
#include <unistd.h>

#include "db.h"
#include "term.h"
#include "typing.h"

int main(int argc, char *argv[]) {
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
