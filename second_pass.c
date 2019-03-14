#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "first_pass.h"
#include "second_pass.h"
#include "errors.h"
#include "codeseg.h"
#include "instructions.h"
#include "memory_words.h"
#include "parsing.h"


void createEntFile(NamedFile* code_file, SymbolTable* symbol_table){
    Symbol* symbol;
    int i;
    FILE* file;
    char* name = (char*)malloc(strlen(code_file->name) + 1);
    char* value;
    strcpy(name, code_file->name);
    file = fopen(strcat(name, ".ent"), "w");
    for(i = 0; i < symbol_table->current_size; i++){
        symbol = symbol_table->table[i];
        if(symbol->is_entry){
            fwrite(symbol->name, 1, strlen(symbol->name), file);
            fwrite(" ", 1, 1, file); /* writing a space */
            value = itoa(symbol->value);
            fwrite(value, 1, strlen(value), file);
            fwrite("\n", 1, 1, file);
            free(value);
        }
    }
    free(name);
    fclose(file);
}

void createObFile(NamedFile* code_file, CodeSegment* codeseg, DataSegment* dataseg, int code_size){
    CodeSegEntry* current = codeseg->first;
    DataSegmentEntry* data = dataseg->first;
    int dataseg_size = getDataSegSize(dataseg);
    char* s;
    char* name = (char*)malloc(strlen(code_file->name) + 1);
    char* size = itoa(dataseg_size);
    FILE* file; 
    strcpy(name, code_file->name);
    file = fopen(strcat(name, ".ob"), "w");
    fwrite(strcat(itoa(code_size), " "), 1, strlen(itoa(code_size)) + 1, file);
    fwrite(strcat(size, "\n"), 1, strlen(size) + 1, file);   
    free(size); 
    while(current != NULL){
        s = wordToBase64(current->encoded_code);
        fwrite(s, 1, 2, file);
        fwrite("\n", 1, 1, file);        
        current = current->next;
        free(s);
    }


    while(data){
        s = wordToBase64(data->data);
        fwrite(s, 1, 2, file);
        fwrite("\n", 1, 1, file);
        data = data->next;
        free(s);
    }
    fclose(file);
}


void appendDirectWordToCodeSeg(CodeSegment* codeseg, char* symbol, SymbolTable* symbol_table){
    int i;
    int address = getValue(symbol, symbol_table);
    int mask = 0;
    int final = 0; /* final result to be appended; bits 0 to 1 will be the A,R,E field, and bits 2 to 11 will be the symbol's address */
    
    if(isExternal(symbol, symbol_table)) final = External;
    else final = Relocateable; /* no need for shifting */
    
    address <<= 2;

        
    /* creating a mask in order to get rid of the unnecessary bits in number */
    for(i = 0; i < WORD_SIZE; i++){
        mask <<= 1;
        mask +=1;
    }

    final += (address & mask);
    appendCode(codeseg, final);
}


void appendImmediateWordToCodeSeg(CodeSegment* codeseg, char* num){
    int i;
    int mask = 0;
    int number = atoi(num);
    int final = 0; /* final result to be appended; bits 0 to 1 will be the A,R,E field, and bits 2 to 11 will be the number; */
    final = Absolute; /* no need for shifting */
    number <<= 2;
    
    /* creating a mask in order to get rid of the unnecessary bits in number */
    for(i = 0; i < WORD_SIZE; i++){
        mask <<= 1;
        mask +=1;
    }

    final += (number & mask);
    appendCode(codeseg, final);
}

void appendRegisterWordToCodeSeg(CodeSegment* codeseg, int no_of_regs, char* reg1, char* reg2){
    RegisterAddressingWord* reg_word = (RegisterAddressingWord*)malloc(sizeof(RegisterAddressingWord));
    reg_word->ARE = Absolute;
    if(no_of_regs == 2){
        reg_word->src_reg = getRegisterNumber(reg1);
        reg_word->dest_reg = getRegisterNumber(reg2);
    }
    else{
         if(reg1 == NULL){
             reg_word->dest_reg = getRegisterNumber(reg2);
             reg_word->src_reg = 0; /* no source register */
         }
        else{ /* reg2 is NULL */
             reg_word->src_reg = getRegisterNumber(reg1);
             reg_word->dest_reg = 0; /* no dest register */  
        }
    }
    appendCode(codeseg, registerWordToInt(reg_word));
    free(reg_word);
}


