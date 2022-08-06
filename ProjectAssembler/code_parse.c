#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ctype.h> 
#include "utils.h"
#include "label_check.h"
#include "orders.h"
#include "code_parse.h"
#include "string_utils.h"

#pragma warning(disable : 4996)


char* first_order_group[5] = { "mov","cmp","add","sub","lea" };
char* second_order_group[9] = { "clr","not","inc", "dec", "jmp", "bne", "jsr", "get", "prn" };
char* third_order_group[2] = { "rts","hlt" };


int operands_check(line_details line, long** code_image_ptr, long *IC) {
	printf("Called not-implemented function - operands_check()");
	return 0;
}


addressing_type parse_operand_addressing_type(line_details line, char* operand) {
	int i;
	char* label_name = NULL;
	char* operand_cpy = NULL;
	operand_cpy = (char*)malloc(80);
	label_name = (char*)malloc(30);
	strcpy(operand_cpy, operand);
	/*get label name, it exists*/
	for (i = 0; operand[i] != '.' && operand[i]; i++) {
		label_name[i] = operand[i];
	}
	label_name[i] = '\0';
	printf("label name is: %s", label_name);
	/*advance the pointer to point after the label*/
	operand_cpy += strlen(label_name);

	/*check if it is immediate addressing*/
	if (operand[0] == '#' && is_legal_num(operand + 1)) {
		printf("true\n");
		return IMMEDIATE_ADD;
	}

	/*check if it is struct addressing*/
	if (is_label_valid_in_struct(line, operand) && (*(operand_cpy) == '.') && ((*(operand_cpy + 1) == '1') || (*(operand_cpy + 1) == '2'))) {
		printf("true\n");
		return STRUCT_ADD;
	}

	/*check if it is direct addressing*/
	if (is_label_valid_in_text(line, operand)) {
		printf("true\n");
		return DIRECT_ADD;
	}

	/*check if it is register addressing*/
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' && operand[2] == '\0') {
		printf("true\n");
		return REGISTER_ADD;
	}
	else
		return NONE;
}



void validate_operand_addressing(line_details line, addressing_type src_address, addressing_type dst_address, char* src_oper, char* dst_oper) {
	int i = 0;
	int cmp;
	char* oper = { 0 };

	/*get the operator name*/
	oper = get_first_word(line.line);
	printf("the first: %s\n", oper);
	printf("the first: %d\n", strlen(oper));
	line.line += strlen(oper);
	while (isspace(*(line.line))) { ((line.line))++; }

	/*get the src_address name*/
	src_oper = get_first_word(line.line);
	printf("the src: %s\n", src_oper);
	line.line += strlen(src_oper);
	while (isspace(*(line.line))) { ((line.line))++; }

	if (*line.line == ',') {
		line.line++;
	}
	printf("line points to: %s\n", line.line);
	while (isspace(*(line.line))) { ((line.line))++; }

	/*get the dst_address name*/

	dst_oper = get_first_word(line.line);
	printf("the dst: %s\n", dst_oper);

	/*check if opcode in the first group of opcodes, which means it gets 2 operands./
	there are 5 opcode in this group.*/
	for (i = 0; i < 5; i++) {
		cmp = strcmp(oper, first_order_group[i]);
		if (cmp == 0) {
			printf("first group\n");
			src_address = parse_operand_addressing_type(line, src_oper);
			printf("src_add type:%d\n", src_address);
			dst_address = parse_operand_addressing_type(line, dst_oper);
			printf("dst_add type:%d\n", dst_address);

			return;
		}
	}
	/*check if opcode in the second group of opcodes, which means it gets 1 operand./
	there are 9 opcode in this group.*/
	for (i = 0; i < 9; i++) {
		cmp = strcmp(oper, second_order_group[i]);
		if (cmp == 0) {
			printf("second");
			src_address = parse_operand_addressing_type(line, src_oper);
			/*no dst address*/
			dst_address = 4;
			return;
		}
	}
	/*check if opcode in the second group of opcodes, which means it gets 1 operand./
	there are 9 opcode in this group.*/
	for (i = 0; i < 9; i++) {
		cmp = strcmp(oper, second_order_group[i]);
		if (cmp == 0) {
			printf("third");
			/*no dst or src address*/
			src_address = 4;
			dst_address = 4;
			return;
		}
	}
}