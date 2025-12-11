```c

/*
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
    if (c == 'q') {
      break;
    }
  }
  return 0;
}
*/

```
```c

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
// fix .c settings for vim to match clangd
// typedef enum { RIGHT, WRONG, TODO } LetterStatus;
//
// typedef struct {
//   char character;
//   LetterStatus status;
// } InputLetter;

//
// void print_letter_list(InputLetter *list, int len) {
//   for (int i = 0; i < len; i++) {
//     InputLetter curr_letter = list[i];
//     char curr_char = curr_letter.character;
//     switch (curr_letter.status) {
//     case RIGHT:
//       printf("\033[1;32m%c\033[0m", curr_char);
//       break;
//     case WRONG:
//       printf("\033[1;31m%c\033[0m", curr_char);
//       break;
//     case TODO:
//       printf("%c", curr_char);
//       break;
//     }
//   }
//   printf("\n");
// }
//
// int main(void) { 
//   InputLetter letters[10];
//   letters[0].character = 'a';
//   letters[1].character = 'a';
//   letters[2].character = 'a';
//   letters[3].character = 'a';
//   letters[0].status = TODO;
//   letters[1].status = RIGHT;
//   letters[2].status = WRONG;
//   letters[3].status = TODO;
//
//   print_letter_list(letters, 4);
//
//   return 0; 
// }
```
