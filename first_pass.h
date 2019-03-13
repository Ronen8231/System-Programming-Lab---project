#include <stdio.h>
#include "symbols.h"
#include "dataseg.h"
#include "instructions.h"

#ifndef  FIRSTPASS_H_
#define FIRSTPASS_H_

void first_pass(NamedFile*, SymbolTable*, DataSegment*, InstructionGroup*);
int calculateInstructionSize(char*, InstructionGroup*);

#endif
