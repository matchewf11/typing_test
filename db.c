#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return -1 for error
// else return 0
static int insert_default_phrases(sqlite3 *db) {
  char *sql = "INSERT INTO phrases (phrase) VALUES"
              "('The cat slept on the warm windowsill.'),"
              "('A cold breeze drifted through the open door.'),"
              "('She wrote a quick note before leaving.'),"
              "('The river moved slowly under the old bridge.'),"
              "('He found a small key hidden in the grass.'),"
              "('The sky turned orange as the sun went down.'),"
              "('Birds gathered on the fence to sing.'),"
              "('A quiet moment passed between them.'),"
              "('The coffee smelled strong and rich.'),"
              "('Rain tapped lightly on the metal roof.'),"
              "('The dog barked loudly at the passing car.'),"
              "('Leaves rustled as the wind blew through the trees.'),"
              "('She hummed a tune while cooking dinner.'),"
              "('He jotted down numbers on a scrap of paper.'),"
              "('The candle flickered in the dark room.')";
  char *err_msg = NULL;

  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    sqlite3_free(err_msg);
    return -1;
  }

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

  int is_empty = is_phrases_empty(db);
  if (is_empty == -1) {
    sqlite3_close(db);
    return NULL;
  }

  if (is_empty) {
    rs = insert_default_phrases(db);
    if (rs == -1) {
      sqlite3_close(db);
      return NULL;
    }
  }

  return db;
}

char** get_phrases(sqlite3 *db, int n, int *out) {
  char *sql = "SELECT phrase FROM phrases ORDER BY RANDOM() LIMIT ?";
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    *out = 0;
    return NULL;
  }

  sqlite3_bind_int(stmt, 1, n);

  char **phrases = malloc(sizeof(char*) * n);
  if (!phrases) {
    sqlite3_finalize(stmt);
    *out = 0;
    return NULL;
  }

  int count = 0;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW && count < n) {
    const unsigned char *text = sqlite3_column_text(stmt, 0);
    if (!text) continue;

    size_t len = strlen((const char*)text);
    char *copy = malloc(len + 1);
    if (!copy) continue;

    strcpy(copy, (const char*) text);
    phrases[count++] = copy;
  }

  sqlite3_finalize(stmt);
  *out = count;
  return phrases;
}
