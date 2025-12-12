#include "input_letter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RIGHT, WRONG, TODO } LetterStatus;

struct input_letter {
  char character;
  LetterStatus status;
};

InputLetter *input_letter_from_str(const char *const str) {
  int str_len = strlen(str);

  InputLetter *list = malloc(sizeof(InputLetter) * str_len);
  if (list == NULL) {
    return NULL;
  }

  for (int i = 0; i < str_len; i++) {
    list[i].character = str[i];
    list[i].status = TODO;
  }

  return list;
}

void print_input_letter(const InputLetter *const list, int len) {
  for (int i = 0; i < len; i++) {
    InputLetter curr_letter = list[i];
    char curr_char = curr_letter.character;
    switch (curr_letter.status) {
    case RIGHT:
      printf("\033[1;32m%c\033[0m", curr_char);
      break;
    case WRONG:
      printf("\033[1;31m%c\033[0m", curr_char);
      break;
    case TODO:
      printf("%c", curr_char);
      break;
    }
  }
  printf("\r\n");
}

void letter_set_status(InputLetter *list, int i, char c) {
  if (list[i].character == c) {
    list[i].status = RIGHT;
  } else {
    list[i].status = WRONG;
  }
}

int letter_accuracy(const InputLetter *const list, int len) {
  int right = 0;
  int wrong = 0;

  for (int i = 0; i < len; i++) {
    LetterStatus ls = list[i].status;
    if (ls == RIGHT) {
      right++;
    } else if (ls == WRONG) {
      wrong++;
    }
  }

  return (100 * right) / (right + wrong);
}
