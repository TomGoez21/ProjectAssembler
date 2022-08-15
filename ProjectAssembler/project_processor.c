
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
#include "string_utils.h"
#include "macro.h"
#include "write_to_file.h"
#include "project_processor.h"

char line[MAX_LINE_LENGTH] = { 0 };

void line_handler(
	SymbolTable* symboltable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH],
	char* extern_filename
);

void line_handler_sec_pass(
	SymbolTable* symboltable,
	CodeTable* codetable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH],
	char* extern_filename);

bool process_file(char* filename, SymbolTable* symboltable, CodeTable* codetable) {
	long IC = 100;
	long DC = 0;
	/*long* data_image = (long*)calloc(1, sizeof(long));*/
	//long* code_image = (long*)calloc(1, sizeof(long));
	long* data_image[MAX_DATA_IMAGE_LENGTH] = { 0 };
	long code_image[MAX_CODE_IMAGE_LENGTH][MAX_LINE_LENGTH] = { 0 };
	unsigned int line_count = 0;
	FILE* file_dst; /* Current assembly file descriptor to process */

	/*creat file path for externals file. change to .ext extension in filename*/
	char* extern_filename = calloc(strlen(filename) + 2, sizeof(char));
	strcpy(extern_filename, filename);
	extern_filename[strlen(filename) - 2] = 'e';
	extern_filename[strlen(filename) - 1] = 'x';
	extern_filename[strlen(filename)] = 't';
	extern_filename[strlen(filename) + 1] = '\0';

	/* Open .am file. Skip if fails */
	file_dst = fopen(filename, "r");
	if (file_dst == NULL) {
		/* if file couldn't be opened, write to stderr. */
		fprintf(stderr, "Error: Can't access to \"%s.as\" . skipping it.\n", filename);
		free(filename); /* The only allocated space is for the full file name */
		set_error(true);
		return false;
	}


	while (fgets(line, MAX_LINE_LENGTH, file_dst)) {
		if (isEmetyLine(line) || isCommnetLine(line)) {
			continue;
		}
		line_count++;
		line_handler(symboltable, line_count, line, filename, &DC, &IC, data_image, code_image, &extern_filename);
	}


	/*second pass on all lines in order to detemine the value of each word. second pass happens only if there were no errors.*/
	if (!(set_error(false))) {
		rewind(file_dst);
		line_count = 0;
		IC = 100;
		DC = 0;

		while (fgets(line, MAX_LINE_LENGTH, file_dst)) {
			line_count++;
			line_handler_sec_pass(symboltable, codetable, line_count, line, filename, &DC, &IC, data_image, code_image, extern_filename);
		}
		data_image_to_code_table(data_image, codetable, &IC, &DC);

		/*creates .ob and .ext files*/
		write_code_table_to_file(codetable, filename);
		write_to_entry_file(symboltable, filename);
	}


}

