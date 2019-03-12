/*


*/
#ifndef DATASEG_H_
#define DATASEG_H_
struct DataSegmentEntry { 
    int address;
    int data;/* a memory word is 12 bits, meaning we can use an int to store the data */
    struct DataSegmentEntry* next;
};

typedef struct DataSegmentEntry DataSegmentEntry;


typedef struct { 
        DataSegmentEntry* first;
} DataSegment;

DataSegmentEntry* getLastEntry(DataSegment*);
void appendData(DataSegment*, int, int);
#endif