
#ifndef CODESEG_H_
#define CODESEG_H_

/* defines an entry of the codesegment */
struct CodeSegEntry{
    int encoded_code;
    struct CodeSegEntry* next;
};

typedef struct CodeSegEntry CodeSegEntry;


/* defines the code segment, implemented with linked list */
typedef struct{
    CodeSegEntry* first;
} CodeSegment;

CodeSegEntry* getLastCodeEntry(CodeSegment*);
void appendCode(CodeSegment*, int);

#endif
