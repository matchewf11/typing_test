#include "term.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  enable_raw_mode();

  char c = '\0';

  for (int i = 0; i < 10; i++) {
    clear_term();
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      perror("could not read the thing");
      exit(1);
    }
    printf("%d\r\n", i);
  }

  return 0;
}
