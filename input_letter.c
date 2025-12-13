#include "input_letter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RIGHT, WRONG, TODO } LetterStatus;

struct input_letter {
  char character;
  LetterStatus status;
};

// @ deprecate this (make this private and static)
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

// make sure cleint checks null
// make sure that client frees each one and the list itself
InputLetter **input_letter_list(char **str, int len) {

  InputLetter **result = malloc(sizeof(char*) * len);
  if (result == NULL) {
    return NULL;
  }

  for (int i = 0; i < len; i++) {
    result[i] = input_letter_from_str(str[i]);
    if (result[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(result[j]);
      }
      free(result);
      return NULL;
    }
  }

  return result;
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
