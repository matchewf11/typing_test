#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <stdlib.h>

#include "typing.h"

typedef struct {
  double acc_5;
  double cps_5;
  double acc_12;
  double cps_12;
  double acc_all;
  double cps_all;
} TestAvgStats;

// make db and the tables if they do not exist
// make sure that the db is not null
// in client
// make sure to free the db after called
sqlite3 *build_db();

// Return a list of malloced string (free the whole list, then the list)
// returns n phrases
// gives the len of teh phrases in out
// ensure char is not null
char **get_phrases(sqlite3 *db, int n, int *out);

// reutnr -1 for an error, else return 0
int store_results(sqlite3 *db, TestInfo info);

// return -1 if error else 0
int get_results(sqlite3 *db, TestAvgStats *stats);

static inline void free_phrases(char **phrases, int len) {
  for (int i = 0; i < len; i++) {
    free(phrases[i]);
  }
  free(phrases);
}

#endif
