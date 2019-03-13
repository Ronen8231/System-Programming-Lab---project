#include "codeseg.h"
#include <stdlib.h>


CodeSegEntry* getLastCodeEntry(CodeSegment* codeseg){
    CodeSegEntry* current = codeseg->first;
    if(current == NULL)
        return NULL;
    while(current->next != NULL)
        current = current->next;
    return current;
}

void appendCode(CodeSegment* codeseg, int encoded_code){
    CodeSegEntry* last = getLastCodeEntry(codeseg);
    CodeSegEntry* new_entry = (CodeSegEntry*)malloc(sizeof(CodeSegEntry));
    new_entry->encoded_code = encoded_code;
    new_entry->next = NULL; 
    if(last == NULL)
        codeseg->first = new_entry;
    else
        last->next = new_entry;
}
