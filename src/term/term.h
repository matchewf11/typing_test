#ifndef TERM_H
#define TERM_H

// enables raw mode in terminal
// call disable_raw_mode when done
// return 0 if OK
// return -1 if failed (do not need to reset)
int enable_raw_mode();

// reset after you call enable_raw_mode
// return 0 if OK
// return -1 if Error
int disable_raw_mode();

void clear_term();

#endif
