#include "symbol_table.h"
#include "utils.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label_check.h"
#include "utils.h"
#include "code_parse.h"
#include "symbol_table.h"


char* directive_list[5] = { "data","string","struct","entry","extern" };
char* order_list[16] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt" };
char* reg_list[8] = { "r0","r1","r2","r3","r4","r5","r6","r7" };

/*gets the decimal value of an order*/
int* get_order_num(char* order) {
	int i;
	int cmp;
	for (i = 0; i < NUM_ORDERS; i++) {
		cmp = strcmp(order, order_list[i]);
		if (cmp == 0) {
			return i;
		}
	}
}

/*extract order from text*/
char* get_order(line_details line) {
	int i;
	int j;
	int cmp;
	char order[4] = { 0 };
	while (isspace(*(line.line))) { ((line.line))++; }
	for (j = 0; line.line[j] != ' ' && line.line[j]; j++) {
		order[j] = line.line[j];
	}
	for (i = 0; i < NUM_ORDERS; i++) {
		cmp = strcmp(order, order_list[i]);
		if (cmp == 0) {
			return order;
		}
		else {
			printf_line_error(line, "%s : not an order", order);
			set_error(true);
		}
	}
	return order;
}

/*check if text is one of the orders from order_list*/
bool is_order(line_details line) {
	int i;
	int j;
	int cmp = 0;
	char order[10] = { 0 };
	while (isspace(*(line.line))) { ((line.line))++; }
	for (j = 0; line.line[j] != ' ' && line.line[j]; j++) {
		order[j] = line.line[j];
	}
	for (i = 0; i < NUM_ORDERS; i++) {
		cmp = strcmp(order, order_list[i]);
		if (cmp == 0) {
			return true;
		}
	}
	return false;
}



bool is_reserved_word(line_details line, char* text) {
	int i;
	for (i = 0; i < NUM_DIRECTIVES; i++) {
		if (*text && strncmp(text, directive_list[i], strlen(text)) == 0) {
			printf_line_error(line, "%s is directive reserved paramater", text);
			set_error(true);
			return true;
		}
	}

	for (i = 0; i < NUM_ORDERS; i++) {
		if (*text && strncmp(text, order_list[i], strlen(text)) == 0) {
			printf_line_error(line, "%s is order reserved paramater", text);
			set_error(true);
			return true;
		}
	}

	for (i = 0; i < NUM_REGERSITERS; i++) {
		if (*text && strncmp(text, reg_list[i], strlen(text)) == 0) {
			printf_line_error(line, "%s is register reserved paramater", text);
			set_error(true);
			return true;
		}
	}
	return false;
}

bool is_label_valid(line_details line, char* text, SymbolTable* symboltable) {
	/* Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits*/
	bool is_valid_label = true;
	bool is_alphnumeric = true;
	int i;
	char label[30];
	/*check each char in the string if it is non alphanumeric char*/
	for (i = 0; text[i] != ':' && text[i] && text[i] != ' '; i++) {
		is_alphnumeric = is_alphnumeric && (isalpha(text[i]) || isdigit(text[i]));
		label[i] = text[i];
		if (!is_alphnumeric) { break; }
	}

	if (text[i] != ':') {
		return false;
	}
	label[i] = '\0';
	is_valid_label &= is_alphnumeric;
	is_valid_label &= strlen(text - 1) <= MAX_LABEL_LENGTH;
	is_valid_label &= (isalpha(text[0]) > 0);
	/*check if label is already in symboltable, thus not valid*/
	if (label_exists(symboltable, label)) {
		printf_line_error(line, "label %s already exists", label);
		is_valid_label &= !label_exists(symboltable, label);
	}
	is_valid_label &= !is_reserved_word(line, label);

	return is_valid_label;
}


bool is_label_valid_in_struct(line_details line, char* text) {
	/* Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits*/
	bool is_valid_label = true;
	bool is_alphnumeric = true;
	int i;
	/*check each char in the string if it is non alphanumeric char*/
	for (i = 0; text[i] != '.' && text[i]; i++) {
		is_alphnumeric = is_alphnumeric && (isalpha(text[i]) || isdigit(text[i]));
		if (!is_alphnumeric) { break; }
	}

	is_valid_label &= is_alphnumeric;
	is_valid_label &= strlen(text) <= MAX_LABEL_LENGTH;
	is_valid_label &= (isalpha(text[0]) > 0);
	is_valid_label &= (text[i] == '.');
	is_valid_label &= !is_reserved_word(line, text);

	return is_valid_label;
}

