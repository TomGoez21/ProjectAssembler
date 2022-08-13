#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"

#pragma warning(disable : 4996)

macro* createNewMacro(char* key, char* val) {
    macro* new;
    char* tempVal;
    new = (macro*)malloc(sizeof(macro));
#pragma warning(suppress : 4996)
    strcpy(new->key, key);
    tempVal = new->val;
    tempVal =(char*) malloc(strlen(val)+81);
    tempVal[0] = '\0';
#pragma warning(suppress : 4996)
    strcpy(tempVal,val);
    new->val = tempVal;
    return (new);
}


macroList* createNewMacroList() {
    macroList* new;
    new = (macroList*)malloc(sizeof(macroList));
    new->size = 0;
    return (new);
}

int Sempty(macroList* temp) {
    if (temp == NULL)
        return 1;
    else
        return 0;
}

void AddToMacroList(macro* m, macroList** list) {
    macroList* new;
    new = (macroList*)malloc(sizeof(macroList));
    new->data = m;
    new->next = *list;
    new->size = ((*list)->size) + 1;
    *list = new;
}
int checkIfMacroInList(char* word, macroList** list) {
    macroList* temp;
    macro* curr;
    temp = *list;
    if (Sempty(temp)) {
        printf("\nThe stack is empty!");
        return FALSE;
    }
    else {
        while (temp->size > 0) {
            curr = temp->data;
            if (!strcmp((curr->key), word)) {
                return TRUE;
            }
            temp = temp->next;
        }
        return FALSE;
    }
}
void PrintList(macroList** list) {
    macroList* temp;
    macro* curr;
    temp = *list;
    if (Sempty(temp))
        printf("\nThe stack is empty!");
    else {
        while (temp->size > 0) {
            curr = temp->data;
            temp = temp->next;
        }
    }
}
macro* getFromMacroList(char* word, macroList** list) {
    macroList* temp;
    macro* curr;
    temp = *list;
    if (Sempty(temp))
        printf("\nThe stack is empty!");
    else {
        while (temp->size > 0) {
            curr = temp->data;
            if (!strcmp((curr->key), word)) {
                return curr;
            }
            temp = temp->next;
        }
    }
    return NULL;
}

int isEndMacroLabel(char* word) {
    if (!strcmp(word, END_MACRO)) {
        return TRUE;
    }
    else
        return FALSE;
}

int isMacroLabel(char* word) {
    if (!strcmp(word, MACRO)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void freeMacroList(macroList* head) {
    if (head == NULL) return;
    int size = head->size;
    macroList* tmp;
    macro* macroTmp;
    char* charTmp;
    for(;size>0 ; size--){
        tmp = head;
        head = head->next;
        macroTmp = tmp->data;
        charTmp = macroTmp->val;
        free(charTmp);
        free(macroTmp);
        free(tmp);
    }
    free(head);
}
