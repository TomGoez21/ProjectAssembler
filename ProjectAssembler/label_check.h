#ifndef LABEL_CHECK_H_
#define LABEL_CHECK_H_

#define MAX_LABEL_LENGTH 30

char* get_order(line_details line);
bool is_order(line_details line);
bool is_reserved_word(char* text);
bool is_label_valid(char* text);
char* get_label(char* line);

#endif /* LABEL_CHECK_H_ */
