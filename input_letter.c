#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_letter.h"

typedef enum { RIGHT, WRONG, TODO } LetterStatus;

struct input_letter {
  char character;
  LetterStatus status;
};

// make a function that takes in **char and return InputLetter*
// client can use one free on the pointer they get
// make sure they check for null
// add spaces in between '. F'
InputLetterList letter_list(char **str_list, int len) {
  int total_len = len - 1;
  for (int i = 0; i < len; i++) {
    total_len += strlen(str_list[i]);
  }

  InputLetter *list = malloc(sizeof(InputLetter) * total_len);
  if (list == NULL) {
    return (InputLetterList){.list = NULL, .len = 0};
  }

  int list_i = 0;

  // iterates over phrases
  for (int i = 0; i < len; i++) {

    // iterate over phrase chars
    for (int j = 0; j < strlen(str_list[i]); j++) {
      list[list_i].status = TODO;
      list[list_i].character = str_list[i][j];
      list_i++;
    }

    // execpt for last iteration
    if (i != len - 1) {
      list[list_i].status = TODO;
      list[list_i].character = ' ';
      list_i++;
    }
  }

  return (InputLetterList){.list = list, .len = total_len};
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

double letter_accuracy(const InputLetter *const list, int len) {
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

  return (100 * right) / ((float)(right + wrong));
}
