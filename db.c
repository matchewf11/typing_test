#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

// could throw if it can not open the db
void init_db() {
  sqlite3 *db;
  char *errMsg = NULL;

  if (sqlite3_open("typing.db", &db) != SQLITE_OK) {
    perror("Could not open db");
    exit(1);
  }

  // char *sql = 
  //   "CREATE TABLE IF NOT EXISTS results ("
  //   "complete_at TIME,"
  //   "accuracy DOUBLE,"
  //   "time DOUBLE"
  //   ");";
  //
  // int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
  // if (rc != SQLITE_OK) {
  //   perror("Could create tables");
  //   sqlite3_free(err);
  // }

  sqlite3_close(db);
}
