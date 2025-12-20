#include <sqlite3.h>
#include <string.h>

#include "../typing_test/typing_test.h"

// <https://sqlite.org/cintro.html>

// Returns SQLITE_OK(0) if good
// else will return error code
// err code can be printed with sqlite3_errmsg();
// be sure to sqlite3_close() when done (if succeeds)
int db_open(sqlite3 **db) {
  int rs = sqlite3_open("typing_test.db", db);
  if (rs != SQLITE_OK) {
    return rs;
  }

  char *sql = "PRAGMA foreign_keys = ON;";
  rs = sqlite3_exec(*db, sql, NULL, NULL, NULL);
  if (rs != SQLITE_OK) {
    sqlite3_close(*db);
    return rs;
  }

  return SQLITE_OK;
}

// Init the schema of db
// Return SQLITE_OK(0) if success
// Else prints err code (sqlite3_errmsg())
int db_init_schema(sqlite3 *db) {
  const char *sql =
      "CREATE TABLE IF NOT EXISTS phrases(phrase TEXT NOT NULL UNIQUE);"
      "CREATE TABLE IF NOT EXISTS results (accuracy REAL, cps REAL, "
      "completed_at DATETIME DEFAULT CURRENT_TIMESTAMP);";
  return sqlite3_exec(db, sql, NULL, NULL, NULL);
}

// Checks if the phrases table is empty
// return 0 if no
// return 1 if yes
// return -1 if error
int phrases_table_empty(sqlite3 *db) {
  const char *sql = "SELECT EXISTS (SELECT 1 FROM phrases)";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    return -1;
  }

  if (sqlite3_step(stmt) != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return -1;
  }

  int is_phrases = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  return is_phrases == 0;
}

// Return 0 on success
// Return -1 on failure
int insert_default_phrases(sqlite3 *db) {
  const char *sql = "INSERT INTO phrases (phrase) VALUES"
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
                    "('The candle flickered in the dark room.');";
  return sqlite3_exec(db, sql, NULL, NULL, NULL) == SQLITE_OK ? 0 : -1;
}

// Return 0 on success
// Return -1 on failure
int store_results(sqlite3 *db, TestInfo info) {
  const char *sql = "INSERT INTO results (accuracy, cps) VALUES (?, ?);";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    return -1;
  }

  if (sqlite3_bind_double(stmt, 1, info.accuracy) != SQLITE_OK ||
      sqlite3_bind_double(stmt, 2, info.cps) != SQLITE_OK ||
      sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return -1;
  }

  sqlite3_finalize(stmt);
  return 0;
}

// int get_results(sqlite3 *db, TestAvgStats *stats) {
//   sqlite3_stmt *stmt;
//   char *sql = "SELECT "
//               "(SELECT AVG(accuracy) FROM (SELECT accuracy FROM results "
//               "ORDER BY completed_at DESC LIMIT 5)) as acc_5, "
//               "(SELECT AVG(cps) FROM (SELECT cps FROM results "
//               "ORDER BY completed_at DESC LIMIT 5)) as cps_5, "
//               "(SELECT AVG(accuracy) FROM (SELECT accuracy FROM results "
//               "ORDER BY completed_at DESC LIMIT 12)) as acc_12, "
//               "(SELECT AVG(cps) FROM (SELECT cps FROM results "
//               "ORDER BY completed_at DESC LIMIT 12)) as cps_12, "
//               "AVG(accuracy) as acc_all, "
//               "AVG(cps) as cps_all "
//               "FROM results";
//
//   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//   if (rc != SQLITE_OK) {
//     return -1;
//   }
//
//   rc = sqlite3_step(stmt);
//   if (rc != SQLITE_ROW) {
//     sqlite3_finalize(stmt);
//     return -1;
//   }
//
//   if (sqlite3_column_type(stmt, 0) == SQLITE_NULL) {
//     sqlite3_finalize(stmt);
//     return -1;
//   }
//
//   stats->acc_5 = sqlite3_column_double(stmt, 0);
//   stats->cps_5 = sqlite3_column_double(stmt, 1);
//   stats->acc_12 = sqlite3_column_double(stmt, 2);
//   stats->cps_12 = sqlite3_column_double(stmt, 3);
//   stats->acc_all = sqlite3_column_double(stmt, 4);
//   stats->cps_all = sqlite3_column_double(stmt, 5);
//
//   sqlite3_finalize(stmt);
//   return 0;
// }

