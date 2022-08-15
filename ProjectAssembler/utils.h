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

/*if an errror was raised, change error to true, in order not to create files*/
bool set_error(bool current);

/*
 * Function:  cat_str
 * --------------------
 * concatinating 2 string with memory allocation
 *
 * input: str0- the target string
 *		  str1 - cahrs to concat
 *
 */
char* cat_str(char* str0, char* str1);

/*
 * Function:  cat_str
 * --------------------
 * check if malloc succeded
 *
 * input: long size - the maclloc response
 *
 */
void* check_malloc(long size);


/*
 * Function:  cat_str
 * --------------------
 * moves the index to the next place where the is no tab of white space
 *
 * input: int index- index for first char
 *		  char* string - string to check
 *			
 * return: index after moving the whtiespaces
 */
int move_white_space(int index, char* string);

/*
 * Function:  printf_line_error
 * --------------------
 * Writes an error to the stderr
 *
 * input:	
 *			line_details line
 *			char* error_message
 *
 * return: total number of chars on success
 *			if fails a negative number is returnd
 */
int printf_line_error(line_details line, char* error_message, ...);

/*
 * Function:  is_legal_num
 * --------------------
 * checks if text starts with + or -
 *
 * input:
 *			char text - a number value
 *
 * return: 1 if starts with +/-;
 */
bool is_legal_num(char* text);



/*
 * Function:  get_first_word
 * --------------------
 * Reads the first word of str, allocates memory for it and returns a copy of it 
 *
 * input:
 *			char str - a string to cut the word from
 *
 * return: the first word
 */
char* get_first_word(char* str);


#endif