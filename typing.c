#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "input_letter.h"
#include "term.h"

void start_typing_test(char **str_list, int len) {

  InputLetterList temp = letter_list(str_list, len);
  if (temp.list == NULL) {
    perror("could not malloc");
    exit(1);
  }

  InputLetter *letter_list = temp.list;
  int letter_len = temp.len;

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // start timeing when first char is clicked

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

    letter_set_status(letter_list, letter_i, c);
    letter_i++;
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  clear_term();

  double acc = letter_accuracy(letter_list, letter_len);
  printf("Accuracy: %f\r\n", acc);

  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("Time: %f seconds\r\n", time_taken);
  printf("CPS: %f\r\n", letter_len / time_taken);

  free(letter_list);
}
