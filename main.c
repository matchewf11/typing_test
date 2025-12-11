#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "input_letter.h"
#include "term.h"

int main(void) {
  enable_raw_mode();

  term_countdown();

  char *str = "hello what is going on?";

  InputLetter *letter_list = input_letter_from_str(str);
  if (!letter_list) {
    perror("could not malloc");
    exit(1);
  }

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

  clear_term();

  return 0;
}
