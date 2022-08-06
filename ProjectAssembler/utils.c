#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define ERROR_FILE stderr

/*Writes an error to the stderr*/
int printf_line_error(line_details line, char* error_message, ...) {
	int result;
	va_list ap;
	/*prints to ERROR_FILE the relevant line number, file name.*/
	fprintf(ERROR_FILE, "\nError in %s line %ld: \n", line.file_name, line.line_number);
	/*prints the wanted error*/
	va_start(ap, error_message);
	result = vfprintf(ERROR_FILE, error_message, ap);
	va_end(ap);
	fprintf(ERROR_FILE, "\n");
	return result;
}


char* cat_str(char* str0, char* str1) {
	char* str = (char*)check_malloc(strlen(str0) + strlen(str1) + 1);
	strcpy(str, str0);
	strcat(str, str1);
	return str;
}


void* check_malloc(long size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		printf("Memory allocation failed.");
		exit(1);
	}
	return ptr;
}

bool is_label(char* str) {
	
}

int move_white_space(int index, char* string) {
	for (; (string[(index)] == '\t' || string[(index)] == ' ') && (string[(index)]); (++(index)));
	return index;
}


bool is_legal_num(char* text) {
	int i = 0;
	/* checks if text starts with + or -*/
	if (text[0] == '-' || text[0] == '+') {
		text++;
	}
	for (i; i <= (int)(strlen(text)); i++) {
		if (isdigit(text[i])) {
			return true;
		}
	}
	return false;
}


/* Reads the first word of str, allocates memory for it and returns a copy of it */
char* get_first_word(char* str) {
	char* word; /* Points to the beginning of the copy of the word */
	int w_len = 0; /* The word's length */
	int allocated = INITIAL_ALLOCATED_SIZE; /* Number of allocated bytes */
	word = (char*)malloc(allocated); /* Initial allocation */
	if (!word) { /* Checks for successful allocation */
		/*TODO: error*/
	}
	/* Read until whitespace, comma or EOF, copy contents into `word` */
	while (*str && !isspace(*str) && *str != ',') {
		/* If we ran out of space in `word`, make a reallocation with twice the size */
		if (w_len >= allocated - 1) {
			word = realloc(word, allocated *= 2);
			if (!word) { /* Checks for successful allocation */
				/*TODO: error*/
			}
		}
		word[w_len++] = *str++;
	}
	word[w_len] = '\0';
	return word;
}
