#include <unistd.h>
#include <sqlite3.h>

#include "term.h"
#include "db.h"
#include "typing.h"

int main(void) {

  // testing it out
  sqlite3 *db = build_db();
  if (db == NULL) {
    perror("could not open db");
  }

  sqlite3_close(db);
  // end of testing it out

  enable_raw_mode();

  char *str_list[] = {"this is the first", "this is the second.",
                      "this is the third"};
  start_typing_test(str_list, 3);

  return 0;
}
