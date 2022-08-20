#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "directives.h"
#include "utils.h"


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
	int space_index;
	directive d;
	while (isspace(*begin)) { begin++; }
	if (*begin++ != '.') {
		printf_line_error(line, "directive should start with period");
		set_error(true);
		return _invalid;
	}

	space_index = strcspn(begin, " ");
	strncpy(directive_string, begin, space_index);

	d = str_to_directive(directive_string);
	if (d == _invalid) {
		printf_line_error(line, "invalid directive %s", directive_string);
		set_error(true);
	}
	return d;

}


/*Handles string directive, 'begin' points to the first character*/
void string_handler(line_details line, char* begin, long *DC, long** data_image_ptr) {
	char* last_quot;
	while (isspace(*begin)) { begin++; }
	/* Checks if the string argument has leading quotation marks */
	if (*begin++ != '"') {
		printf_line_error(line, "String directive should start with quotation marks");
		set_error(true);
		return;
	}
	last_quot = strrchr(begin, '"');

	if (!last_quot) {
		printf_line_error(line, "String %s should end with quotation marks", begin);
		set_error(true);
		return;
	}

	/*insert data to the data list*/
	while (*begin && *begin != '"') {
		data_image_ptr[*DC] = (long*)*begin;
		begin++;
		(*DC)++;
	}

	/* add \0 to the end of string in data image */
	data_image_ptr[*DC] = '\0';
	(*DC)++;

	if (*begin == '"') {
		begin++;
	}
	while (isspace(*begin)) { begin++; }
	/* checks for extra chracters after string directive */
	if (*begin) {
		printf_line_error(line, "Extra characters after end of string directive: %s", begin);
		set_error(true);
	}

}

/* Handles the data directive when begin points to the first argument */
void data_handler(line_details line, char* begin, long *DC, long** data_image_ptr) {
	long num; /* Current integer being read */
	int ilen; /* Characters length of the integer */
	while (isspace(*begin)) { begin++; }

	/* Get first argument from the string input */
	if (sscanf(begin, "%ld%n", &num, &ilen) <= 0) {
		printf_line_error(line, "expected number after .data");
		set_error(true);
		return;
	}
	begin += ilen;
	while (isspace(*begin)) { begin++; }

	if (*begin != ',' && *begin) {
		printf_line_error(line, "not an integer");
		set_error(true);
		return;
	}
	/* Insert the data into data_image */
	data_image_ptr[*DC] = (long*)num;
	(*DC)++;

	/* for each argument the the .data directive, add it to data_image */
	while (*begin == ',') {
		begin++;

		while (isspace(*begin)) { begin++; }

		/* get argument */
		if (sscanf(begin, "%ld%n", &num, &ilen) <= 0) {
			printf_line_error(line, "Expected a number after comma");
			set_error(true);
			return;
		}

		begin += ilen;

		while (isspace(*begin)) { begin++; }

		if (*begin != ',' && *begin != 0) {
			printf_line_error(line, "not an integer in data content");
			set_error(true);
			return;
		}

		/* Insert the data into data_image */
		data_image_ptr[*DC] = (long*)num;
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
		set_error(true);
		return;
	}
	begin += ilen;

	while (isspace(*begin)) { begin++; }
	if (*begin != ',') {
		printf_line_error(line, "not an integer in struct directive");
		set_error(true);
		return;
	}
	/* Insert the data into data_image*/
	data_image_ptr[*DC] = (long*)num;
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
		/*char* copied_line = (char*)malloc(82);*/
		char copied_line[MAX_LINE_LENGTH];
		strcpy(copied_line, line);
		copied_directive = strtok(copied_line, delim);
		if (copied_directive) {
			if ((!strcmp(copied_directive, "data")) || (!strcmp(copied_directive, "string")) || (!strcmp(copied_directive, "struct")) || (!strcmp(copied_directive, "extern")) || (!strcmp(copied_directive, "entry"))) {
				is_directive = true;
				return is_directive;
			}
			else {
				is_directive = false;
				fprintf(stderr, "\"%s\" is invalid directive name", copied_directive);
				set_error(true);
				return is_directive;
			}
		}
	}
	is_directive = false;
	return is_directive;
}

