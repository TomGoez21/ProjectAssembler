#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "symbol_table.h"
#include "directives.h"
#include "label_check.h"
#include "orders.h"
#include "code_parse.h"



#define MAX_LINE_LENGTH 80
#define MAX_DATA_IMAGE_LENGTH 2000
#define MAX_CODE_IMAGE_LENGTH 2000
char line[MAX_LINE_LENGTH] = { 0 };
#pragma warning(disable : 4996)

static bool process_file(char* filename, SymbolTable* symboltable);
void line_handler(SymbolTable* symboltable, int line_count, char* line, char* file_name, long *DC, long *IC, long** data_image_ptr, long** code_image_ptr);

int main(int argc, char* argv[]) {
	int i;
	SymbolTable symboltable;
	symboltable.size = 0;
	symboltable.entries = NULL;

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
	long DC = 0;
	long IC = 0;
	/*long* data_image = (long*)calloc(1, sizeof(long));
	long* code_image = (long*)calloc(1, sizeof(long));*/
	long* data_image[MAX_DATA_IMAGE_LENGTH] = {0};
	long* code_image[MAX_CODE_IMAGE_LENGTH] = { 0 };
	unsigned int line_count = 0;
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
		printf("-------------\n");
		printf("line[%06d]: %s", ++line_count, line);
		line_handler(symboltable, line_count, line, input_file, &DC, &IC, &data_image, &code_image);
		printf("\nDC:%ld\n", DC);
		/*printf("data_image:%l\n", data_image);*/
		printf("IC:%ld\n", IC);

	}
	printf("\nhandled all lines :)\n");

}

void line_handler(
	SymbolTable* symboltable,
	int line_count,
	char* line,
	char* file_name,
	long *DC,
	long *IC,
	long** data_image_ptr,
	long** code_image_ptr
) {
	line_details ld;
	addressing_type src_address = NONE;
	addressing_type dst_address = NONE;
	char* src_oper = NULL;
	char* dst_oper = NULL;
	SymbolTableEntry* line_to_table = malloc(sizeof(SymbolTableEntry));

	ld.line_number = line_count;
	ld.file_name = file_name;
	ld.line = line;

/*gets a label. returns null if no label found*/
	char* label = { 0 };
	label = get_label(ld);
	printf("\nlabel:%s\n", label);
	printf("length label:%ld\n", strlen(label));


	/*continue to the next word*/
	ld.line = ld.line + strlen(label) + 1;
	while (isspace(*(ld.line))) { (ld.line)++; }

	printf("points after label to:%c\n", *ld.line);

	/*check whether it is one of .data, .struct, .string, .extern, .entry*/
	if (is_directive(ld.line)) {
		char dir[10] = { 0 };
		/*find what kind of directive it is*/
		directive directive_type = find_directive_type(ld, ld.line, dir);
		printf("the directive is: %s\n", dir);
		if (*label) {
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

		ld.line = ld.line + strlen(dir) + 1;
		printf("points after directie to:%c\n", *ld.line);
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
		if (*label) {
			line_to_table->counter = *IC;
			line_to_table->symbol_name = label;
			line_to_table->type = _CODE;
		}
		/*check if word is order from the order_list. If so, analyze the operands*/
		if (is_order(ld)) {
			/* check the structre of the order. return number of words this code is translated to*/
			validate_operand_addressing(ld, src_address, dst_address, src_oper, dst_oper);
			/*int L = operands_check(ld, code_image_ptr, IC);
			binary_conversion_to_base_32(line, IC);
			IC += L;*/
			/*TO DO: using the final value of IC to detemrine the value of the data symbols*/
		}
	}


	
}