void second_pass(NamedFile* code_file, SymbolTable* symbol_table, InstructionGroup* instructions, DataSegment* dataseg){
    char* entrySymbol;
    Symbol* curr_symbol;
    int i;
    bool symbolFound = false;
    char* line;
    char* inst_name;
    int ic = 100;
    int size_of_instruction;
    int line_number = 0;
    int amount_of_operands;
    char* op1;
    char* op2;
    bool found_errors = false;
    CodeSegment* codeseg = (CodeSegment*)malloc(sizeof(CodeSegment));
    InstructionWord* inst_word;
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
            if(!symbolFound) {
                error(UNKNOWN_SYMBOL, code_file->name, line_number);
                found_errors = true;
            }
        }
        else{
            inst_name = getInstructionName(line);
            amount_of_operands = getAmountOfOperands(inst_name, instructions);
            inst_word = (InstructionWord*) malloc(sizeof(InstructionWord));
            inst_word->opcode = getInstruction(inst_name, instructions)->opcode;
            free(inst_name);
            inst_word->ARE = Absolute; /* the first word of the instruction is always absolute */
    
            inst_word->first_addressing_mode = None;
            inst_word->second_addressing_mode = None;

            if(amount_of_operands == 1){
                op1 = getOperand(1,line);
                inst_word->second_addressing_mode = getOperandsAddressingMode(op1);
                free(op1);
            }
            if(amount_of_operands == 2){
                op1 = getOperand(1, line);
                op2 = getOperand(2, line);
                inst_word->first_addressing_mode = getOperandsAddressingMode(op1);
                inst_word->second_addressing_mode = getOperandsAddressingMode(op2);
                free(op1);  
                free(op2);
            }
            appendCode(codeseg, instructionWordToInt(inst_word));
            ic += 1;
            size_of_instruction = calculateInstructionSize(line, instructions);
            if(size_of_instruction == 1)
                continue;
            if(size_of_instruction == 2){
                op1 = getOperand(1, line);
                if(inst_word->second_addressing_mode == Register){
                    if(amount_of_operands == 2){
                        op2 = getOperand(2, line);
                        appendRegisterWordToCodeSeg(codeseg, amount_of_operands, op1, op2);
                        free(op2);
                    }
                    else{
                        appendRegisterWordToCodeSeg(codeseg, amount_of_operands, NULL, op1); /* one operand, op1 is destination operand */
                    }
                }

                if(inst_word->second_addressing_mode == Immediate){
                    appendImmediateWordToCodeSeg(codeseg, op1);
                }
                if(inst_word->second_addressing_mode == Direct){
                    appendDirectWordToCodeSeg(codeseg, op1, symbol_table);
                }
                free(op1); 
                ic += 1;
            }
    
            if(size_of_instruction == 3){
                op1 = getOperand(1, line);
                op2 = getOperand(2, line);
                
                if(inst_word->first_addressing_mode == Register){
                    appendRegisterWordToCodeSeg(codeseg, 1, op1, NULL);
                }
                else if(inst_word->first_addressing_mode == Immediate){
                    appendImmediateWordToCodeSeg(codeseg, op1);
                }
                else{ /* must be direct */
                    appendDirectWordToCodeSeg(codeseg, op1, symbol_table);
                }

                if(inst_word->second_addressing_mode == Register){
                    appendRegisterWordToCodeSeg(codeseg, 1, NULL, op2);
                }
                else if(inst_word->second_addressing_mode == Immediate){
                    appendImmediateWordToCodeSeg(codeseg, op2);
                }
                else{ /* must be direct */
                    appendDirectWordToCodeSeg(codeseg, op2, symbol_table);
                }
                
                free(op1); free(op2);
                ic += 2;
            }

            free(inst_word);
        }
    }
    if(!found_errors){   
        createObFile(code_file, codeseg, dataseg, ic - 100);
        createEntFile(code_file, symbol_table);
    }
}
