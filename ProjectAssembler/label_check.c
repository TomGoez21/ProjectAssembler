#include "symbol_table.h"
#include "utils.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include "label_check.h"
#include "utils.h"

#define MAX_LABEL_LENGTH 30
#define NUM_DIRECTIVES 5
#define NUM_ORDERS 16
#define NUM_REGERSITERS 8

bool is_reserved_word(char* text) {
	int i;
	for (i = 0; i < NUM_DIRECTIVES; i++) {
		if (!strncmp(text, directive_list[i], strlen(text) - 1)) {
			return true;
		}
	}

	for (i = 0; i < NUM_ORDERS; i++) {
		if (!strncmp(text, order_list[i], strlen(text) - 1)) {
			return true;
		}
	}

	for (i = 0; i < NUM_REGERSITERS; i++) {
		if (!strncmp(text, reg_list[i], strlen(text) - 1)) {
			return true;
		}
	}
	return false;
}

bool is_label_valid(char* text) {
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
	is_valid_label &= !is_reserved_word(text);

	printf("%d\n", is_valid_label);
	return is_valid_label;
}


