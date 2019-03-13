
/* defining all the instructions in the language */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "support.h"

#define NUM_OF_INSTRUCTIONS 16
typedef enum{ None = 0, Immediate = 1, Direct = 3, Register = 5} AddressingMode;

typedef struct {
    char* inst; /* instruction name */
    int opcode;
    /* arrays containing the possible address modes for each operand 
        None if no more addressing modes are possible */
    AddressingMode operand1[3];
    AddressingMode operand2[3];
} Instruction;

typedef struct{
    Instruction* all_instructions;
} InstructionGroup;

InstructionGroup* allInstructions();

bool instructionExists(char*, InstructionGroup*);
Instruction* getInstruction(char* name, InstructionGroup*);
int getAmountOfOperands(char* name, InstructionGroup*);
#endif
