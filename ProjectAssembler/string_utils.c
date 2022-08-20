#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_utils.h"
/*#pragma warning(suppress : 4996)*/


char* removeLeadingWhiteSpaces(char* line) {
	int i;
	int len;
	i = 0;
	len = strlen(line);
	while (isspace(line[i]) && (i < len))
	{
		i++;
	}
	line = line + i;
	return line;
}

char* getEndOfWord(char* line) {
	int i = 0;
	int len = strlen(line);
	while (!isspace(line[i]) && (i < len))
	{
		i++;
	}
	line[i] = '\0';
	return (line);
}

char* getFirstWordFromALine(char* line, char* currWord)
{
	char word[LINE_LENGTH] = { 0 };
	word[0] = '\0';
	strcpy(word, line);
	strcpy(currWord, removeLeadingWhiteSpaces(word));
	strcpy(currWord, getEndOfWord(currWord));
	strcpy(currWord, removeEndWhiteSpaces(currWord));
	return currWord;
}

char* removeEndWhiteSpaces(char* line) {
	int length = strlen(line);
	while (isspace(line[length]) && (length < 0)) {
		--length;
	}
	line[length] = '\0';
	return (line);
}

int isCommnetLine(char* line) {
	char static wordBuffer[LINE_LENGTH] = "";
	char* firstWord = NULL;
	firstWord = getFirstWordFromALine(line, wordBuffer);
	if (firstWord[0] == ';') {
		return 1;
	}
	return 0;
}

int isEmetyLine(char* line) {
	int size = 0;
	size = strlen(removeLeadingWhiteSpaces(line));
	if (size == 0) {
		return 1;
	}
	return 0;
}

char* removeEndOfString(char* string, int charsToRemove) {
	char* newStr = (char*)malloc(strlen(string) + 10);
	string[strlen(string) - charsToRemove] = '\0';
	strcpy(newStr, string);
	return newStr;
}

char* getLeadingWhiteSpace(char* line) {
	int i;
	int len;
	i = 0;
	len = strlen(line);
	while (isspace(line[i]) && (i < len))
	{
		i++;
	}
	line[i] = '\0';
	return line;
}

char* concatWhiteSpacesPerEachLine(char* str, char* whiteSpace)
{
	char static whitSpaceLine[LINE_LENGTH] = "";
	int i, j;
	int lines;
	char* buffer;
	j = 0;
	lines = countLines(str);
	buffer = (char*)malloc(sizeof(char) * lines * LINE_LENGTH);
	strcpy(buffer, "");
	strcpy(whitSpaceLine, whiteSpace);
	lines--;
	j = strlen(whiteSpace);
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '\n') {
			strcat(buffer, whitSpaceLine);
			strcat(buffer, "\n");
			strcpy(whitSpaceLine, whiteSpace);
			j = strlen(whiteSpace);
			i++;
			lines--;
			if (lines == 0) {
				break;
			}
		}
		whitSpaceLine[j] = str[i];
		whitSpaceLine[j + 1] = '\0';
		j++;
	}
	str[i + 1] = '\0';
	return buffer;
}

int countLines(char* str) {
	int i;
	int lines = 1;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			lines++;
		}
	}
	return lines;
}