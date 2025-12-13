#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "input_letter.h"
#include "term.h"

void start_typing_test(char **str_list, int len) {

  InputLetter **letter_lists = input_letter_list(str_list, len);
  if (letter_lists == NULL) {
    // handle this better?
    perror("could not malloc");
    exit(1);
  }

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // start timeing when first char is clicked

  int phrase_i = 0;
  int letter_i = 0;

  while (phrase_i < len) {
    clear_term();

    // print every list
    for (int i = 0; i < len; i++) {
      print_input_letter(letter_lists[i], strlen(str_list[i]));
    }

    char c = '\0';

    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      perror("could not read the thing");
      exit(1);
    } else if (c == '\0') {
      continue;
    }

    letter_set_status(letter_lists[phrase_i], letter_i, c);
    letter_i++;

    if (letter_i == strlen(str_list[phrase_i])) {
      phrase_i++;
      letter_i = 0;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  clear_term();

  // double acc = letter_accuracy(letter_list, strlen(str));
  // printf("Accuracy: %f\r\n", acc);
  //
  // double time_taken =
  //     (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  //
  // printf("Time: %f seconds\r\n", time_taken);
  // printf("CPS: %f\r\n", strlen(str) / time_taken);

  for (int i = 0; i < len; i++) {
    free(letter_lists[i]);
  }
  free(letter_lists);
}
