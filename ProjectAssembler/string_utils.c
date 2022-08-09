#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_utils.h"

#pragma warning(disable : 4996)

char* removeLeadingWhiteSpaces(char* line) {
    int i;
    int len;
    i = 0;
    len = strlen(line);
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

char* getFirstWordFromALine(char* line, char* word)
{
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

int isCommnetLine(char* line){
    char* firstWord = NULL;
    firstWord = getFirstWordFromALine(line, firstWord);
    if(firstWord[0] == ';'){
        return 1;
    }
    return 0;
}

int isEmetyLine(char* line){
    int size = 0;
    size = strlen(removeLeadingWhiteSpaces(line));
    if(size == 0){
        return 1;
    }
    return 0;
}