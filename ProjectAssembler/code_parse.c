#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ctype.h> 
#include "utils.h"
#include "label_check.h"
#include "code_parse.h"
#include "orders.h"

char* first_order_group[5] = { "mov","cmp","add","sub","lea" };
char* second_order_group[9] = { "clr","not","inc", "dec", "jmp", "bne", "jsr", "get", "prn" };
char* third_order_group[2] = { "rts","hlt" };


int operands_check(line_details line, long** code_image_ptr, int* IC) {
	printf("Called not-implemented function - operands_check()");
	return 1;
}


addressing_type parse_operand_addressing_type(char *operand) {
	if (operand[0] == '#' && is_legal_num(operand + 1)) {
		return IMMEDIATE_ADD;
	}
	if (is_label_valid(operand) && (*(operand + 1) == '.') && ((*(operand + 2) == '1') || (*(operand + 2) == '2'))) {
		return STRUCT_ADD;
	}
	if (is_label_valid(operand)) {
		return DIRECT_ADD;
	}
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' && operand[2] == '\0') {
		return REGISTER_ADD;
	}
	else
		return NONE;
}


void validate_operand_addressing(char* operand, line_details line, addressing_type src_address, addressing_type dst_address) {
	int i = 0;
	int cmp;
	/*
	char op_to_enum[3] = { 0 };
	while (operand[i]) {
		op_to_enum[i] = toupper(operand[i]);
		i++;
	}
	op_to_enum[i] = '\0';
	*/
	/*check if opcode in the first group of opcodes, which means it gets 2 operands./
	there are 5 opcode in this group.*/
	for (i = 0; i < 5; i++) {
		cmp = strcmp(operand, first_order_group[i]);
		if (cmp == 0) {
			/*TODO*/
			src_address = 0; //extract_src_add();
			dst_address = 0; // extract_dst_add();
			return;
		}
	}
	/*check if opcode in the second group of opcodes, which means it gets 1 operand./
	there are 9 opcode in this group.*/
	for (i = 0; i < 9; i++) {
		cmp = strcmp(operand, second_order_group[i]);
		if (cmp == 0) {
			/*TODO*/
			//src_address = extract_src_add();
			src_address = 0;
			dst_address = 0;
			return;
		}
	}
	/*check if opcode in the second group of opcodes, which means it gets 1 operand./
	there are 9 opcode in this group.*/
	for (i = 0; i < 9; i++) {
		cmp = strcmp(operand, second_order_group[i]);
		if (cmp == 0) {
			/*TODO*/
			//src_address = extract_src_add();
			src_address = 0;
			dst_address = 0;
			return;
		}
	}
}