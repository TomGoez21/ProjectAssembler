#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h> 

#define NUM_DIRECTIVES 5
#define NUM_ORDERS 16
#define NUM_REGERSITERS 8
#define INITIAL_ALLOCATED_SIZE 10

typedef struct line_details {
	int line_number;
	char* file_name;
	char* line;
} line_details;

char* cat_str(char* str0, char* str1);

void* check_malloc(long size);

/*bool is_reserved_word(char* name);*/

bool is_label(char* str);

/*moves the index to the next place where the is no tab of white space*/
int move_white_space(int index, char* string);

int printf_line_error(line_details line, char* error_message, ...);

bool is_legal_num(char* text);

char* get_first_word(char* str);


#endif