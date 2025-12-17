#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typing_token.h"

typedef enum { RIGHT, WRONG, TODO } TokenStatus;

struct typing_token {
  char ch;
  TokenStatus status;
};

TypingToken *typing_token_list(const char *const *str_list, int len,
                                      int *out) {
  int total_len = len - 1;
  for (int i = 0; i < len; i++) {
    total_len += strlen(str_list[i]);
  }

  TypingToken *list = malloc(sizeof(TypingToken) * total_len);
  if (list == NULL) {
    return NULL;
  }

  int list_i = 0;

  // iterates over phrases
  for (int i = 0; i < len; i++) {

    // iterate over phrase chars
    for (int j = 0; j < strlen(str_list[i]); j++) {
      list[list_i].status = TODO;
      list[list_i].ch = str_list[i][j];
      list_i++;
    }

    // execpt for last iteration
    if (i != len - 1) {
      list[list_i].status = TODO;
      list[list_i].ch = ' ';
      list_i++;
    }
  }

  return list;
}

void print_typing_token_list(const TypingToken *list, int len) {
  for (int i = 0; i < len; i++) {
    TypingToken curr_token = list[i];
    char curr_ch = curr_token.ch;
    switch (curr_token.status) {
    case RIGHT:
      printf("\033[1;32m%c\033[0m", curr_ch);
      break;
    case WRONG:
      printf("\033[1;31m%c\033[0m", curr_ch);
      break;
    case TODO:
      printf("%c", curr_ch);
      break;
    }

    if (curr_ch == ' ' && i != 0 && list[i - 1].ch == '.' && i != len - 1) {
      printf("\r\n");
    }
  }

  printf("\r\n");
}

void update_typing_token_status(TypingToken *list, int i, char c) {
  if (list[i].ch == c) {
    list[i].status = RIGHT;
  } else {
    list[i].status = WRONG;
  }
}

float letter_accuracy(const TypingToken *list, int len) {
  int right = 0;
  int wrong = 0;

  for (int i = 0; i < len; i++) {
    TokenStatus ls = list[i].status;
    if (ls == RIGHT) {
      right++;
    } else if (ls == WRONG) {
      wrong++;
    }
  }
  return right / (float)right + wrong;
}
