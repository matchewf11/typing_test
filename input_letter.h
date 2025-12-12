#ifndef INPUT_LETTER_H
#define INPUT_LETTER_H

typedef struct input_letter InputLetter;

// make sure to check if this is null in caller
InputLetter* input_letter_from_str(const char *const str);

// print a letter list
void print_input_letter(const InputLetter *const list, int len);

// set thing at i to right
void letter_set_status(InputLetter *list, int i, char c);

// get the accuracy
double letter_accuracy(const InputLetter *const list, int len);

#endif
