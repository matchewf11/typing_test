#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

// return -1 for error
// else return 0
static int insert_default_phrases(sqlite3* db) {
  return 0;
}


// return -1 for error
static int is_phrases_empty(sqlite3 *db) {
  sqlite3_stmt *stmt;
  char *sql = "SELECT EXISTS (SELECT 1 FROM phrases);";
  int exists = 0;

  int rs = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rs != SQLITE_OK) {
    return -1;
  }

  rs = sqlite3_step(stmt);
  if (rs == SQLITE_ROW) {
    exists = sqlite3_column_int(stmt, 0);
  }
  
  sqlite3_finalize(stmt);

  return exists == 0;
}

// make db and the tables if they do not exist
// make sure that the db is not null
// in client
// make sure to free the db after called
sqlite3 *build_db() {
  sqlite3 *db;
  char *err_msg = NULL;

  int rs = sqlite3_open("typeing.db", &db);
  if (rs != SQLITE_OK) {
    return NULL;
  }

  char *pragma = "PRAGMA foreign_keys = ON;";
  char *sql_phrases = "CREATE TABLE IF NOT EXISTS phrases ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "phrase TEXT"
                      ");";
  char *sql_results = "CREATE TABLE IF NOT EXISTS results("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "accuracy REAL,"
                      "cps REAL,"
                      "phrase_id INTEGER"
                      ");";

  rs = sqlite3_exec(db, pragma, NULL, NULL, &err_msg);
  if (rs != SQLITE_OK) {
    perror(err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }

  rs = sqlite3_exec(db, sql_phrases, NULL, NULL, &err_msg);
  if (rs != SQLITE_OK) {
    perror(err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }

  rs = sqlite3_exec(db, sql_results, NULL, NULL, &err_msg);
  if (rs != SQLITE_OK) {
    perror(err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }

  // TODO: fill in teh phrases if they are not there
  // call the is empty
  // call the insert default phrases

  return db;
}
