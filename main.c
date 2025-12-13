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

  start_typing_test();

  return 0;
}
