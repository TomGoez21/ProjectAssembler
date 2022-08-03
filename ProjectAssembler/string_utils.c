#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* removeLeadingWhiteSpaces(char* line) {
    int i;
    i = 0;
    int len = strlen(line);
    while(isspace(line[i]) && (i < len))
    {
        i++;
    }
    line = line + i;
    printf("---->%s , len->%ld\n",(line),strlen(line));
    return line;
}

char* getEndOfWord(char* line) { 
    int i  = 0;
    int len = strlen(line);
    while(!isspace(line[i]) && (i < len))
    {
        i++;
    }
    line[i] = '\0';
    return (line);
}

int move_white_space(int index, char* string) {
	for (; (string[(index)] == '\t' || string[(index)] == ' ') && (string[(index)]); (++(index)));
	return index;
}

char* getFirstWordFromALine(char* line, char* word)
{
    int i;
    int endOfWordIndex;
    int length;
    i = 0;
    length = strlen(line);
    endOfWordIndex = 0;
    strcpy(word , line);
    word = removeLeadingWhiteSpaces(word);
    word  = getEndOfWord(word);
    return word;
}
char* removeEndWhiteSpaces(char* line){
    int length = strlen(line);
     while(isspace(line[length]) && (length < 0)){
        --length;
     }
    line[length] = '\0';
    return (line);
}
