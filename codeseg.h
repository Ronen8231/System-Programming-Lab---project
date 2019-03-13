
#ifndef CODESEG_H_
#define CODESEG_H_

struct CodeSegEntry{
    int encoded_code;
    struct CodeSegEntry* next;
};

typedef struct CodeSegEntry CodeSegEntry;

typedef struct{
    CodeSegEntry* first;
} CodeSegment;

CodeSegEntry* getLastCodeEntry(CodeSegment*);
void appendCode(CodeSegment*, int);

#endif
