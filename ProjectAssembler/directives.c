#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
directive find_directive_type(line_details line, char* begin) {
	char dir[10] = { 0 };
	while (isspace(*begin)) { begin++; }
	if (*begin++ != '.') {
		printf_line_error(line, "directive should start with period");
		return _invalid;
	}

	int space_index = strcspn(begin, " ");
	strncpy(dir, begin, space_index);

	directive d = str_to_directive(dir);
	if (d == _invalid) {
		printf_line_error(line, "invalid directive %s", dir);
	}
	return d;

}


/*Handles string directive, 'begin' points to the first character*/
void string_handler(line_details line, char* begin, int* DC, long* data_image) {
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
		data_image[*DC] = *begin;
		begin++;
		(*DC)++;
	}

	/* add \0 to the end of string in data image */
	data_image[*DC] = '\0';

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
void data_handler(struct line_details line, char* begin, int* DC, long* data_image) {
	int num; /* Current integer being read */
	int ilen; /* Characters length of the integer */

	/* Get first argument from the string input */
	if (sscanf(begin, "%d%n", &num, &ilen) <= 0) {
		printf_line_error(line, "expected number after .data");
		return;
	}
	begin += ilen;

	while (isspace(*begin)) { begin++; }
	if (*begin != ',') {
		printf_line_error(line, "not an integer");
		return;
	}
	/* Insert the data into data_image */
	data_image[*DC] = num;
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
		data_image[*DC] = num;
		begin++;
		(*DC)++;

		while (isspace(*begin)) { begin++; }
	}
}


/* Handles the data directive when begin points to the first argument */
void struct_handler(line_details line, char* begin, int* DC, long* data_image) {
	int num; /* Current integer being read */
	int ilen; /* Characters length of the integer */

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
	data_image[*DC] = num;
	(*DC)++;


	/* For each argument the the .data directive, add it to data_image */
	if (*begin == ',') {
		begin++;
		while (isspace(*begin)) { begin++; }
		string_handler(line, begin, DC, data_image);
	}
}
