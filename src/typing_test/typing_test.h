#ifndef TYPING_TEST_H
#define TYPING_TEST_H

typedef struct test_info TestInfo;

// return -1 on error, 0 on success
int typing_test(const char *const *str_list, int len, TestInfo *out);

inline void print_test_info(const TestInfo test_info);

#endif
