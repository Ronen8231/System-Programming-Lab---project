#include <stdlib.h>
#include "dataseg.h"
#include <stdio.h>



DataSegmentEntry* getLastDataEntry(DataSegment* dataseg){
    DataSegmentEntry* current;
    if(dataseg->first == NULL)
        return NULL;
    for(current = dataseg->first; current->next; current = current->next);
    return current;
}



void appendData(DataSegment* dataseg, int address, int data){
    DataSegmentEntry* new_entry = (DataSegmentEntry*)malloc(sizeof(DataSegmentEntry));
    new_entry->address = address;
    new_entry->data = data;
    new_entry->next = NULL;
    if(getLastDataEntry(dataseg) == NULL)
        dataseg->first = new_entry;
    else
        getLastDataEntry(dataseg)->next = new_entry;
}
