#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

void clear_phrases() {

}

void add_all_phrases() {

}


// could throw if it can not open the db
void init_db() {
  sqlite3 *db;
  char *err_msg = NULL;

  if (sqlite3_open("typing.db", &db) != SQLITE_OK) {
    perror("Could not open db");
    exit(1);
  }
char *sql = "CREATE TABLE IF NOT EXISTS phrases ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "phrase TEXT"
    ");";

  // makea table with results
  // id of phrase used
  // cps
  // accuracy

  int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
  if (rc != SQLITE_OK) {
    perror("Could not create tables");
    sqlite3_free(err_msg);
  }

  // delete everything and reinsert it? for teh phrases

  sqlite3_close(db);
}
