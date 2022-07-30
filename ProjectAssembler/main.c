#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "symbol_table.h"
#include "directives.h"

#define MAX_LINE_LENGTH 80
char line[MAX_LINE_LENGTH] = { 0 };
#pragma warning(disable : 4996)

static bool process_file(char* filename, SymbolTable* symboltable);
void line_handler(SymbolTable* symboltable, int line_count, char* line, char* file_name, int* DC, int* IC, long* data_image);

int main(int argc, char* argv[]) {
	int i;
	SymbolTable symboltable;
	/* To break line if needed */
	bool succeeded = true;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* if last process failed and there's another file, break line: */
		if (!succeeded) puts("");
		/* for each file name, send it for full processing. */
		succeeded = process_file(argv[i], &symboltable);
		/* Line break if failed */
	}
	return 0;
}


bool process_file(char* filename, SymbolTable* symboltable) {
	int DC = 0;
	int IC = 0;
	long* data_image = { 0 };
	unsigned int line_count = 0;
	bool is_success = true;
	FILE* file_dst; /* Current assembly file descriptor to process */
	char* input_file;

	/* Concat filename with .as extension */
	input_file = cat_str(filename, ".as");

	/* Open file. Skip if fails */
	file_dst = fopen(input_file, "r");
	if (file_dst == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: Can't access to \"%s.as\" . skipping it.\n", filename);
		free(input_file); /* The only allocated space is for the full file name */
		return false;
	}
	while (fgets(line, MAX_LINE_LENGTH, file_dst)) {
		printf("line[%06d]: %s", ++line_count, line);
		line_handler(symboltable, line_count, line, input_file, &DC, &IC, &data_image);

	}
}

void line_handler(SymbolTable* symboltable, int line_count, char* line, char* file_name, int* DC, int* IC, long* data_image) {
	line_details ld;
	SymbolTableEntry line_to_table;
	ld.line_number = line_count;
	ld.file_name = file_name;
	ld.line = line;


	char* label = get_label(ld);
	/*check whether it is one of .data, .struct, .string, .extern, .entry*/
	if (is_directive(ld)) {
		/*find what kind of directive it is*/
		directive directive_type = handle_directive(ld);
		if (label) {
			if (directive_type == _string || directive_type == _data || directive_type == _struct) {
				line_to_table.counter = *DC;
				line_to_table.symbol_name = label;
				line_to_table.type = _DATA;
			}
			if (directive_type == _extern) {
				line_to_table.counter = 0;
				line_to_table.symbol_name = label;
				line_to_table.type = _EXTERN;
			}
			if (directive_type == _entry) {
				line_to_table.counter = 0;
				line_to_table.symbol_name = label;
				line_to_table.type = _ENTRY;
			}
			/*add symbol to symboltable*/
			add_to_table(symboltable, line_to_table);
		}
		if (directive_type == _data) {
			data_handler(ld, ld.line, DC, data_image);
		}
		if (directive_type == _string) {
			string_handler(ld, ld.line, DC, data_image);
		}
		if (directive_type == _struct) {
			struct_handler(ld, ld.line, DC, data_image);
		}
	}
	else {
		if (label) {
			line_to_table.counter = *IC;
			line_to_table.symbol_name = label;
			line_to_table.type = _CODE;
		}
		/*check if word is order from the order_list. If so, analyze the operands*/
		if (is_order(ld)) {
			/* check the structre of the order. return number of words this code is translated to*/
			int L = operands_check(ld);
			binary_conversion_to_base_32(first_word);

			IC += L;
		}
	}


	
}
