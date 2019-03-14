/*


*/

/*TODO: get rid of addresses in entries, thay are unnecessary */

#ifndef DATASEG_H_
#define DATASEG_H_
struct DataSegmentEntry { 
    int data;/* a memory word is 12 bits, meaning we can use an int to store the data */
    struct DataSegmentEntry* next;
};

typedef struct DataSegmentEntry DataSegmentEntry;


typedef struct { 
        DataSegmentEntry* first;
} DataSegment;

DataSegmentEntry* getLastDataEntry(DataSegment*);
void appendData(DataSegment*, int);
int getDataSegSize(DataSegment*);

#endif
