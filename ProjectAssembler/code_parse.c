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
#include "symbol_conversion.h"
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



void validate_operand_addressing(char** oper, long* L_ptr, line_details line, addressing_type* src_address, addressing_type* dst_address, char** src_oper, char** dst_oper, long code_image_ptr[][80], long* IC) {
	int i = 0;
	int cmp;

	/*get the operator name*/
	*oper = get_first_word(line.line);
	printf("the first: %s\n", *oper);
	/*add +1 to L because of opcode*/
	*L_ptr += 1;

	/*add operand to code_image*/
	//*code_image_ptr = (long*)realloc(*code_image_ptr, (*IC + 1) * sizeof(long));
	strcpy(code_image_ptr[*IC], *oper);
	printf(" code in IC place: %s\n", code_image_ptr[*IC]);
	printf(" IC: %d\n", *IC);
	(*IC)++;

	line.line += strlen(*oper);
	while (isspace(*(line.line))) { ((line.line))++; }

	/*get the src_address name*/
	*src_oper = get_first_word(line.line);
	printf("the src: %s\n", *src_oper);
	line.line += strlen(*src_oper);
	while (isspace(*(line.line))) { ((line.line))++; }

	if (*line.line == ',') {
		line.line++;
	}
	printf("line points to: %s\n", line.line);
	while (isspace(*(line.line))) { ((line.line))++; }

	/*get the dst_address name*/

	*dst_oper = get_first_word(line.line);
	printf("the dst: %s\n", *dst_oper);

	/*check if opcode in the first group of opcodes, which means it gets 2 operands./
	there are 5 opcode in this group.*/
	for (i = 0; i < 5; i++) {
		cmp = strcmp(*oper, first_order_group[i]);
		if (cmp == 0) {
			printf("first group\n");
			*src_address = parse_operand_addressing_type(L_ptr, line, *src_oper, code_image_ptr, IC);
			printf("src_add type:%d\n", *src_address);
			*dst_address = parse_operand_addressing_type(L_ptr, line, *dst_oper, code_image_ptr, IC);
			printf("dst_add type:%d\n", *dst_address);

			return;
		}
	}
	/*check if opcode in the second group of opcodes, which means it gets 1 operand./
	there are 9 opcode in this group.*/
	for (i = 0; i < 9; i++) {
		cmp = strcmp(oper, second_order_group[i]);
		if (cmp == 0) {
			printf("second");
			*src_address = parse_operand_addressing_type(L_ptr, line, *src_oper, code_image_ptr, IC);
			/*no dst address*/
			*dst_address = NONE;
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
			*src_address = NONE;
			*dst_address = NONE;
			return;


			/*TODO: check the type of opcode and type of src and dst operand*/
		}
	}
}


void add_to_code_table(CodeTable* table, CodeTableEntry to_add) {
	if (label_exists(table, &to_add)) {
		printf("Exists"); /*TODO: print row numberand indicitive error*/
	}
	table->size++;
	if (!table->entries) {
		/*First add :*/
		table->entries = (CodeTableEntry*)malloc(table->size * sizeof(CodeTableEntry)); /* Initial allocation */
	}
	else {
		/* Allocate more memory for the new entry */
		table->entries = (CodeTableEntry*)realloc(table->entries, table->size * sizeof(CodeTableEntry));
	}
	if (!table->entries) {
		printf("Memory allocation failed.");
		exit(1);
	}
	/* (Shallow) copy the new entry to the table*/
	memcpy(&(table->entries[table->size - 1]), &to_add, sizeof(CodeTableEntry));
}


void opcode_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line) {
	int operand_num = 0;
	int bin_num = 0;
	code_structure ten_bit_code;
	/*gets the numeric value of an opcode*/
	operand_num = get_order_num(opernad);
	printf("order_num %d\n", operand_num);
	ten_bit_code.opcode = operand_num;
	ten_bit_code.src_addressing = src_add;
	ten_bit_code.dest_addressing = dst_add;
	ten_bit_code.ARE = 0;

	bin_num = ten_bit_code.opcode << 6
		| ten_bit_code.src_addressing << 4
		| ten_bit_code.dest_addressing << 2
		| ten_bit_code.ARE;

	code_table_line->code = decimalToBin(bin_num);
	code_table_line->address = (*IC - *L_ptr);

	add_to_code_table(codetable, *code_table_line);


	printf("bin number: %s\n", code_table_line->code);

}

