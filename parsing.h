/*************************************************************************

    File:           parsing.h

    Written By:     Ronen Hanukayev
    ID:             322881327

    Description:    This header file declares the functions that are used
                    to parse the code.     

*************************************************************************/
#ifndef PARSING_H_
#define PARSING_H_

#include "support.h"
#include "instructions.h"

bool isLegal(char* line);
bool isLabeled(char* line);
char* getLabel(char* line);
bool isDataStatement(char* line);
bool isStringStatement(char* line);
bool isExternStatement(char* line);
bool isEntryStatement(char* line);
char* getExternEntryLabel(char* line);
bool isValidInstruction(char*, InstructionGroup*);
char* getInstructionName(char*);
char* getOperand(int, char*);
bool isRegister(char*);
bool isValidRegister(char*);
AddressingMode getOperandsAddressingMode(char*);
bool isValidLabel(char*);
int getRegisterNumber(char*);
#endif
