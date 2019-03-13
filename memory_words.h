

#ifndef MEMORYWORDS_H_
#define MEMORYWORDS_H_

typedef enum{Absolute = 0, Relocateable = 1, External = 2} ARE; /* enumeration of the A,R,E fiels */

typedef struct{
    int first_addressing_mode: 3;
    int opcode: 4;
    int second_addressing_mode: 3;
    int ARE: 2;
} InstructionWord;


typedef struct{
    int src_reg: 5;
    int dest_reg: 5;
    int ARE: 2;
} RegisterAddressingWord;

int RegisterWordToInt(RegisterAddressingWord* word);
int instructionWordToInt(InstructionWord* word);

#endif
