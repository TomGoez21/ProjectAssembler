#ifndef CODE_PARSE_H_
#define CODE_PARSE_H_

#include "utils.h"

typedef enum adderssing_type {
	IMMEDIATE_ADD, DIRECT_ADD, STRUCT_ADD, REGISTER_ADD, NONE
} addressing_type;

int operands_check(line_details line, long** code_image_ptr, int* IC);

addressing_type parse_operand_addressing_type(char* operand);

void validate_operand_addressing(char* operand, line_details line, addressing_type src_address, addressing_type dst_address);

#endif