void src_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable) {
	code_structure ten_bit_code;
	int bin_num = 0;
	if (src_add == IMMEDIATE_ADD) {
		ten_bit_code.address = src_oper[1];
		ten_bit_code.ARE = 0;
		bin_num = ten_bit_code.address << 2
			| ten_bit_code.ARE;
		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);
		return;
	}

	if (src_add == REGISTER_ADD) {
		ten_bit_code.src_reg_add = src_oper[1];
		ten_bit_code.dst_reg_add = 0;
		ten_bit_code.ARE = 0;
		bin_num = ten_bit_code.src_reg_add << 6
			| ten_bit_code.dst_reg_add << 2
			| ten_bit_code.ARE;
		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);
		return;
	}

	if (src_add == STRUCT_ADD) {
		/*first gets the label and checks in the symbol table*/
		char label_name[30] = { 0 };
		SymbolTableEntry* symbol_line;
		int i;
		for (i = 0; src_oper[i] != '.' && src_oper[i]; i++) {
			label_name[i] = src_oper[i];
		}
		label_name[i] = '\0';
		symbol_line = find_label_from_table(symboltable, label_name);
		
		ten_bit_code.address = symbol_line->counter;
		ten_bit_code.ARE = 2;
		bin_num = ten_bit_code.src_reg_add << 2
			| ten_bit_code.ARE;

		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);

		/*adding the number after the label*/
		src_oper = src_oper + strlen(label_name);
		bin_num = (int)src_oper[1];
		memset(code_table_line, 0, sizeof(code_table_line));
		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 2);
		add_to_code_table(codetable, *code_table_line);

		return;
	}

	if (src_add == DIRECT_ADD) {
		SymbolTableEntry* symbol_line = find_label_from_table(symboltable, src_oper);

		ten_bit_code.address = symbol_line->counter;
		ten_bit_code.ARE = 2;
		bin_num = ten_bit_code.src_reg_add << 2
			| ten_bit_code.ARE;

		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);
		return;
	}
}



void dst_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable) {
	code_structure ten_bit_code;
	int bin_num = 0;
	if (dst_add == IMMEDIATE_ADD) {
		ten_bit_code.address = src_oper[1];
		ten_bit_code.ARE = 0;
		bin_num = ten_bit_code.address << 2
			| ten_bit_code.ARE;
		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);
		return;
	}

	if (dst_add == REGISTER_ADD) {
		ten_bit_code.src_reg_add = 0;
		ten_bit_code.dst_reg_add = src_oper[1];
		ten_bit_code.ARE = 0;
		bin_num = ten_bit_code.src_reg_add << 6
			| ten_bit_code.dst_reg_add << 2
			| ten_bit_code.ARE;
		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);
		return;
	}

	if (dst_add == STRUCT_ADD) {
		/*first gets the label and checks in the symbol table*/
		char label_name[30] = { 0 };
		SymbolTableEntry* symbol_line;
		int i;
		for (i = 0; dst_oper[i] != '.' && dst_oper[i]; i++) {
			label_name[i] = dst_oper[i];
		}
		label_name[i] = '\0';
		symbol_line = find_label_from_table(symboltable, label_name);

		ten_bit_code.address = symbol_line->counter;
		ten_bit_code.ARE = 2;
		bin_num = ten_bit_code.src_reg_add << 2
			| ten_bit_code.ARE;

		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);

		/*adding the number after the label*/
		dst_oper = dst_oper + strlen(label_name);
		bin_num = (int)dst_oper[1];
		memset(code_table_line, 0, sizeof(code_table_line));
		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 2);
		add_to_code_table(codetable, *code_table_line);

		return;
	}

	if (dst_add == DIRECT_ADD) {
		SymbolTableEntry* symbol_line = find_label_from_table(symboltable, dst_oper);

		ten_bit_code.address = symbol_line->counter;
		ten_bit_code.ARE = 2;
		bin_num = ten_bit_code.src_reg_add << 2
			| ten_bit_code.ARE;

		code_table_line->code = decimalToBin(bin_num);
		code_table_line->address = (*IC - *L_ptr + 1);
		add_to_code_table(codetable, *code_table_line);
		return;
	}
}