bool is_label_valid_in_text(line_details line, char* text) {
	/* Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits*/
	bool is_valid_label = true;
	bool is_alphnumeric = true;
	int i;
	/*check each char in the string if it is non alphanumeric char*/
	for (i = 0; text[i]; i++) {
		is_alphnumeric = is_alphnumeric && (isalpha(text[i]) || isdigit(text[i]));
		if (!is_alphnumeric) { break; }
	}

	is_valid_label &= is_alphnumeric;
	is_valid_label &= strlen(text) <= MAX_LABEL_LENGTH;
	is_valid_label &= (isalpha(text[0]) > 0);
	is_valid_label &= !is_reserved_word(line, text);
	
	return is_valid_label;
}

/*get the label name when in struct. comes after validation of the label*/
char* get_label_in_struct(char* text, char* label_name) {
	int i;
	/*check each char in the string if it is non alphanumeric char*/
	for (i = 0; text[i] != '.' && text[i]; i++) {
		label_name[i] = text[i];
	}
	label_name[i] = '\0';
	return label_name;

}


char* get_label(line_details line, SymbolTable* symboltable) {
	int i = 0;
	char *label = calloc(30, sizeof(char));
	if (!label) {
		printf_line_error(line, "cannot allocate memory for label");
		set_error(true);
	}

	bool is_label = true;
	while (isspace(*(line.line))) { (line.line)++; }
	is_label = is_label_valid(line, line.line, symboltable);
	if (is_label) {
		for (i; line.line[i] != ':'; i++) {
			label[i] = line.line[i];
		}
	label[i] = '\0';
	return label;
	}
	label[i] = '\0';
	return label;

}

/*checks whether the src and dst operands are ligal for the current opcode*/
void check_src_dst_per_opcode(char* opcode, addressing_type src_add, addressing_type dst_add, line_details line){
	if ((strcmp(opcode, "mov") == 0) || (strcmp(opcode, "add") == 0) || (strcmp(opcode, "sub") == 0)) {
		if (!(src_add == 0 || src_add == 1 || src_add == 2 || src_add == 3) || !(dst_add == 1 || dst_add == 2 || dst_add == 3)) {
			printf_line_error(line , "addressing type doesnt match opecode type");
			set_error(true);
		}
	}
	else if (strcmp(opcode, "cmp") == 0) {
		if (!(src_add == 0 || src_add == 1 || src_add == 2 || src_add == 3) || !(dst_add == 0 || dst_add == 1 || dst_add == 2 || dst_add == 3)) {
			printf_line_error(line, "addressing type doesnt match opecode type");
			set_error(true);
		}
	}
	else if ((strcmp(opcode, "not") == 0) || (strcmp(opcode, "clr") == 0) || (strcmp(opcode, "inc") == 0) || (strcmp(opcode, "dec") == 0) || (strcmp(opcode, "jmp") == 0) || (strcmp(opcode, "bne") == 0) || (strcmp(opcode, "get") == 0) || (strcmp(opcode, "jsr") == 0)) {
		if (!(src_add == 4) || !(dst_add == 1 || dst_add == 2 || dst_add == 3)) {
			printf_line_error(line, "addressing type doesnt match opecode type");
			set_error(true);
		}
	}
	else if (strcmp(opcode, "lea") == 0) {
		if (!(src_add == 1 || src_add == 2) || !(dst_add == 1 || dst_add == 2 || dst_add == 3)) {
			printf_line_error(line, "addressing type doesnt match opecode type");
			set_error(true);
		}
	}
	else if (strcmp(opcode, "prn") == 0) {
		if (!(src_add == 4) || !(dst_add == 0 || dst_add == 1 || dst_add == 2 || dst_add == 3)) {
			printf_line_error(line, "addressing type doesnt match opecode type");
			set_error(true);
		}
	}
	else if ((strcmp(opcode, "rts") == 0) || (strcmp(opcode, "hlt") == 0)) {
		if (!(src_add == 4) || !(dst_add == 4)) {
			printf_line_error(line, "addressing type doesnt match opecode type");
			set_error(true);
		}
	}
}

