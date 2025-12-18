#include <stdlib.h>

#include "typing_token/typing_token.h"

typedef struct {
  float accuracy;
  float time;
  float cps;
} TestInfo;

TestInfo typing_test(const char *const *str_list, int len) {

  int token_len;
  TypingToken *token_list = typing_token_list(str_list, len, &token_len);
  if (token_list == NULL) {
    // handle this case
  }

  // set start and end

  struct timespec start, end;

  //   int letter_i = 0;
  //   while (letter_i < letter_len) {
  //     clear_term();
  //     print_input_letter(letter_list, letter_len);
  //     char c = '\0';
  //     if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
  //       perror("could not read the thing");
  //       exit(1);
  //     } else if (c == '\0') {
  //       continue;
  //     }
  //     if (letter_i == 0) {
  //       clock_gettime(CLOCK_MONOTONIC, &start);
  //     }
  //     letter_set_status(letter_list, letter_i, c);
  //     letter_i++;
  //   }
  //
  //   clock_gettime(CLOCK_MONOTONIC, &end);
  //   clear_term();
  //
  //   double acc = letter_accuracy(letter_list, letter_len);
  //   double time_taken =
  //       (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  //   double cps = letter_len / time_taken;

  free(token_list);

  // return (TestInfo){.accuracy = acc, .time = time_taken, .cps = cps};
  return (TestInfo){};
}

// typing.c
// ```C
// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <unistd.h>
// ```
//
// typing.h
// ```C
// #ifndef TYPING_H
// #define TYPING_H
//
// #include <stdio.h>
//
// TestInfo start_typing_test(char **str_list, int len);
//
// static inline void print_test_info(TestInfo test_info) {
//   printf("Accuracy: %f\r\n", test_info.accuracy);
//   printf("Time: %f seconds\r\n", test_info.time);
//   printf("CPS: %f\r\n", test_info.cps);
// }
//
// #endif
// ```
