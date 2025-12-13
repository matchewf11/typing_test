#ifndef TYPING_H
#define TYPING_H

typedef struct {
  double accuracy;
  double time;
  double cps;
} TestInfo;

TestInfo start_typing_test(char **str_list, int len);

#endif
