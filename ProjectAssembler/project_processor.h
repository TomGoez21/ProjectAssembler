#ifndef PROJECT_PROCESSOR_H_
#define PROJECT_PROCESSOR_H_


#define MAX_LINE_LENGTH 80
#define MAX_DATA_IMAGE_LENGTH 2000
#define MAX_CODE_IMAGE_LENGTH 2000
#pragma warning(disable : 4996).

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


#endif