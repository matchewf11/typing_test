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
