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


/* structs to use when building the .ext file */

/* a row of the ext file */
struct ExternFileRow{
    int value;
    char* name;
    struct ExternFileRow* next;
};

typedef struct ExternFileRow ExternFileRow;

/* a linked list of ext file rows */
typedef struct{
    ExternFileRow* first;
} ExternFileList;




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

/* appending a direct or immediate word to the codesegment */
void appendNonRegisterWordToCodeSeg(CodeSegment* codeseg, char* data, SymbolTable* symbol_table, AddressingMode mode){
    int i;
    int mask = 0;
    int number;
    int final = 0; /* final result to be appended; bits 0 to 1 will be the A,R,E field, and bits 2 to 11 will be the number; */

    if(mode == Immediate){
        final = Absolute;
        number = atoi(data);
    }
    else {  /* mode == Direct */
        if(isExternal(data, symbol_table)) final = External;
        else final = Relocateable;
        number = getValue(data, symbol_table); 
    }
    number <<= 2;
    
    /* creating a mask in order to get rid of the unnecessary bits in number */
    for(i = 0; i < WORD_SIZE; i++){
        mask <<= 1;
        mask +=1;
    }

    final += (number & mask);
    appendCode(codeseg, final);
}


/* appending a register word to the code segment */
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


/* handling entry statements */
bool handleEntryStatement(char* line, SymbolTable* symbol_table){
    char* label = getExternEntryLabel(line);
    Symbol* symbol = getSymbol(label, symbol_table);

    if(symbol == NULL) /* wasn't found */        
        return false;

    symbol->is_entry = true;
    return true;
}

/* changing the addressing modes of an instruction words based on the operands */
void changeAddressingModes(char* line, int amount_of_operands, InstructionWord* inst_word){
    char *op1;
    char* op2;
    if(amount_of_operands == 0)
        return;
    
    op1 = getOperand(1, line);
    
    if(amount_of_operands == 1)
        inst_word->second_addressing_mode = getOperandsAddressingMode(op1);
    
    else if(amount_of_operands == 2){
        op2 = getOperand(2, line);    
        inst_word->first_addressing_mode = getOperandsAddressingMode(op1);
        inst_word->second_addressing_mode = getOperandsAddressingMode(op2);
        free(op2);
    }

    free(op1);
}

/* appending the extra words for each instructions(the words that are used to determine the operands) */
void appendAdditionalWordsToCodeSeg(char* line, InstructionGroup* instructions, CodeSegment* codeseg, SymbolTable* symbol_table, int* ic){
    int size = calculateInstructionSize(line, instructions);
    char* name = getInstructionName(line);
    int amount_of_operands = getAmountOfOperands(name, instructions);
    char* op1;
    char* op2;
    AddressingMode mode;
    if(size == 1)
        return;
    op1 = getOperand(1, line);    
    mode = getOperandsAddressingMode(op1);
    if(size == 2){
       if(mode == Register){
            if(amount_of_operands == 2){
                op2 = getOperand(2, line);
                appendRegisterWordToCodeSeg(codeseg, 2, op1, op2);
                free(op2);
            }
            else appendRegisterWordToCodeSeg(codeseg, 1, NULL, op1);
        }
        else
            appendNonRegisterWordToCodeSeg(codeseg, op1, symbol_table, mode); 
        (*ic)++;
    }
    if(size == 3){
        /* First Operand */
        if(mode == Register)
            appendRegisterWordToCodeSeg(codeseg, 1, op1, NULL);
        else
            appendNonRegisterWordToCodeSeg(codeseg, op1, symbol_table, mode);
        op2 = getOperand(2, line);
        mode = getOperandsAddressingMode(op2);
        if(mode == Register)
            appendRegisterWordToCodeSeg(codeseg, 1, NULL, op2);
        else
            appendNonRegisterWordToCodeSeg(codeseg, op2, symbol_table, mode);
        free(op2);
        (*ic) += 2;
    }
    free(op1);
    free(name);
}


