put everything in teh build folder
test the code


db.c
```C
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "typing.h"

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

  int rs = sqlite3_open("typing.db", &db);
  if (rs != SQLITE_OK) {
    return NULL;
  }

  char *pragma = "PRAGMA foreign_keys = ON;";
  char *sql_phrases = "CREATE TABLE IF NOT EXISTS phrases ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "phrase TEXT"
                      ");";
  char *sql_results = "CREATE TABLE IF NOT EXISTS results ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "accuracy REAL,"
                      "cps REAL,"
                      "completed_at DATETIME DEFAULT CURRENT_TIMESTAMP"
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

char **get_phrases(sqlite3 *db, int n, int *out) {
  char *sql = "SELECT phrase FROM phrases ORDER BY RANDOM() LIMIT ?";
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    *out = 0;
    return NULL;
  }

  sqlite3_bind_int(stmt, 1, n);

  char **phrases = malloc(sizeof(char *) * n);
  if (!phrases) {
    sqlite3_finalize(stmt);
    *out = 0;
    return NULL;
  }

  int count = 0;
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW && count < n) {
    const unsigned char *text = sqlite3_column_text(stmt, 0);
    if (!text)
      continue;

    size_t len = strlen((const char *)text);
    char *copy = malloc(len + 1);
    if (!copy)
      continue;

    strcpy(copy, (const char *)text);
    phrases[count++] = copy;
  }

  sqlite3_finalize(stmt);
  *out = count;
  return phrases;
}

int store_results(sqlite3 *db, TestInfo info) {
  sqlite3_stmt *stmt;
  char *sql = "INSERT INTO results (accuracy, cps) VALUES (?, ?)";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    return -1;
  }

  sqlite3_bind_double(stmt, 1, info.accuracy);
  sqlite3_bind_double(stmt, 2, info.cps);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  return rc == SQLITE_DONE ? 0 : -1;
}

int get_results(sqlite3 *db, TestAvgStats *stats) {
  sqlite3_stmt *stmt;
  char *sql = "SELECT "
              "(SELECT AVG(accuracy) FROM (SELECT accuracy FROM results "
              "ORDER BY completed_at DESC LIMIT 5)) as acc_5, "
              "(SELECT AVG(cps) FROM (SELECT cps FROM results "
              "ORDER BY completed_at DESC LIMIT 5)) as cps_5, "
              "(SELECT AVG(accuracy) FROM (SELECT accuracy FROM results "
              "ORDER BY completed_at DESC LIMIT 12)) as acc_12, "
              "(SELECT AVG(cps) FROM (SELECT cps FROM results "
              "ORDER BY completed_at DESC LIMIT 12)) as cps_12, "
              "AVG(accuracy) as acc_all, "
              "AVG(cps) as cps_all "
              "FROM results";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    return -1;
  }

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return -1;
  }

  if (sqlite3_column_type(stmt, 0) == SQLITE_NULL) {
    sqlite3_finalize(stmt);
    return -1;
  }

  stats->acc_5 = sqlite3_column_double(stmt, 0);
  stats->cps_5 = sqlite3_column_double(stmt, 1);
  stats->acc_12 = sqlite3_column_double(stmt, 2);
  stats->cps_12 = sqlite3_column_double(stmt, 3);
  stats->acc_all = sqlite3_column_double(stmt, 4);
  stats->cps_all = sqlite3_column_double(stmt, 5);

  sqlite3_finalize(stmt);
  return 0;
}
```

db.h
```C
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
```

input_letter.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_letter.h"

typedef enum { RIGHT, WRONG, TODO } LetterStatus;

struct input_letter {
  char character;
  LetterStatus status;
};

InputLetterList letter_list(char **str_list, int len) {
  int total_len = len - 1;
  for (int i = 0; i < len; i++) {
    total_len += strlen(str_list[i]);
  }

  InputLetter *list = malloc(sizeof(InputLetter) * total_len);
  if (list == NULL) {
    return (InputLetterList){.list = NULL, .len = 0};
  }

  int list_i = 0;

  // iterates over phrases
  for (int i = 0; i < len; i++) {

    // iterate over phrase chars
    for (int j = 0; j < strlen(str_list[i]); j++) {
      list[list_i].status = TODO;
      list[list_i].character = str_list[i][j];
      list_i++;
    }

    // execpt for last iteration
    if (i != len - 1) {
      list[list_i].status = TODO;
      list[list_i].character = ' ';
      list_i++;
    }
  }

  return (InputLetterList){.list = list, .len = total_len};
}

void print_input_letter(const InputLetter *const list, int len) {
  for (int i = 0; i < len; i++) {
    InputLetter curr_letter = list[i];
    char curr_char = curr_letter.character;
    switch (curr_letter.status) {
    case RIGHT:
      printf("\033[1;32m%c\033[0m", curr_char);
      break;
    case WRONG:
      printf("\033[1;31m%c\033[0m", curr_char);
      break;
    case TODO:
      printf("%c", curr_char);
      break;
    }

    if (curr_char == ' ' && i != 0 && list[i - 1].character == '.' &&
        i != len - 1) {
      printf("\r\n");
    }
  }
  printf("\r\n");
}

void letter_set_status(InputLetter *list, int i, char c) {
  if (list[i].character == c) {
    list[i].status = RIGHT;
  } else {
    list[i].status = WRONG;
  }
}

double letter_accuracy(const InputLetter *const list, int len) {
  int right = 0;
  int wrong = 0;

  for (int i = 0; i < len; i++) {
    LetterStatus ls = list[i].status;
    if (ls == RIGHT) {
      right++;
    } else if (ls == WRONG) {
      wrong++;
    }
  }

  return (100 * right) / ((float)(right + wrong));
}
```

input_letter.h
```C
#ifndef INPUT_LETTER_H
#define INPUT_LETTER_H

