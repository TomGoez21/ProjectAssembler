#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h> 

char* directive_list[5] = { "data","string","struct","entry","extern" };
char* order_list[16] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt" };
char* reg_list[8] = { "r0","r1","r2","r3","r4","r5","r6","r7" };

typedef struct line_details {
	int line_number;
	char* file_name;
	char* line;
} line_details;

char* cat_str(char* str0, char* str1);

void* check_malloc(long size);

bool is_reserved_word(char* name);

/*moves the index to the next place where the is no tab of white space*/
int move_white_space(int index, char* string);

int printf_line_error(line_details line, char* error_message, ...);

#endif