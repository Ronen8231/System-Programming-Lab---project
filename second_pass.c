#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "second_pass.h"
#include "errors.h"
#include "codeseg.h"
#include "instructions.h"
#include "memory_words.h"
#include "parsing.h"

void second_pass(NamedFile* code_file, SymbolTable* symbol_table, InstructionGroup* instructions){
    char* entrySymbol;
    Symbol* curr_symbol;
    int i;
    bool symbolFound = false;
    char* line;
    char* inst_name;
    int ic = 100;
    int line_number = 0;
    int amount_of_operands;
    CodeSegment* codeseg = (CodeSegment*)malloc(sizeof(CodeSegment));
    InstructionWord* inst_word;
    RegisterAddressingWord reg_word;
    codeseg->first = NULL;
    while(strcmp((line = read_line(code_file->file)), EOF_REACHED) != 0){
        line_number++;

        if(isDataStatement(line) || isStringStatement(line) || isExternStatement(line)) /* took care of those at the first pass */
            continue;
        if(isEntryStatement(line)){
            symbolFound = false;
            entrySymbol = getExternEntryLabel(line); /* getting the symbol name */
            /* searching to see if the symbol exists */
            for(i = 0; i < symbol_table->current_size; i++){
                curr_symbol = symbol_table->table[i];
                if(strcmp(curr_symbol->name, entrySymbol) == 0){
                    curr_symbol->is_entry = true;
                    symbolFound = true;
                }
            }
            if(!symbolFound) 
                error(UNKNOWN_SYMBOL, code_file->name, line_number);
        }
        else{
            /* TODO: encode based on opcodes, addressing codes etc */
            inst_name = getInstructionName(line);
            amount_of_operands = getAmountOfOperands(inst_name, instructions);
            inst_word = (InstructionWord*) malloc(sizeof(InstructionWord));
            inst_word->opcode = getInstruction(inst_name, instructions)->opcode;
            inst_word->ARE = Absolute; /* the first word of the instruction is always absolute */
    
            inst_word->first_addressing_mode = None;
            inst_word->second_addressing_mode = None;

            if(amount_of_operands == 1){
                inst_word->second_addressing_mode = getOperandsAddressingMode(getOperand(1, line));
            }
            if(amount_of_operands == 2){
                inst_word->first_addressing_mode = getOperandsAddressingMode(getOperand(1, line));
                inst_word->second_addressing_mode = getOperandsAddressingMode(getOperand(2, line));
            }
            printf("%d\n", instructionWordToInt(inst_word));
            break;
            appendCode(codeseg, instructionWordToInt(inst_word));
        }
    }
}
