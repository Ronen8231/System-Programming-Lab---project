#include <stdlib.h>
#include "dataseg.h"
#include <stdio.h>

int getDataSegSize(DataSegment* dataseg){
    int size = 0;
    DataSegmentEntry* entry = dataseg->first;
    while(entry){
        size++;
        entry = entry->next;
    }
    return size;
}

DataSegmentEntry* getLastDataEntry(DataSegment* dataseg){
    DataSegmentEntry* current;
    if(dataseg->first == NULL)
        return NULL;
    for(current = dataseg->first; current->next; current = current->next);
    return current;
}



void appendData(DataSegment* dataseg, int data){
    DataSegmentEntry* new_entry = (DataSegmentEntry*)malloc(sizeof(DataSegmentEntry));
    new_entry->data = data;
    new_entry->next = NULL;
    if(getLastDataEntry(dataseg) == NULL)
        dataseg->first = new_entry;
    else
        getLastDataEntry(dataseg)->next = new_entry;
}
