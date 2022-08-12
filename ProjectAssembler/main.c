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



#define MAX_LINE_LENGTH 80
#define MAX_DATA_IMAGE_LENGTH 2000
#define MAX_CODE_IMAGE_LENGTH 2000
char line[MAX_LINE_LENGTH] = { 0 };
#pragma warning(disable : 4996)

static bool process_file(char* filename, SymbolTable* symboltable, CodeTable* codetable);
char* preAssemblerProccess(char* filename);
void line_handler(
	SymbolTable* symboltable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH]);

void line_handler_sec_pass(
	SymbolTable* symboltable,
	CodeTable* codetable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH]
);


int main(int argc, char* argv[]) {
	int i;

	/* To break line if needed */
	bool succeeded = true;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* if last process failed and there's another file, break line: */
		if (!succeeded) puts("");
		argv[i] = preAssemblerProccess(argv[i]);
		/* for each file name, send it for full processing. */
		SymbolTable symboltable;
		symboltable.size = 0;
		symboltable.entries = NULL;
		CodeTable codetable;
		codetable.size = 0;
		codetable.entries = NULL;

		succeeded = process_file(argv[i], &symboltable, &codetable);
		/* Line break if failed */
	}
	return 0;
}


bool process_file(char* filename, SymbolTable* symboltable, CodeTable* codetable) {
	long IC = 100;
	long DC = 0;
	/*long* data_image = (long*)calloc(1, sizeof(long));*/
	//long* code_image = (long*)calloc(1, sizeof(long));
	long* data_image[MAX_DATA_IMAGE_LENGTH] = { 0 };
	long code_image[MAX_CODE_IMAGE_LENGTH][MAX_LINE_LENGTH] = { 0 };
	unsigned int line_count = 0;
	FILE* file_dst; /* Current assembly file descriptor to process */

	/* Open file. Skip if fails */
	file_dst = fopen(filename, "r");
	if (file_dst == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: Can't access to \"%s.as\" . skipping it.\n", filename);
		free(filename); /* The only allocated space is for the full file name */
		return false;
	}


	while (fgets(line, MAX_LINE_LENGTH, file_dst)) {
		printf("-------------\n");
		printf("line[%06d]: %s", ++line_count, line);
		
		if(isEmetyLine(line) || isCommnetLine(line)){
			continue;
		}

		line_handler(symboltable, line_count, line, filename, &DC, &IC, data_image, code_image);
		printf("\nDC:%ld\n", DC);
		/*printf("data_image:%l\n", data_image);*/
		printf("IC:%ld\n", IC);

	}
	int i = 100;
	for (; i < IC; i++) {
		printf("code image: %s\n", code_image[i]);
	}
	i = 0;
	for (; i < DC; i++) {
		printf("data_image: %d\n", data_image[i]);
	}

	i = 0;
	for (; i < symboltable->size; i++) {
		printf("symbol name: %s\n", symboltable->entries[i].symbol_name);
		printf("symbol type: %d\n", symboltable->entries[i].type);
		printf("counter: %d\n", symboltable->entries[i].counter);
	}


	printf("\nhandled all lines :)\n");


	/*second pass on all lines in order to detemine the value of each word*/
	rewind(file_dst);
	line_count = 0;
	IC = 100;
	DC = 0;

	while (fgets(line, MAX_LINE_LENGTH, file_dst)) {
		printf("\n-------------\n");
		printf("line[%06d]: %s", ++line_count, line);
		line_handler_sec_pass(symboltable, codetable, line_count, line, input_file, &DC, &IC, data_image, code_image);
	}

	i = 0;
	for (; i < codetable->size; i++) {
		printf("code: %s\n", codetable->entries[i].code);
		printf("address: %d\n", codetable->entries[i].address);
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
	long code_image_ptr[][MAX_LINE_LENGTH]
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
	label = get_label(ld);
	printf("\nlabel:%s\n", label);
	printf("length label:%lu\n", strlen(label));

	/*continue to the next word*/
	ld.line = ld.line + strlen(label) + 1;
	while (isspace(*(ld.line))) { (ld.line)++; }

	printf("points after label to:%s\n", ld.line);

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
			printf("points after directvie to:%c\n", *ld.line);
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
	long code_image_ptr[][MAX_LINE_LENGTH]
) {
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
	label = get_label(ld);
	printf("\nlabel:%s\n", label);
	printf("length label:%lu\n", strlen(label));

	/*continue to the next word*/
	ld.line = ld.line + strlen(label) + 1;
	while (isspace(*(ld.line))) { (ld.line)++; }

	printf("points after label to:%s\n", ld.line);

	/*check whether it is one of .data, .struct, .string, .extern, .entry*/
	if (is_directive(ld.line)) {
		char dir[10] = { 0 };
		/*find what kind of directive it is*/
		directive directive_type = find_directive_type(ld, ld.line, dir);
		printf("the directive is: %s\n", dir);
		if (*label) {
			/* TODO: dealing with entry in the second pass*/
			if (directive_type == _entry) {
				printf_line_error(ld, "ignoring labels in the beginning of .entry line");
			}

			ld.line = ld.line + strlen(dir) + 1;
			printf("points after directvie to:%c\n", *ld.line);
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
				printf("regester src & dst");
			}
			else {
				src_to_bin(&L, IC, oper, src_address, dst_address, src_oper, dst_oper, codetable, code_table_line, symboltable);
				dst_to_bin(&L, IC, oper, src_address, dst_address, src_oper, dst_oper, codetable, code_table_line, symboltable);
			
			}
		}
	}

}



