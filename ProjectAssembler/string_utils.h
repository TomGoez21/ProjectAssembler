
#ifndef STRING_UTIlS_H_
#define STRING_UTIlS_H_
#include "utils.h"

char* removeLeadingWhiteSpaces(char* line);
char* getEndOfWord(char* line);
int move_white_space(int index, char* string);
char* getFirstWordFromALine(char* line, char* word);
char* removeEndWhiteSpaces(char* line);

#endif