void line_handler(
	SymbolTable* symboltable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH],
	char* extern_filename
) {
	/*L is number of code words in a line*/
	long L = 0;
	char* oper = 0;
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
	label = get_label(ld, symboltable, false);

	/*checks wherther it is .extern or .entry directive in the beginning of the line*/
	if (is_directive(ld.line)) {
		char dir_beginning[10] = { 0 };
		/*find what kind of directive it is*/
		directive directive_type = find_directive_type(ld, ld.line, dir_beginning);
		/*continue to the next word after directive*/
		ld.line = ld.line + strlen(dir_beginning) + 1;
		char* label_after_directive = { 0 };
		label_after_directive = get_first_word(ld.line);
		if (!is_label_valid_in_text(ld, label_after_directive)) {
			printf_line_error(ld, "label, %s ,is ilegal", label_after_directive);
			set_error(true);
		}

		if (directive_type == _string || directive_type == _data || directive_type == _struct) {
			printf_line_error(ld, "%s supposed to come after label", directive_type);
			set_error(true);
		}
		if (directive_type == _extern) {
			line_to_table->counter = 0;
			line_to_table->symbol_name = label_after_directive;
			line_to_table->type = _EXTERN;
		}

		if (directive_type == _entry) {
			line_to_table->counter = 0;
			line_to_table->symbol_name = label_after_directive;
			line_to_table->type = _ENTRY;
		}

		/*add symbol to symboltable*/
		add_to_table(symboltable, *line_to_table);
	}

	/*continue to the next word*/
	if (*label) {
		ld.line = ld.line + strlen(label) + 1;
	}
	else {
		ld.line = ld.line + strlen(label);
	}
	while (isspace(*(ld.line))) { (ld.line)++; }


	/*check whether it is one of .data, .struct, .string, .extern, .entry*/
	if (is_directive(ld.line)) {
		char dir[10] = { 0 };
		/*find what kind of directive it is*/
		directive directive_type = find_directive_type(ld, ld.line, dir);
		if (*label) {
			if (directive_type == _string || directive_type == _data || directive_type == _struct) {
				line_to_table->counter = *DC + *IC;
				line_to_table->symbol_name = label;
				line_to_table->type = _DATA;
			}
			if (directive_type == _extern) {
				printf_line_error(ld, "ignoring labels in the beginning of .extern line");
				set_error(true);

				line_to_table->counter = 0;
				line_to_table->symbol_name = label;
				line_to_table->type = _EXTERN;
			}
			/*add symbol to symboltable*/
			add_to_table(symboltable, *line_to_table);
			/*dealing with entry in the second pass*/
			if (directive_type == _entry) {
				printf_line_error(ld, "ignoring labels in the beginning of .entry line");
				set_error(true);
			}

			ld.line = ld.line + strlen(dir) + 1;
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
			/*add symbol to symboltable*/
			add_to_table(symboltable, *line_to_table);
		}

		/*check if word is order from the order_list. If so, analyze the operands*/
		if (is_order(ld)) {
			/* check the structre of the order. return number of words this code is translated to*/
			validate_operand_addressing(&oper, &L, ld, &src_address, &dst_address, &src_oper, &dst_oper, code_image_ptr, IC);
			if (src_address == REGISTER_ADD && dst_address == REGISTER_ADD) {
				(*IC)--;
			}
		}
	}
}



