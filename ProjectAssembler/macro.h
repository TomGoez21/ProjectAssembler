#ifndef MACRO_H_
#define MACRO_H_
#define MACRO "macro"
#define END_MACRO "endmacro"
#define MAX_LINE_LENGTH 82
#define FALSE 0
#define TRUE 1

typedef struct macro_struct {
    char key[MAX_LINE_LENGTH];
    char* val;
}macro;

typedef struct macro_list_struct {
    macro* data;
    int size;
    struct macro_list_struct* next;
}macroList;


/*
 * Function:  createNewMacro
 * --------------------
 * creates a new instance of macro, using malloc
 *
 * INPUT: char* key char* val -new macro values
 * RETURNS: pointer macro* to the new instance
 */
macro* createNewMacro(char* key, char* val);


/*
 * Function:  createNewMacroList
 * --------------------
 * creates a new instance of macroList
 * RETURNS: pointer macroList* to the new instance
 */
macroList* createNewMacroList();

/*
 * Function:  Sempty
 * --------------------
 * check id the list conatins macros
 * RETURNS: TRUE(1) if emepty
 */
int Sempty(macroList* temp);

/*
 * Function:  AddToMacroList
 * --------------------
 * INPUT: macro* macro to add to the end  of the list;   macroList** the mcroList to addded to
 */
void AddToMacroList(macro*, macroList**);


/*
 * Function:  PrintList
 * --------------------
 * print all of the macros in the lis
 */
void PrintList(macroList**);

/*
 * Function:  getFromMacroList
 * --------------------
 * iterate over the list and return the first macro that "word" mach to macro->key
 *
 * input: char* word as the key ,  macroList ** list
 * return: pointer tho the macro
 */
macro* getFromMacroList(char* word, macroList** list);

/*
 * Function:  isEndMacroLabel
 * --------------------
 * check if the string equls to "endmacro"
 *
 * input: char* word
 * return:  1 if the word "endmacro" else returns 0
 */
int isEndMacroLabel(char* word);


/*
 * Function:  isMacroLabel
 * --------------------
 * check if the string equls to "macro"
 *
 * input: char* word
 * return:  1 if the word "macro" else returns 0
 */
int isMacroLabel(char* currWord);

/*
 * Function:  checkIfMacroInList
 * --------------------
 * iterate over the list and return the first macro that "word" mach to macro->key
 *
 * input: char* word as the key ,  macroList ** list
 * return: pointer tho the macro
 */
int checkIfMacroInList(char* word, macroList** list);
void freeMacroList(macroList* head);
#endif 