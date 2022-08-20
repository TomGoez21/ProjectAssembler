#ifndef PROJECT_PROCESSOR_H_
#define PROJECT_PROCESSOR_H_


#define MAX_LINE_LENGTH 82
#define ERROR_FILE_NOT_FOUND -10
#define ERROR_LINE_SIZE_TO_BIG -11
#define MAX_DATA_IMAGE_LENGTH 2000
#define MAX_CODE_IMAGE_LENGTH 2000


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "symbol_table.h"

#include "code_parse.h"





bool process_file(char* filename, SymbolTable* symboltable, CodeTable* codetable);

char* preAssemblerProccess(char* filename);

void line_handler(
	SymbolTable* symboltable,
	int line_count,
	char* line,
	char* file_name,
	long* DC,
	long* IC,
	long** data_image_ptr,
	long code_image_ptr[][MAX_LINE_LENGTH]
);


#endif