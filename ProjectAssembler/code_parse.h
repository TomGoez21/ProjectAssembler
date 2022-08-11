#ifndef CODE_PARSE_H_
#define CODE_PARSE_H_

#include "utils.h"

typedef enum adderssing_type {
	IMMEDIATE_ADD, DIRECT_ADD, STRUCT_ADD, REGISTER_ADD, NONE
} addressing_type;


/* code word structure */
typedef struct code_structure {
	unsigned int opcode : 4;
	unsigned int src_addressing : 2;
	unsigned int dest_addressing : 2;
	unsigned int ARE : 2;
} code_structure;

/*TODO: do we want to use it?*/
typedef struct binary_line {
	line_details line;
	int first_word_address;
	int num_of_words;
	char* first_operand_name;
	char* second_operand_name;
	code_structure word[5];
} binary_line;


int operands_check(line_details line, long* code_image_ptr, long *IC);

addressing_type parse_operand_addressing_type(long* L, line_details line, char* operand, long code_image_ptr[][80], long* IC);

void validate_operand_addressing(char** oper, long* L_ptr, line_details line, addressing_type* src_address, addressing_type* dst_address, char* src_oper, char* dst_oper, long code_image_ptr[][80], long* IC);

void opcode_to_bin(char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper);

#endif