// also be sure to free each string then the list (malloced list of malloced
// string). must check if main pointer is NULL (all string gurantted to be
// properly malloced). n is the amount of phrases you wanted, out is how many
// you got
// char **phrases_from_db(sqlite3 *db, int n, int *out) {
//   char *sql = "SELECT phrase FROM phrases ORDER BY RANDOM() LIMIT ?";
//   sqlite3_stmt *stmt;
//
//   // out is here so that it does it before any returns
//   *out = 0;
//
//   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//   if (rc != SQLITE_OK) {
//     return NULL;
//   }
//
//   rc = sqlite3_bind_int(stmt, 1, n);
//   if (rc != SQLITE_OK) {
//     sqlite3_finalize(stmt);
//     return NULL;
//   }
//
//   char **phrases = malloc(sizeof(char *) * n);
//   if (phrases == NULL) {
//     sqlite3_finalize(stmt);
//     return NULL;
//   }
//
//   while (sqlite3_step(stmt) == SQLITE_ROW) {
//     const unsigned char *phrase = sqlite3_column_text(stmt, 0);
//
//     char *temp = malloc(sizeof(char) * (strlen((const char *)phrase) + 1));
//     if (temp == NULL) {
//       continue;
//     }
//
//     strcpy(temp, (const char *)phrase);
//     phrases[*out] = temp;
//     (*out)++;
//   }
//
//   (*out)++; // goes from i to len
//   sqlite3_finalize(stmt);
//   return phrases;
// }

//   char *sql = "SELECT phrase FROM phrases ORDER BY RANDOM() LIMIT ?";
//   sqlite3_stmt *stmt;
//
//   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//   if (rc != SQLITE_OK) {
//     *out = 0;
//     return NULL;
//   }
//
//   sqlite3_bind_int(stmt, 1, n);
//
//   char **phrases = malloc(sizeof(char *) * n);
//   if (!phrases) {
//     sqlite3_finalize(stmt);
//     *out = 0;
//     return NULL;
//   }
//
//   int count = 0;
//   while ((rc = sqlite3_step(stmt)) == SQLITE_ROW && count < n) {
//     const unsigned char *text = sqlite3_column_text(stmt, 0);
//     if (!text)
//       continue;
//
//     size_t len = strlen((const char *)text);
//     char *copy = malloc(len + 1);
//     if (!copy)
//       continue;
//
//     strcpy(copy, (const char *)text);
//     phrases[count++] = copy;
//   }
//
//   sqlite3_finalize(stmt);
//   *out = count;
//   return phrases;
// }

// db.c
// ```C
// #include <sqlite3.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// #include "db.h"
// #include "typing.h"
//
//
// ```
//
// db.h
// ```C
// #ifndef DB_H
// #define DB_H
//
// #include <sqlite3.h>
// #include <stdlib.h>
//
// #include "typing.h"
//
// typedef struct {
//   double acc_5;
//   double cps_5;
//   double acc_12;
//   double cps_12;
//   double acc_all;
//   double cps_all;
// } TestAvgStats;
//
// // make db and the tables if they do not exist
// // make sure that the db is not null
// // in client
// // make sure to free the db after called
// sqlite3 *build_db();
//
// // Return a list of malloced string (free the whole list, then the list)
// // returns n phrases
// // gives the len of teh phrases in out
// // ensure char is not null
// char **get_phrases(sqlite3 *db, int n, int *out);
//
// // reutnr -1 for an error, else return 0
// int store_results(sqlite3 *db, TestInfo info);
//
// // return -1 if error else 0
// int get_results(sqlite3 *db, TestAvgStats *stats);
//
// static inline void free_phrases(char **phrases, int len) {
//   for (int i = 0; i < len; i++) {
//     free(phrases[i]);
//   }
//   free(phrases);
// }
//
// #endif
// ```
