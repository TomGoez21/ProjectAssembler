
#ifndef STRING_UTIlS_H_
#define STRING_UTIlS_H_
#include "utils.h"

char* removeLeadingWhiteSpaces(char* line);
char* getEndOfWord(char* line);
char* getFirstWordFromALine(char* line , char* currWord);
char* removeEndWhiteSpaces(char* line);
int isCommnetLine(char* line);
int isEmetyLine(char* line);
char* removeEndOfString(char* string, int charsToRemove);
char* getLeadingWhiteSpace(char* line);
char* concatWhiteSpacesPerEachLine(char* str, char* whiteSpace);
int countLines(char* str);

#endif