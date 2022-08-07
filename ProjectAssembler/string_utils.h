
#ifndef STRING_UTIlS_H_
#define STRING_UTIlS_H_
#include "utils.h"

char* removeLeadingWhiteSpaces(char* line);
char* getEndOfWord(char* line);
char* getFirstWordFromALine(char* line, char* word);
char* removeEndWhiteSpaces(char* line);
int isCommnetLine(char* line);
int isEmetyLine(char* line);

#endif