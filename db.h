#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <stdlib.h>

// make db and the tables if they do not exist
// make sure that the db is not null
// in client
// make sure to free the db after called
sqlite3 *build_db();

// Return a list of malloced string (free the whole list, then the list)
// returns n phrases
// gives the len of teh phrases in out
// ensure char is not null
char** get_phrases(sqlite3 *db, int n, int *out);

static inline void free_phrases(char** phrases, int len) {
  for (int i = 0; i < len; i++) {
    free(phrases[i]);
  }
  free(phrases);
}

#endif
