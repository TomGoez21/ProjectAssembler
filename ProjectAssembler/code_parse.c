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


int operands_check(line_details line, long* code_image_ptr, long* IC) {
	printf("Called not-implemented function - operands_check()");
	return 0;
}

/*determine the type of addressing. also add to the word counter L*/
addressing_type parse_operand_addressing_type(long* L, line_details line, char* operand, long code_image_ptr[][80], long* IC) {
	int i;
	char label_name[30] = { 0 };
	char* operand_after_label = 0;

	/*get label name, if exists*/
	for (i = 0; operand[i] != '.' && operand[i]; i++) {
		label_name[i] = operand[i];
	}

	label_name[i] = '\0';
	printf("label name is: %s\n", label_name);

	/*advance the pointer to point after the label*/
	operand_after_label = operand + strlen(label_name);

	/*check if it is immediate addressing*/
	if (operand[0] == '#' && is_legal_num(operand + 1)) {
		printf("true\n");
		*L += 1;
		/*adding the immediate operand to code_image*/
		strcpy(code_image_ptr[*IC], operand + 1);
		printf(" code in IC place: %s\n", code_image_ptr[*IC]);
		printf(" IC: %d\n", *IC);
		(*IC)++;
		return IMMEDIATE_ADD;
	}

	/*check if it is register addressing*/
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' && operand[2] == '\0') {
		printf("true\n");
		*L += 1;
		/*adding the register operand to code_image*/
		strcpy(code_image_ptr[*IC], operand);
		printf(" code in IC place: %s\n", code_image_ptr[*IC]);
		printf(" IC: %d\n", *IC);
		(*IC)++;
		return REGISTER_ADD;
	}

	/*check if it is struct addressing*/
	if (is_label_valid_in_struct(line, operand) &&
		(*(operand_after_label) == '.') && ((*(operand_after_label + 1) == '1') || (*(operand_after_label + 1) == '2'))) {
		printf("true\n");
		*L += 2;

		/*adding the label of the struct operand to code_image*/
		if (*label_name) {
			//*code_image_ptr = (long*)realloc(*code_image_ptr, (*IC + 1) * sizeof(long));
			printf(" BEFORE: code in IC place: %s\n", code_image_ptr[*IC]);
			printf(" BEFORE IC: %d\n", *IC);
			//memcpy(code_image_ptr[*IC], label_name, sizeof(label_name));
			strncpy(code_image_ptr[*IC], label_name, sizeof(label_name));
			//code_image_ptr[*IC] = *label_name;
			printf(" code in IC place: %s\n", code_image_ptr[*IC]);
			printf(" IC: %d\n", *IC);
			(*IC)++;
			/*adding the strcut index to code_image*/
			strcpy(code_image_ptr[*IC], operand_after_label + 1);
			//code_image_ptr[*IC] = *(operand_after_label + 1);
			printf(" code in IC place: %s\n", code_image_ptr[*IC]);
			printf(" IC: %d\n", *IC);
			(*IC)++;

		}

		return STRUCT_ADD;
	}

	/*check if it is direct addressing*/
	if (is_label_valid_in_text(line, operand)) {
		printf("true\n");
		*L += 1;
		/*adding the label operand to code_image*/
		strcpy(code_image_ptr[*IC], operand);
		printf(" code in IC place: %s\n", code_image_ptr[*IC]);
		printf(" IC: %d\n", *IC);
		(*IC)++;
		return DIRECT_ADD;
	}

	else
		return NONE;
}



void validate_operand_addressing(long* L_ptr, line_details line, addressing_type src_address, addressing_type dst_address, char* src_oper, char* dst_oper, long code_image_ptr[][80], long* IC) {
	int i = 0;
	int cmp;
	char* oper = 0;

	/*get the operator name*/
	oper = get_first_word(line.line);
	printf("the first: %s\n", oper);
	/*add +1 to L because of opcode*/
	*L_ptr += 1;

	/*add operand to code_image*/
	//*code_image_ptr = (long*)realloc(*code_image_ptr, (*IC + 1) * sizeof(long));
	strcpy(code_image_ptr[*IC], oper);
	printf(" code in IC place: %s\n", code_image_ptr[*IC]);
	printf(" IC: %d\n", *IC);
	(*IC)++;

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
			src_address = parse_operand_addressing_type(L_ptr, line, src_oper, code_image_ptr, IC);
			printf("src_add type:%d\n", src_address);
			dst_address = parse_operand_addressing_type(L_ptr, line, dst_oper, code_image_ptr, IC);
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
			src_address = parse_operand_addressing_type(L_ptr, line, src_oper, code_image_ptr, IC);
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
/*
void first_word_code(line_details line, char *operand, addressing_type src_address, addressing_type dst_address) {

}*/