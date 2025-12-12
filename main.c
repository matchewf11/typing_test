#include <bits/time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sqlite3.h>

#include "input_letter.h"
#include "term.h"

int main(void) {

  sqlite3 *db;
  char *err = NULL;

  // Open/Create db
  if (sqlite3_open("typing.db", &db) != SQLITE_OK) {
    perror("Could not open db");
    exit(1);
  }

  char *sql = 
    "CREATE TABLE IF NOT EXISTS results ("
    "complete_at TIME,"
    "accuracy DOUBLE,"
    "time DOUBLE"
    ");";

  int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    perror("Could create tables");
    sqlite3_free(err);
  }

  // close the db
  sqlite3_close(db);

  enable_raw_mode();

  char *str = "these are some words that you can type";
  InputLetter *letter_list = input_letter_from_str(str);
  if (!letter_list) {
    perror("could not malloc");
    exit(1);
  }

  sleep(1);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  int at = 0;
  while (at < strlen(str)) {
    clear_term();
    print_input_letter(letter_list, strlen(str));
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      perror("could not read the thing");
      exit(1);
    } else if (c == '\0') {
      continue;
    }
    letter_set_status(letter_list, at, c);
    at++;
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  clear_term();

  double acc = letter_accuracy(letter_list, strlen(str));
  printf("Accuracy: %f\r\n", acc);

  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Time: %f seconds\r\n", time_taken);
  printf("CPS: %f\r\n", strlen(str) / time_taken);

  free(letter_list);
  return 0;
}
