#include <sqlite3.h>
#include <unistd.h>

#include "db.h"
#include "term.h"
#include "typing.h"

int main(void) {

  sqlite3 *db = build_db();
  if (db == NULL) {
    perror("could not open db");
    return 1;
  }

  enable_raw_mode();

  int out_len;
  char **str_list = get_phrases(db, 5, &out_len); // gets 5 phrases
  if (str_list == NULL) {
    perror("could not open db");
    return 1;
  }

  TestInfo test_info = start_typing_test(str_list, out_len);
  print_test_info(test_info);

  free_phrases(str_list, out_len);
  sqlite3_close(db);

  return 0;
}
