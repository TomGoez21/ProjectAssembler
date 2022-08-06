#ifndef LABEL_CHECK_H_
#define LABEL_CHECK_H_

#define MAX_LABEL_LENGTH 30

char* get_order(line_details line);
bool is_order(line_details line);
bool is_reserved_word(line_details line, char* text);
bool is_label_valid(line_details line, char* text);
bool is_label_valid_in_struct(line_details line, char* text);
bool is_label_valid_in_text(line_details line,char* text);
char* get_label(line_details line);
char* get_label_in_struct(char* text, char* label_name);
#endif /* LABEL_CHECK_H_ */