/*handling an instruction */
void handleInstruction(char* line, InstructionGroup* instructions, CodeSegment* codeseg, SymbolTable* symbol_table, int* ic){
    char* name = getInstructionName(line);
    InstructionWord* inst_word = createInstructionWord(None, getInstruction(name, instructions)->opcode, None, Absolute);
    changeAddressingModes(line, getAmountOfOperands(name, instructions), inst_word);
    appendCode(codeseg, instructionWordToInt(inst_word));
    (*ic)++;
    appendAdditionalWordsToCodeSeg(line, instructions, codeseg, symbol_table, ic);
    free(name);
    free(inst_word);
}


/* adding a row to the ext rows list */
void addExt(char* name, int value, ExternFileRow* last_row){
    last_row->name = (char*)malloc(strlen(name) + 1);
    strcpy(last_row->name, name);
    last_row->value = value;
    last_row->next = (ExternFileRow*)malloc(sizeof(ExternFileRow));
    last_row = last_row->next;
    last_row->name = NULL;
}

void createExtFile(NamedFile* code_file, ExternFileList* list){
    char* name = (char*)malloc(strlen(code_file->name) + 5);
    FILE* file;
    ExternFileRow* current = list->first;
    strcpy(name, code_file->name);
    file = fopen(strcat(name, ".ext"), "w");
    while(current && current->name != NULL){
        fwrite(current->name, 1, strlen(current->name), file);
        fwrite(" ", 1, 1, file);
        fwrite(itoa(current->value), 1, strlen(itoa(current->value)), file);
        fwrite("\n", 1, 1, file);
        current = current->next;
    }
}

void second_pass(NamedFile* code_file, SymbolTable* symbol_table, InstructionGroup* instructions, DataSegment* dataseg){
    bool symbolFound = false;
    char* line;
    int ic = 100;
    int line_number = 0;
    char* operand;
    char* name;
    bool entries_exist = false;
    int no_of_operands;
    ExternFileList* externs = (ExternFileList*)malloc(sizeof(ExternFileList));
    ExternFileRow* last_row;
    bool found_errors = false;
    CodeSegment* codeseg = (CodeSegment*)malloc(sizeof(CodeSegment));
    codeseg->first = NULL;
    externs->first = (ExternFileRow*)malloc(sizeof(ExternFileRow));
    last_row = externs->first;
    last_row->name = NULL;
    while(strcmp((line = read_line(code_file->file)), EOF_REACHED) != 0){

        line_number++;

        if(isEmptyLine(line) || line[0] == ';')
            continue;

        if(isDataStatement(line) || isStringStatement(line) || isExternStatement(line)) /* took care of those at the first pass */
            continue;

        if(isEntryStatement(line)){

            symbolFound = handleEntryStatement(line, symbol_table);
    
            if(!symbolFound) {
                error(UNKNOWN_SYMBOL, code_file->name, line_number);
                found_errors = true;
                continue;
            }
            entries_exist = true;
        }
        else{
            name = getInstructionName(line);
            no_of_operands = getAmountOfOperands(name, instructions);
            if(no_of_operands >= 1){
                operand = getOperand(1, line);
                if(getOperandsAddressingMode(operand) == Direct){
                    if(getSymbol(operand, symbol_table) == NULL){
                        error(UNKNOWN_SYMBOL, code_file->name, line_number);
                        found_errors = true;
                        free(operand);
                        continue;                        
                    }                
                    else if(isExternal(operand, symbol_table)){
                    addExt(operand, ic + 1, last_row);
                    last_row = last_row->next;
                    } 
                }
                free(operand);
                if(no_of_operands == 2){
                    operand = getOperand(2, line);
                    if(getOperandsAddressingMode(operand) == Direct){
                        if(getSymbol(operand, symbol_table) == NULL){
                            error(UNKNOWN_SYMBOL, code_file->name, line_number);
                            found_errors = true;
                            free(operand);
                            continue;
                        }
                        else if(isExternal(operand, symbol_table)){
                        addExt(operand, ic + 2, last_row);
                        last_row = last_row->next;
                        }
                    }
                    free(operand);
                }
            }
            free(name);
            handleInstruction(line, instructions, codeseg, symbol_table, &ic);
        }
    }
    if(!found_errors){   
        createObFile(code_file, codeseg, dataseg, ic - 100);
        if(entries_exist) createEntFile(code_file, symbol_table);
        if(externs->first->name != NULL) createExtFile(code_file, externs);
    }
}
