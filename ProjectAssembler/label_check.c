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


char* directive_list[5] = { "data","string","struct","entry","extern" };
char* order_list[16] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt" };
char* reg_list[8] = { "r0","r1","r2","r3","r4","r5","r6","r7" };


/*extract order from text*/
char* get_order(line_details line) {
	int i;
	int j;
	int cmp;
	char order[4] = { 0 };
	while (isspace(*(line.line))) { ((line.line))++; }
	for (int j = 0; line.line[j] != ' ' && line.line[j]; j++) {
		order[j] = line.line[j];
	}
	for (i = 0; i < NUM_ORDERS; i++) {
		cmp = strcmp(order, order_list[i]);
		if (cmp == 0) {
			return order;
		}
	}
	return order;
}

/*check if text is one of the orders from order_list*/
bool is_order(line_details line) {
	int i;
	int j;
	int cmp = 0;
	char order[5] = { 0 };
	while (isspace(*(line.line))) { ((line.line))++; }
	for (int j = 0; line.line[j] != ' ' && line.line[j]; j++) {
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
		if (!strncmp(text, directive_list[i], strlen(text))) {
			printf_line_error(line, "directive reserved paramater");
			return true;
		}
	}

	for (i = 0; i < NUM_ORDERS; i++) {
		if (!strncmp(text, order_list[i], strlen(text))) {
			printf_line_error(line, "order reserved paramater");
			return true;
		}
	}

	for (i = 0; i < NUM_REGERSITERS; i++) {
		if (!strncmp(text, reg_list[i], strlen(text))) {
			printf_line_error(line, "register reserved paramater");
			return true;
		}
	}
	return false;
}

bool is_label_valid(line_details line, char* text) {
	/* Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits*/
	bool is_valid_label = true;
	bool is_alphnumeric = true;
	int i;
	/*check each char in the string if it is non alphanumeric char*/
	for (i = 0; text[i] != ':' && text[i]; i++) {
		is_alphnumeric = is_alphnumeric && (isalpha(text[i]) || isdigit(text[i]));
		if (!is_alphnumeric) { break; }
	}

	is_valid_label &= is_alphnumeric;
	is_valid_label &= strlen(text) <= MAX_LABEL_LENGTH;
	is_valid_label &= (isalpha(text[0]) > 0);
	is_valid_label &= (text[i] == ':');
	is_valid_label &= !is_reserved_word(line, text);

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



char* get_label(line_details line) {
	char* label = { 0 };
	int i = 0;
	label = (char*)malloc(30);
	if (!label) {
		/*TODO: Print error*/ 
		exit(1);
	}

	bool is_label = true;
	while (isspace(*(line.line))) { (line.line)++; }
	is_label = is_label_valid(line, line.line);
	if (is_label) {
		for (i; line.line[i] != ':'; i++) {
			label[i] = line.line[i];
		}
	label[i] = '\0';
	return label;
	}
	label[i] = '\0';
	printf("label in that point %c\n", *label);
	return label;

}