void line_handler_sec_pass(
	SymbolTable* symboltable,
	CodeTable* codetable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH],
	char* extern_filename)
{
	/*L is number of code words in a line*/
	long L = 0;
	char* oper = 0;
	line_details ld;
	addressing_type src_address = NONE;
	addressing_type dst_address = NONE;
	CodeTableEntry* code_table_line = malloc(sizeof(CodeTableEntry));
	char* src_oper = NULL;
	char* dst_oper = NULL;
	ld.line_number = line_count;
	ld.file_name = file_name;
	ld.line = line;

	/*gets a label. returns null if no label found*/
	char* label = { 0 };
	label = get_label(ld, symboltable, true);

	/*continue to the next word*/
	if (*label) {
		ld.line = ld.line + strlen(label) + 1;
	}
	else {
		ld.line = ld.line + strlen(label);
	}
	while (isspace(*(ld.line))) { (ld.line)++; }


	/*check whether it is one of .data, .struct, .string, .extern, .entry*/
	if (is_directive(ld.line)) {
		char dir[10] = { 0 };
		/*find what kind of directive it is*/
		directive directive_type = find_directive_type(ld, ld.line, dir);
		if (*label) {
			if (directive_type == _entry) {
				printf_line_error(ld, "ignoring labels in the beginning of .entry line");
				set_error(true);
			}

			ld.line = ld.line + strlen(dir) + 1;
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
		/*check if word is order from the order_list. If so, analyze the operands*/
		if (is_order(ld)) {
			/* check the structre of the order. return number of words this code is translated to*/
			validate_operand_addressing(&oper, &L, ld, &src_address, &dst_address, &src_oper, &dst_oper, code_image_ptr, IC);
			opcode_to_bin(&L, IC, oper, src_address, dst_address, src_oper, dst_oper, codetable, code_table_line);
			if (src_address == REGISTER_ADD && dst_address == REGISTER_ADD) {
				(*IC)--;
				src_dst_reg_to_bin(&L, IC, oper, src_address, dst_address, src_oper, dst_oper, codetable, code_table_line, symboltable);
			}
			else {
				src_to_bin(&L, IC, oper, src_address, dst_address, src_oper, dst_oper, codetable, code_table_line, symboltable, extern_filename);
				dst_to_bin(&L, IC, oper, src_address, dst_address, src_oper, dst_oper, codetable, code_table_line, symboltable, extern_filename);
			}
		}
	}

}


char* preAssemblerProccess(char* filename)
{
	FILE* fd;
	FILE* newFp;
	macroList* list;
	char line[MAX_LINE_LENGTH];
	char currWord[MAX_LINE_LENGTH];
	int macroFlag = 0;
	char* macroBuffer = (char*)malloc(MAX_LINE_LENGTH);
	char macroName[MAX_LINE_LENGTH];
	char leadingWhiteSpace[MAX_LINE_LENGTH];
	char* concatWhiteSpaces = "";
	macro* foundedMacro;
	char* newFile = (char*)calloc(MAX_LINE_LENGTH * sizeof(char), sizeof(char*));
	char* input_file;
	strcpy(newFile, "");
	strcpy(macroBuffer, "");
	strcpy(leadingWhiteSpace, "");
	input_file = cat_str(filename, ".as");
	if (!(fd = fopen(input_file, "r")))
	{
		fprintf(stderr, "cannot  open file\n");
		exit(-10);
	}
	list = createNewMacroList();
	while (!feof(fd))
	{
		if (fgets(line, MAX_LINE_LENGTH, fd) != NULL)
		{
			strcpy(currWord, getFirstWordFromALine(line, currWord));
			if (macroFlag)
			{
				if (!isEndMacroLabel(currWord))
				{
					macroBuffer = (char*)realloc(macroBuffer, strlen(macroBuffer) + MAX_LINE_LENGTH * 2);
					strcat(macroBuffer, removeLeadingWhiteSpaces(line));
					continue;
				}
				else
				{
					AddToMacroList(createNewMacro(macroName, macroBuffer), &list);
					PrintList(&list);
					macroFlag = FALSE;
					strcpy(macroBuffer, "");
					continue;
				}
			}
			foundedMacro = getFromMacroList(currWord, &list);
			if (foundedMacro != NULL)
			{
				strcpy(leadingWhiteSpace, getLeadingWhiteSpace(line));
				newFile = (char*)realloc(newFile, (strlen(newFile) + countLines(foundedMacro->val)) * MAX_LINE_LENGTH + 1);
				concatWhiteSpaces = concatWhiteSpacesPerEachLine(foundedMacro->val, leadingWhiteSpace);
				strcat(newFile, concatWhiteSpaces);
			}
			else if (isMacroLabel(currWord))
			{
				macroFlag = TRUE;
				strcpy(line, removeLeadingWhiteSpaces(line));
				strcpy(line, line + (strlen(currWord) + 1));
				strcpy(currWord, getFirstWordFromALine(line, currWord));
				strcpy(macroName, currWord);
				if (checkIfMacroInList(macroName, &list))
				{
					/*return error*/
				}
			}
			else
			{
				newFile = (char*)realloc(newFile, (strlen(newFile) + MAX_LINE_LENGTH));
				if (newFile == NULL) {
					fprintf(stderr, "Failed to alocate");
				}
				strcat(newFile, line);
			}
		}
	}
	printf("--------------------NEW MACRO FILE-------------------------\n%s\n", newFile);
	input_file[strlen(input_file) - 1] = 'm';
	newFp = fopen(input_file, "w");
	if (fputs(newFile, newFp) == EOF) {
		exit(-11);
	};
	freeMacroList(list);
	free(macroBuffer);
	fclose(newFp);
	return input_file;
}