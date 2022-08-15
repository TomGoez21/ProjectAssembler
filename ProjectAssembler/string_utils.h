
#ifndef STRING_UTIlS_H_
#define STRING_UTIlS_H_
#include "utils.h"
#define LINE_LENGTH	82

/*
 * Function:  removeLeadingWhiteSpaces
 * --------------------
 * iterate over the string and move the poninter forward as long as whitespace found
 *
 * INPUT: char* line (For exmp: " \t this string should be take care")
 * RETURNS: pointer char* to string without a leading whitespaces: ("this string should be take care")
 */
char* removeLeadingWhiteSpaces(char* line);


/*
 * Function:  getFirstWord
 * --------------------
 * iterate over the string and reaplcae the first whitespcae with '\0'

 * INPUT: char* line (For exmp: "this string should be take care")
 * RETURNS: pointer char* to string oif the first word ("this")
 */
char* getFirstWord(char* line);


/*
 * Function:  getFirstWordFromALine
 * --------------------
 * INPUT: char* line (For exmp: " \t this string should be take care")
 * RETURNS: pointer char* to string without a leading whitespaces: ("this")
 */
char* getFirstWordFromALine(char* line, char* currWord);


/*
 * Function:  removeEndWhiteSpaces
 * --------------------
 * INPUT: char* line (For exmp: " \t this \n")
 * RETURNS: pointer char* to string without a leading whitespaces: (" \t this")
 */
char* removeEndWhiteSpaces(char* line);

/*
 * Function:  isCommnetLine
 * --------------------
 * first getFirstWordFromALine than check if the word start with ';' if yes return true(1) else false(0).
 *
 * INPUT: char* line (For exmp: " ;this string should be take care")
 * RETURNS: pointer char* to string without a leading whitespaces: (TRUE (1))
 */
int isCommnetLine(char* line);


/*
 * Function:  isEmetyLine
 * --------------------
 * iterate over the line if only whitespace returns true.
 *
 * INPUT: char* line (For exmp: " \t \n    ")
 * RETURNS: pointer char* to string without a leading whitespaces: (TRUE (1))
 */
int isEmetyLine(char* line);


/*
 * Function:  getLeadingWhiteSpace
 * --------------------
 * iterate over the line and returns the leading whitespaces.
 *
 * INPUT: char* line (For exmp: " \t \n    balblabl")
 * RETURNS: pointer char* to string of whitespaces: " \t \n    ";
 */
char* getLeadingWhiteSpace(char* line);

/*
 * Function:  concatWhiteSpacesPerEachLine
 * --------------------
 * concat each line with giving whitespaces string.
 *
 * INPUT: char* line (For exmp: "thie is the first line \nthis is the second line") whitepace:(" \t ")
 * RETURNS: pointer char* to string : " \t thie is the first line \n
 *										\t this is the second line";
 */
char* concatWhiteSpacesPerEachLine(char* str, char* whiteSpace);

/*
 * Function:  countLines
 * --------------------
 * count how many lines in the string "\n"
 *
 * INPUT: char* line (For exmp: "thie is the first line \nthis is the second line")
 * RETURNS: int :  2;
 */
int countLines(char* str);

#endif