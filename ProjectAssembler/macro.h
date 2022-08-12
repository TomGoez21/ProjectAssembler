#ifndef MACRO_H_
#define MACRO_H_
#define MACRO "macro"
#define END_MACRO "endmacro"
#define FALSE 0
#define TRUE 1

typedef struct macro_struct{
        char key[80];
        char* val;
    }macro;

typedef struct macro_list_struct{
    macro *data;
    int size;
    struct macro_list_struct *next;
}macroList;

macro* createNewMacro(char* key , char* val);
macroList* createNewMacroList();
int Sempty(macroList *temp);
void AddToMacroList(macro* , macroList **);
void clearPrintList(macroList **);
void PrintList(macroList ** );
macro* getFromMacroList(char* word , macroList ** list);
int isEndMacroLabel(char* word);
int isMacroLabel(char* currWord);
int checkIfMacroInList(char* word , macroList** list);
void freeMacroList(macroList * head);
#endif 