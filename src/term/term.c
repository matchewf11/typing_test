// <https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

int enable_raw_mode() {

  int result = tcgetattr(STDIN_FILENO, &orig_termios);
  if (result == -1) {
    return -1;
  }

  struct termios raw = orig_termios;

  // adding BRKINT, INPCK, ISTRIP for random reasons
  // Disable <C-s> and <C-q>
  // Fix <C-m>
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST); // \r and \n handling
  raw.c_cflag |= (CS8);    // random reason

  // ISIG is to turn of <C-c> and <C-z>, do i need this?
  // IEXTEN is for <C-v>
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;  // min bytes before read() can return (timeout for read)
  raw.c_cc[VTIME] = 1; // wait 1/10 of a second

  result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  if (result == -1) {
  }

  if (result == -1) {
    return -1;
  }

  return 0;
}

int disable_raw_mode() {
  int result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  if (result == -1) {
    return -1;
  }
  return 0;
}

void clear_term() { printf("\033[2J\033[H"); }
