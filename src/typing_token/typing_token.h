#ifndef TYPING_TOKEN_H
#define TYPING_TOKEN_H

#endif

// ```
//
// input_letter.h
// ```C
// #ifndef INPUT_LETTER_H
// #define INPUT_LETTER_H
//
// typedef struct input_letter InputLetter;
//
// typedef struct {
//   InputLetter* list;
//   int len;
// } InputLetterList;
//
// // print a letter list
// void print_input_letter(const InputLetter *const list, int len);
//
// // set thing at i to right
// void letter_set_status(InputLetter *list, int i, char c);
//
// // get the accuracy
// double letter_accuracy(const InputLetter *const list, int len);
//
// // make a function that takes in **char and return InputLetter*
// // client can use one free on the pointer they get
// // make sure they check for null
// // add spaces in between '. F'
// InputLetterList letter_list(char **str_list, int len);
//
// #endif
// ```
//
// do i make an opaque pointer
