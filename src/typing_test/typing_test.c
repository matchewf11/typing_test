#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "typing_test.h"
#include "typing_token/typing_token.h"

struct test_info {
  float accuracy;
  float time;
  float cps;
};

int typing_test(const char *const *str_list, int len, TestInfo *out) {

  int token_len;
  TypingToken *token_list = typing_token_list(str_list, len, &token_len);
  if (token_list == NULL) {
    return -1; // error
  }

  struct timespec start, end;

  int letter_i = 0;
  while (letter_i < token_len) {

    // clear_term();

    print_typing_token_list(token_list, token_len);

    char c = '\0';

    int err_code = read(STDIN_FILENO, &c, 1);
    if (err_code == -1 && errno != EAGAIN) {
      return -1; // error
    } else if (c == '\0') {
      continue;
    }

    if (letter_i == 0) {
      clock_gettime(CLOCK_MONOTONIC, &start);
    }

    update_typing_token_status(token_list, letter_i, c);
    letter_i++;
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  // clear_term(); // do i need this?

  float acc = letter_accuracy(token_list, token_len);
  float time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  float cps = token_len / time_taken;

  free(token_list);

  out->accuracy = acc;
  out->time = time_taken;
  out->cps = cps;

  return 0;
}

inline void print_test_info(const TestInfo test_info) {
  printf("Accuracy: %f\r\n", test_info.accuracy);
  printf("Time: %f seconds\r\n", test_info.time);
  printf("CPS: %f\r\n", test_info.cps);
}