char* preAssemblerProccess(char* filename)
{
	FILE* fd;
	char line[MAX_LINE_LENGTH];
	char currWord[MAX_LINE_LENGTH];
	int macroFlag = 0;
	char* macroBuffer = (char*)malloc(MAX_LINE_LENGTH);
	char macroName[MAX_LINE_LENGTH];
	char leadingWhiteSpace[MAX_LINE_LENGTH] ;
	char* concatWhiteSpaces = "";
	macro *foundedMacro;
	char *newFile = (char*) calloc(MAX_LINE_LENGTH * sizeof(char) , sizeof(char*));
#pragma warning(suppress : 4996)
	strcpy(newFile, "");
	strcpy(macroBuffer, "");
	strcpy(leadingWhiteSpace, "");
	
	if (!(fd = fopen(filename, "r")))
	{
		fprintf(stderr, "cannot  open file\n");
		exit(-10);
	}
	macroList *list = createNewMacroList();
	while (!feof(fd))
	{
		if (fgets(line, MAX_LINE_LENGTH, fd) != NULL)
		{
			strcpy(currWord, getFirstWordFromALine(line , currWord));
			if (macroFlag)
			{
				if (!isEndMacroLabel(currWord))
				{
					macroBuffer = (char*)realloc(macroBuffer, strlen(macroBuffer) + MAX_LINE_LENGTH*2);
					strcat(macroBuffer, removeLeadingWhiteSpaces(line));
					continue;
				}
				else
				{
					AddToMacroList(createNewMacro(macroName, macroBuffer), &list);
					printf("macro added, key-%s \nval-> \n%s \n", macroName, macroBuffer);
					PrintList(&list);
					macroFlag = FALSE;
					strcpy(macroBuffer,"");
					continue;
				}
			}
			foundedMacro = getFromMacroList(currWord, &list);
			if (foundedMacro != NULL)
			{
				strcpy(leadingWhiteSpace, getLeadingWhiteSpace(line));
				newFile = (char *)realloc(newFile, (strlen(newFile) + countLines(foundedMacro->val)) * 81 + 1);
				concatWhiteSpaces = concatWhiteSpacesPerEachLine(foundedMacro->val, leadingWhiteSpace);
				strcat(newFile, concatWhiteSpaces);
			}
			else if (isMacroLabel(currWord))
			{
				macroFlag = TRUE;
				strcpy(line, removeLeadingWhiteSpaces(line));
				strcpy(line, line + (strlen(currWord) + 1));
				strcpy(currWord, getFirstWordFromALine(line , currWord));
				strcpy(macroName, currWord);
				if (checkIfMacroInList(macroName, &list))
				{
					/*return error*/
				}
			}
			else
			{
				newFile = (char*)realloc(newFile,  (strlen(newFile) + 81));
				if (newFile== NULL) {
					printf("Failed to alcote");
				}
				strcat(newFile, line);
			}
		}
	}
	printf("--------------------NEW MACRO FILE-------------------------\n%s\n" , newFile);
	FILE *newFp;
	filename[strlen(filename)-1] = 'm';
    newFp = fopen(filename, "w");
	if(fputs(newFile, newFp) == EOF){
		exit(-11);
	};
	freeMacroList(list);
	free(macroBuffer);
	free(concatWhiteSpaces);
	fclose(newFp);
	return filename;
}