#ifndef CODE_PARSE_H_
#define CODE_PARSE_H_

#include "utils.h"

typedef enum adderssing_type {
	IMMEDIATE_ADD, DIRECT_ADD, STRUCT_ADD, REGISTER_ADD, NONE
} addressing_type;

int operands_check(line_details line, long** code_image_ptr, long *IC);

addressing_type parse_operand_addressing_type(line_details line, char* operand);

void validate_operand_addressing(line_details line, addressing_type src_address, addressing_type dst_address, char* src_oper, char* dst_oper);

#endif