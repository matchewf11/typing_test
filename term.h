#ifndef TERM_H
#define TERM_H

#include <stdio.h>

// enables raw mode in the terminal
void enable_raw_mode();

// clears the terminal screen
static inline void clear_term() {
    printf("\033[2J\033[H");
}

// counts down from 3 to then returns
// clears term before and after
void term_countdown();

#endif
