#include "instructions.h"
#include <stdlib.h>
#include <string.h>
#include "support.h"

Instruction* getInstruction(char* name, InstructionGroup* instructions){
    Instruction* current;
    int i;
    for(i = 0 ; i < NUM_OF_INSTRUCTIONS; i++){
        current = &(instructions->all_instructions[i]);
        if(strcmp(current->inst, name) == 0)
            return current;
    }
    return NULL;
}

Instruction createInstruction(char* name, int opcode, AddressingMode op1[3], AddressingMode op2[3]){
    int i;
    Instruction instruction;
    instruction.inst = (char*) malloc(sizeof(name) + 1);
    strcpy(instruction.inst, name);
    instruction.opcode = opcode;
    for(i = 0; i < 3; i++){
        instruction.operand1[i] = op1[i];
        instruction.operand2[i] = op2[i];
    }
    return instruction;
}

bool instructionExists(char* instruction, InstructionGroup* all_instructions){
    int i;
    Instruction current;
    for(i = 0; i < NUM_OF_INSTRUCTIONS; i++){
        current = all_instructions->all_instructions[i];
        if(strcmp(current.inst, instruction) == 0)
            return true;
    }
    return false;
}

InstructionGroup* allInstructions(){
    AddressingMode op1[3];
    AddressingMode op2[3];
    InstructionGroup *instructions = (InstructionGroup*)malloc(sizeof(InstructionGroup));
    instructions->all_instructions = (Instruction*)malloc(sizeof(Instruction) * NUM_OF_INSTRUCTIONS);

    /* ------mov-------*/
    op1[0] = Immediate; op1[1] = Direct; op1[2] = Register;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[0] = createInstruction("mov", 0, op1, op2); 

    /* ------cmp-------*/
    op1[0] = Immediate; op1[1] = Direct; op1[2] = Register;
    op2[0] = Immediate; op2[1] = Direct; op2[2] = Register;
    instructions->all_instructions[1] = createInstruction("cmp", 1, op1, op2); 
    
    /* ------add-------*/
    op1[0] = Immediate; op1[1] = Direct; op1[2] = Register;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[2] = createInstruction("add", 2, op1, op2); 
    
    /* ------sub-------*/
    op1[0] = Immediate; op1[1] = Direct; op1[2] = Register;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[3] = createInstruction("sub", 3, op1, op2); 
   
    /* ------not-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[4] = createInstruction("not", 4, op1, op2); 
   
    /* ------clr-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[5] = createInstruction("clr", 5, op1, op2); 
   
    /* ------lea-------*/
    op1[0] = Direct; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[6] = createInstruction("lea", 6, op1, op2); 
   
    /* ------inc-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[7] = createInstruction("inc", 7, op1, op2); 
   
    /* ------dec-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[8] = createInstruction("dec", 8, op1, op2); 
   
    /* ------jmp-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[9] = createInstruction("jmp", 9, op1, op2); 
   
    /* ------bne-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[10] = createInstruction("bne", 10, op1, op2); 
   
    /* ------red-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[11] = createInstruction("red", 11, op1, op2); 
   
    /* ------prn-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Immediate; op2[1] = Direct; op2[2] = Register;
    instructions->all_instructions[12] = createInstruction("prn", 12, op1, op2); 
   
    /* ------jsr-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = Direct; op2[1] = Register; op2[2] = None;
    instructions->all_instructions[13] = createInstruction("jsr", 13, op1, op2); 
   
    /* ------rts-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = None; op2[1] = None; op2[2] = None;
    instructions->all_instructions[14] = createInstruction("rts", 14, op1, op2); 
   
    /* ------stop-------*/
    op1[0] = None; op1[1] = None; op1[2] = None;
    op2[0] = None; op2[1] = None; op2[2] = None;
    instructions->all_instructions[15] = createInstruction("stop", 15, op1, op2); 
   

    return instructions;
}
