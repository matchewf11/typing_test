#ifndef TYPING_TOKEN_H
#define TYPING_TOKEN_H

typedef struct typing_token TypingToken;

// out is the len of the output list
// make sure that return is not null
TypingToken *typing_token_list(const char *const *str_list, int len, int *out);

// prints out the typing token list
// it gives them colors :)
void print_typing_token_list(const TypingToken *list, int len);

void update_typing_token_status(TypingToken *list, int i, char c);

float letter_accuracy(const TypingToken *list, int len);

#endif