typedef struct input_letter InputLetter;

typedef struct {
  InputLetter* list;
  int len;
} InputLetterList;

// print a letter list
void print_input_letter(const InputLetter *const list, int len);

// set thing at i to right
void letter_set_status(InputLetter *list, int i, char c);

// get the accuracy
double letter_accuracy(const InputLetter *const list, int len);

// make a function that takes in **char and return InputLetter*
// client can use one free on the pointer they get
// make sure they check for null
// add spaces in between '. F'
InputLetterList letter_list(char **str_list, int len);

#endif
```

main.c
```C
#include <sqlite3.h>
#include <unistd.h>

#include "cli.h"
#include "db.h"
#include "term.h"
#include "typing.h"

// fix err handling, and use fprintf(stderr, "%s\n", foo); instead of perror
// keep it from the term.c stuff

int main(int argc, char *argv[]) {
  CliOptions cli_opts = parse_cli(argv, argc);

  switch (cli_opts.cli_cmd) {
  case HELP:
    printf("The valid args are 'help', 'run', and 'stats'\n");
    return 0;
  case RUN: {
    sqlite3 *db = build_db();
    if (db == NULL) {
      perror("could not open db");
      return 1;
    }

    enable_raw_mode();

    int out_len;
    char **str_list = get_phrases(db, 2, &out_len); // gets 2 phrases
    if (str_list == NULL) {
      sqlite3_close(db);
      perror("could not open db");
      return 1;
    }

    TestInfo test_info = start_typing_test(str_list, out_len);
    print_test_info(test_info);

    // do I need to handle error?
    int _rc = store_results(db, test_info);

    free_phrases(str_list, out_len);
    sqlite3_close(db);
    return 0;
  }
  case STATS: {
    sqlite3 *db = build_db();
    if (db == NULL) {
      perror("could not open db");
      return 1;
    }
    TestAvgStats stats;
    int rc = get_results(db, &stats);
    if (rc == -1) {
      sqlite3_close(db);
      perror("something bad happened");
      return 1;
    }
    sqlite3_close(db);
    printf("Test Average Stats:\n");
    printf("  Accuracy (last 5): %.2f\n", stats.acc_5);
    printf("  CPS (last 5): %.2f\n", stats.cps_5);
    printf("  Accuracy (last 12): %.2f\n", stats.acc_12);
    printf("  CPS (last 12): %.2f\n", stats.cps_12);
    printf("  Accuracy (all): %.2f\n", stats.acc_all);
    printf("  CPS (all): %.2f\n", stats.cps_all);
    return 0;
  }

  case ERROR:
    perror(cli_opts.err_msg);
    return 1;
  }
}
```

term.c
```C
// <https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html>

#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

static void die(const char *s) {
  perror(s);
  exit(1);
}

static void disable_raw_mode() {
  int result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  if (result == -1) {
    die("tcsetattr");
  }
}

void enable_raw_mode() {
  int result = tcgetattr(STDIN_FILENO, &orig_termios);
  if (result == -1) {
    die("tcgetattr");
  }

  atexit(disable_raw_mode);

  struct termios raw = orig_termios;

  // adding BRKINT, INPCK, ISTRIP for random reasons
  // Disable <C-s> and <C-q>
  // Fix <C-m>
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  // \r and \n handling
  raw.c_oflag &= ~(OPOST);

  // random reason
  raw.c_cflag |= (CS8);

  // ISIG is to turn of <C-c> and <C-z>, do i need this?
  // IEXTEN is for <C-v>
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // timeout for read
  raw.c_cc[VMIN] = 0;  // min bytes before read() can return
 
  raw.c_cc[VTIME] = 1; // wait 1/10 of a second

  result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  if (result == -1) {
    die("tcsetattr");
  }
}
```

term.h
```C
#ifndef TERM_H
#define TERM_H

#include <stdio.h>

// enables raw mode in the terminal
void enable_raw_mode();

// clears the terminal screen
static inline void clear_term() {
    printf("\033[2J\033[H");
}

#endif
```

typing.c
```C
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "input_letter.h"
#include "term.h"
#include "typing.h"

TestInfo start_typing_test(char **str_list, int len) {
  InputLetterList temp = letter_list(str_list, len);
  if (temp.list == NULL) {
    perror("could not malloc");
    exit(1);
  }

  InputLetter *letter_list = temp.list;
  int letter_len = temp.len;

  struct timespec start, end;

  int letter_i = 0;

  while (letter_i < letter_len) {
    clear_term();
    print_input_letter(letter_list, letter_len);

    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      perror("could not read the thing");
      exit(1);
    } else if (c == '\0') {
      continue;
    }

    if (letter_i == 0) {
      clock_gettime(CLOCK_MONOTONIC, &start);
    }

    letter_set_status(letter_list, letter_i, c);
    letter_i++;
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  clear_term();

  double acc = letter_accuracy(letter_list, letter_len);
  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  double cps = letter_len / time_taken;

  free(letter_list);

  return (TestInfo){.accuracy = acc, .time = time_taken, .cps = cps};
}
```

typing.h
```C
#ifndef TYPING_H
#define TYPING_H

#include <stdio.h>

typedef struct {
  double accuracy;
  double time;
  double cps;
} TestInfo;

TestInfo start_typing_test(char **str_list, int len);

static inline void print_test_info(TestInfo test_info) {
  printf("Accuracy: %f\r\n", test_info.accuracy);
  printf("Time: %f seconds\r\n", test_info.time);
  printf("CPS: %f\r\n", test_info.cps);
}

#endif
```

