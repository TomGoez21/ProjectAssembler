#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"

#pragma warning(disable : 4996)

macro* createNewMacro(char* key , char* val){
    macro * new;
    new = (macro*) malloc(sizeof(macro));
    strcpy(new -> key, key);
    new -> val = val;
    return (new);
}


macroList* createNewMacroList(){
    macroList * new;
    new = (macroList*) malloc(sizeof(macroList));
    new -> size = 0;
    return (new);
}

int Sempty(macroList *temp) {
   if (temp == NULL)
      return 1;
   else
      return 0;
}

void AddToMacroList(macro* m , macroList** list){
    macroList * new;
    printf("key2 -> %s \n", m -> key);
    new = (macroList*) malloc(sizeof(macroList));
    new -> data = m;
    new -> next = *list;
    new -> size = ((*list)->size) + 1;
    *list = new;
}
int checkIfMacroInList(char* word , macroList** list){
    macroList *temp;
    macro *curr;
    temp = *list;
    printf("list size - %d \n" , (*list)-> size);
    if (Sempty(temp)){
        printf("\nThe stack is empty!");
        return FALSE;
    }
    else {
      while (temp->size > 0) {
        curr = temp->data;
        if(!strcmp((curr->key) , word)){
            printf(" key :%s , word: %s\n", curr->key, word);
            return TRUE;     
        }
        temp = temp->next;
      }
      return FALSE;
    }
}
void PrintList(macroList ** list){
    macroList *temp;
    macro *curr;
    temp = *list;
    printf("list size - %d \n" , (*list)-> size);
    if (Sempty(temp))
        printf("\nThe stack is empty!");
    else {
      while (temp->size > 0) {
        curr = temp->data;
        printf(" key :%s\n", curr->key);
        printf(" val :%s\n", curr->val);
        temp = temp->next;
      }
   }
}
char* getFromMacroList(char* word , macroList ** list){
    macroList *temp;
    macro *curr;
    temp = *list;
    printf("list size - %d \n" , (*list)-> size);
    if (Sempty(temp))
        printf("\nThe stack is empty!");
    else {
      while (temp->size > 0) {
        curr = temp->data;
        if(!strcmp((curr->key) , word)){
            printf(" key :%s , word: %s\n", curr->key, word);
            return curr->val;     
        }
        temp = temp->next;
      }
   }
   return NULL;
}

int isEndMacroLabel(char* word){
    if(!strcmp(word ,END_MACRO)){
        return TRUE;
    }else 
        return FALSE;
}

int isMacroLabel(char* word){
    if(!strcmp(word ,MACRO)){
        return TRUE;
    }else{ 
        return FALSE;
    }
}

void freeMacroList(macroList * head){
    macroList *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->data);
        free(tmp);
    }
}
