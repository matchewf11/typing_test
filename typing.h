#ifndef TYPING_H
#define TYPING_H

#include <stdio.h>

typedef struct {
  double accuracy;
  double time;
  double cps;
} TestInfo;

TestInfo start_typing_test(char **str_list, int len);

static inline void print_test_info(TestInfo test_info) {
  printf("Accuracy: %f\r\n", test_info.accuracy);
  printf("Time: %f seconds\r\n", test_info.time);
  printf("CPS: %f\r\n", test_info.cps);
}

#endif
