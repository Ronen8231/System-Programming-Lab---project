

#ifndef DATASEG_H_
#define DATASEG_H_


/* an entry of the data segment */
struct DataSegmentEntry { 
    int data;/* a memory word is 12 bits, meaning we can use an int to store the data */
    struct DataSegmentEntry* next;
};

typedef struct DataSegmentEntry DataSegmentEntry;

/* the data segment, implemented with linked list */
typedef struct { 
        DataSegmentEntry* first;
} DataSegment;

DataSegmentEntry* getLastDataEntry(DataSegment*);
void appendData(DataSegment*, int);
int getDataSegSize(DataSegment*);

#endif
