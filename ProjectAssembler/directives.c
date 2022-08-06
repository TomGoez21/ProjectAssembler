#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "directives.h"
#include "utils.h"
#pragma warning(disable : 4996)

directive str_to_directive(char* str) {
	if (!strcmp(str, "data")) {
		return _data;
	}
	else if (!strcmp(str, "string")) {
		return _string;
	}
	else if (!strcmp(str, "struct")) {
		return _struct;
	}
	else if (!strcmp(str, "extern")) {
		return _extern;
	}
	else if (!strcmp(str, "entry")) {
		return _entry;
	}
	return _invalid;

}

/*Checks type of directive*/
directive find_directive_type(line_details line, char* begin, char *directive_string) {
	while (isspace(*begin)) { begin++; }
	if (*begin++ != '.') {
		printf_line_error(line, "directive should start with period");
		return _invalid;
	}

	int space_index = strcspn(begin, " ");
	strncpy(directive_string, begin, space_index);

	directive d = str_to_directive(directive_string);
	if (d == _invalid) {
		printf_line_error(line, "invalid directive %s", directive_string);
	}
	return d;

}


/*Handles string directive, 'begin' points to the first character*/
void string_handler(line_details line, char* begin, long *DC, long** data_image_ptr) {
	while (isspace(*begin)) { begin++; }
	/* Checks if the string argument has leading quotation marks */
	if (*begin++ != '"') {
		printf_line_error(line, "String directive should start with quotation marks");
		return;
	}
	char* last_quot = strrchr(begin, '"');

	if (!last_quot) {
		printf_line_error(line, "String should end quotation marks");
		return;
	}

	/*insert data to the data list*/
	while (*begin && *begin != '"') {
		/*printf("*DC=%d *data_image_ptr=%x, value of data_image = %s\n", *DC, *data_image_ptr, *data_image_ptr);*/
		/**data_image_ptr = (long*)realloc(*data_image_ptr, (*DC + 1) * sizeof(long));
		if (!(*data_image_ptr)) {
			printf_line_error(line, "unable to reallocate memory for data_image");
		}
		*/

		data_image_ptr[*DC] = *begin;
		printf(" data in DC place: %c\n", data_image_ptr[*DC]);
		printf(" DC: %d\n", *DC);
		begin++;
		(*DC)++;
	}

	/* add \0 to the end of string in data image */
	/**data_image_ptr = (long*)realloc(data_image_ptr, (*DC + 1) * sizeof(long));*/
	/*TODO: checkand print error*/
	data_image_ptr[*DC] = '\0';
	(*DC)++;

	if (*begin == '"') {
		begin++;
	}
	while (isspace(*begin)) { begin++; }
	/* checks for extra chracters after string directive */
	if (*begin) {
		printf_line_error(line, "Extra characters after end of directive");
	}

}

/* Handles the data directive when begin points to the first argument */
void data_handler(line_details line, char* begin, long *DC, long** data_image_ptr) {
	long num; /* Current integer being read */
	int ilen; /* Characters length of the integer */
	while (isspace(*begin)) { begin++; }

	/* Get first argument from the string input */
	if (sscanf(begin, "%d%n", &num, &ilen) <= 0) {
		printf_line_error(line, "expected number after .data");
		return;
	}
	begin += ilen;

	while (isspace(*begin)) { begin++; }
	if (*begin != ',' && *begin) {
		printf_line_error(line, "not an integer");
		return;
	}
	/* Insert the data into data_image */
	/**data_image_ptr = (long*)realloc(data_image_ptr, (*DC + 1) * sizeof(long));*/
	/*TODO: checkand print error*/
	data_image_ptr[*DC] = num;
	(*DC)++;


	/* For each argument the the .data directive, add it to data_image */
	while (*begin == ',') {
		begin++;

		while (isspace(*begin)) { begin++; }

		/* Gets argument */
		if (!sscanf(begin, "%d%n", &num, &ilen)) {
			printf_line_error(line, "Expected a number after comma");
			return;
		}

		begin += ilen;

		while (isspace(*begin)) { begin++; }
		printf("%c\n", *begin);

		if (*begin != ',' && *begin != 0) {
			printf_line_error(line, "not an integer");
			return;
		}
		begin += ilen;

		/* Insert the data into data_image */
		/**data_image_ptr = (long*)realloc(data_image_ptr, (*DC + 1) * sizeof(long));*/
		/*TODO: checkand print error*/
		data_image_ptr[*DC] = num;
		begin++;
		(*DC)++;

		while (isspace(*begin)) { begin++; }
	}
}


/* Handles the data directive when begin points to the first argument */
void struct_handler(line_details line, char* begin, long *DC, long** data_image_ptr) {
	int num; /* Current integer being read */
	int ilen; /* Characters length of the integer */
	while (isspace(*begin)) { begin++; }
	/* Get first argument from the string input */
	if (sscanf(begin, "%d%n", &num, &ilen) <= 0) {
		printf_line_error(line, "expected number after .struct");
		return;
	}
	begin += ilen;

	while (isspace(*begin)) { begin++; }
	if (*begin != ',') {
		printf_line_error(line, "not an integer");
		return;
	}
	/* Insert the data into data_image*/
	/**data_image_ptr = (long*)realloc(data_image_ptr, (*DC + 1) * sizeof(long));*/
	/*TODO: checkand print error*/
	data_image_ptr[*DC] = num;
	(*DC)++;


	/* For each argument the the .data directive, add it to data_image */
	if (*begin == ',') {
		begin++;
		while (isspace(*begin)) { begin++; }
		string_handler(line, begin, DC, data_image_ptr);
	}
}

bool is_directive(char* line) {
	bool is_directive = true;
	while (isspace(*line)) { line++; }
	if (*line++ == '.') {
		char* copied_directive;
		char *delim = " ";
		/*char* copied_line = (char*)malloc(80);*/
		char copied_line[80];
		strcpy(copied_line, line);
		copied_directive = strtok(copied_line, delim);
		if (copied_directive) {
			if ((!strcmp(copied_directive, "data")) || (!strcmp(copied_directive, "string")) || (!strcmp(copied_directive, "struct")) || (!strcmp(copied_directive, "extern")) || (!strcmp(copied_directive, "entry"))) {
				is_directive = true;
				return is_directive;
			}
			else {
				is_directive = false;
				return is_directive;
			}
		}
	}
	is_directive = false;
	return is_directive;
}

