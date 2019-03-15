

#ifndef MEMORYWORDS_H_
#define MEMORYWORDS_H_

#define WORD_SIZE 12 /* size of a word in bits */
#include "instructions.h"


typedef enum{Absolute = 0, External = 1, Relocateable = 2} ARE; /* enumeration of the A,R,E fiels */

/* an instruction word */
typedef struct{
    unsigned int first_addressing_mode: 3;
    unsigned int opcode: 4;
    unsigned int second_addressing_mode: 3;
    unsigned int ARE: 2;
} InstructionWord;

/* register addressing word */
typedef struct{
    unsigned int src_reg: 5;
    unsigned int dest_reg: 5;
    unsigned int ARE: 2;
} RegisterAddressingWord;

int registerWordToInt(RegisterAddressingWord* word);
int instructionWordToInt(InstructionWord* word);
InstructionWord* createInstructionWord(AddressingMode, int, AddressingMode, ARE);

#endif
