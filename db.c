#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>


// return -1 if error
// return the count otherwise
static int phrase_count(sqlite3 *db) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT COUNT(*) FROM phrases";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    return -1;
  }

  int count = 0;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    count = sqlite3_column_int(stmt, 0);
  }

  sqlite3_finalize(stmt);
  return count;
}

// could throw if it can not open the db
void init_db() {
  sqlite3 *db;
  char *err_msg = NULL;

  if (sqlite3_open("typing.db", &db) != SQLITE_OK) {
    perror("Could not open db");
    exit(1);
  }

  char *sql = 
    "CREATE TABLE IF NOT EXISTS phrases ("
    "phrase TEXT"
    ");";

  int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
  if (rc != SQLITE_OK) {
    perror("Could not create tables");
    sqlite3_free(err_msg);
  }

  int phrases = phrase_count(db);
  if (phrases == -1) {
    perror("Could not access phrases");
  } else if (phrases == 0) {
    // insert the stuff
  }

  sqlite3_close(db);
}
