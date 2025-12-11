#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void die(const char *s) {
    perror(s);
    exit(1);
}

void disable_raw_mode() { 
    int result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); 
    if (result == -1) {
        die("tcsetattr");
    }
}

// <https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html>
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
  raw.c_cc[VMIN] = 0; // min bytes before read() can return
  raw.c_cc[VTIME] = 1; // wait 1/10 of a second

  result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  if (result == -1) {
      die("tcsetattr");
  }
}

int main() {
  enable_raw_mode();

  while (1) {
    char c = '\0';

    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
        die("read");
    }

    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }

    if (c == 'q') break;

  }

  return 0;
}

// for (int i = 0; i < 4; i++) {
//     printf("\033[2J\033[H");
//     printf("%c\n", i + '0');
//     getchar();
// }
// for (int i = 0; i < LINE_LEN; i++) {
//     putchar('a');
// }
// printf("\033[2J\033[H");
// printf("Hello\n");
// sleep(2);
// // printf("\033[1;31mWELCOME TO OPENGENUS\033[0m\n");
// // printf("\033[1;32mWELCOME TO OPENGENUS\033[0m\n");
// return 0;
// #include <stdio.h>
// #include <unistd.h>
// #define LINE_LEN 40
// typedef enum { CORRECT, WRONG, TODO } Status;
//
//
// fix .c settings for vim to match clangd
