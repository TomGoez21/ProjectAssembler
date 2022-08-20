#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "code_parse.h"
#include "project_processor.h"


int main(int argc, char* argv[]) {
	int i;
	SymbolTable symboltable;
	CodeTable codetable;
	/* To break line if needed */
	bool succeeded = true;
	/* Process each file by arguments */
	for (i = 1; i < argc; ++i) {
		/* if last process failed and there's another file, break line: */
		if (!succeeded) puts("");
		argv[i] = preAssemblerProccess(argv[i]);
		/* for each file name, send it for full processing. */
		symboltable.size = 0;
		symboltable.entries = NULL;
		codetable.size = 0;
		codetable.entries = NULL;
		succeeded = process_file(argv[i], &symboltable, &codetable);
		freeSymbolTable(&symboltable);
		freeCodeTable(&codetable);
	}

	return 0;
}
