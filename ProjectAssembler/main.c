#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "symbol_table.h"
#include "directives.h"
#include "label_check.h"
#include "orders.h"

#define MAX_LINE_LENGTH 80
char line[MAX_LINE_LENGTH] = { 0 };
#pragma warning(disable : 4996)

static bool process_file(char* filename, SymbolTable* symboltable);
void line_handler(SymbolTable* symboltable, int line_count, char* line, char* file_name, int* DC, int* IC, long** data_image_ptr, long** code_image_ptr);

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
	long* data_image = (long*)malloc(1 * sizeof(long));
	long* code_image = (long*)malloc(1 * sizeof(long));
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
		line_handler(symboltable, line_count, line, input_file, &DC, &IC, &data_image, &code_image);
	}
	printf("\nhandled all lines :)\n");

}

void line_handler(
	SymbolTable* symboltable,
	int line_count,
	char* line,
	char* file_name,
	int* DC,
	int* IC,
	long** data_image_ptr,
	long** code_image_ptr
) {
	line_details ld;
	SymbolTableEntry* line_to_table = malloc(sizeof(SymbolTableEntry));

	ld.line_number = line_count;
	ld.file_name = file_name;
	ld.line = line;
	char* index;

/*gets a label. returns null if no label found*/
	char* label = get_label(ld.line);
	/*continue to the next word*/
	ld.line = ld.line + strlen(label) + 1;
	while (isspace(*(ld.line))) { (ld.line)++; }

	/*check whether it is one of .data, .struct, .string, .extern, .entry*/
	if (is_directive(ld.line)) {
		/*find what kind of directive it is*/
		directive directive_type = find_directive_type(ld, ld.line);
		if (label) {
			if (directive_type == _string || directive_type == _data || directive_type == _struct) {
				line_to_table->counter = *DC;
				line_to_table->symbol_name = label;
				line_to_table->type = _DATA;
			}
			if (directive_type == _extern) {
				line_to_table->counter = 0;
				line_to_table->symbol_name = label;
				line_to_table->type = _EXTERN;
			}
			/*add symbol to symboltable*/
			add_to_table(symboltable, *line_to_table);
			/*dealing with entry in the second pass*/
			if (directive_type == _entry) {
				printf_line_error(ld, "ignoring labels in the beginning of .entry line");
			}
		}
		if (directive_type == _data) {
			data_handler(ld, ld.line, DC, data_image_ptr);
		}
		if (directive_type == _string) {
			string_handler(ld, ld.line, DC, data_image_ptr);
		}
		if (directive_type == _struct) {
			struct_handler(ld, ld.line, DC, data_image_ptr);
		}
	}
	else {
		if (label) {
			line_to_table->counter = *IC;
			line_to_table->symbol_name = label;
			line_to_table->type = _CODE;
		}
		/*check if word is order from the order_list. If so, analyze the operands*/
		if (is_order(ld)) {
			/* check the structre of the order. return number of words this code is translated to*/
			int L = operands_check(ld, code_image_ptr, IC);
			//binary_conversion_to_base_32(line, IC);
			IC += L;
			/*TO DO: using the final value of IC to detemrine the value of the data symbols*/
		}
	}


	
}
