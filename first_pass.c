#include "support.h"
#include "dataseg.h"
#include <string.h>
#include <stdlib.h>
#include "symbols.h"
#include "parsing.h"
#include "instructions.h"
#include "errors.h"


/* adding data to the data segment */
void addDataToDataSeg(char* line, DataSegment* dataseg, int* dc){
    char* code_line = (char*)malloc(strlen(line) + 1);
    char* code;
    char* all_data;
    char* curr_data;
    strcpy(code_line, line);
    code = strstr(code_line, ".data");
    all_data = strtok(code, ".data");
    curr_data = strtok(all_data, ",");
    
    while(curr_data != NULL){
        appendData(dataseg, atoi(curr_data));
        (*dc)++;
        curr_data = strtok(NULL, ",");
    }
    
    free(code_line);
}

/* adding a string to the data segment */
void addStringToDataSeg(char* line, DataSegment* dataseg, int* dc){
    char* code_line = (char*) malloc(strlen(line) + 1);
    char* code;
    char* string;
    int i;
    strcpy(code_line, line);
    code = strstr(code_line, ".string");
    string = strtok(code, ".string");
    for(i = 0;string[i] != '\"' ;i++);
    i++;
    while(string[i] != '\"'){
        appendData(dataseg, string[i]);
        (*dc)++;
        i++;
    }
    appendData(dataseg, 0); /* null terminator */
    (*dc)++;
    free(code_line);
}

/* calculating the size of an instruction */
int calculateInstructionSize(char* line, InstructionGroup* instructions){
    char* instruction_name;
    char* op1;
    char* op2;
    int amount_of_operands = 0;
    int ret;
    instruction_name = getInstructionName(line);
    amount_of_operands = getAmountOfOperands(instruction_name, instructions);
    if(amount_of_operands == 0)
        return 1; /* only one word of data needed */
    if(amount_of_operands == 1)
        return 2; /* two words of data needed */ 
    op1 = getOperand(1, line);
    op2 = getOperand(2, line);
    if(amount_of_operands == 2){
        if(isRegister(op1) && isRegister(op2))
            ret =  2;
        else
            ret =  3;
        free(op1);
        free(op2);
        return ret;
    }
    
    return -1; /* impossible in valid code */
}

/* updating the data symbols value; should be called at the end of the first pass
    when we know the final size of the code segment */
void updateSymbolTable(SymbolTable* symbol_table, int start_address){
    int i, size;
    Symbol* current;
    size = symbol_table->current_size;
    for(i = 0; i < size; i++){
        current =  symbol_table->table[i];
        if( (! current->is_instruction) && (! current->is_external) )
            current->value += start_address;
    }
}


/* return true if symbol already in symbol table */
bool isSymbolRedefined(char* line, SymbolTable* symbol_table){
    char* label = getLabel(line);
    Symbol* symbol = getSymbol(label, symbol_table);
    free(label);
    return (symbol != NULL);
}


/* returns true if the addressing modes of the operands
    are legal for this instruction */
bool validateOperandsAddressingMode(char* instruction_name, InstructionGroup* instructions, char* op1, char* op2){
    int i;
    Instruction* instruction = getInstruction(instruction_name, instructions);
    AddressingMode mode;
    bool found_mode = false;
    if(op1!=NULL){
        mode = getOperandsAddressingMode(op1);
        /* i less than 3 because the addressing modes array has 3 elements */
        for(i = 0; i < 3; i++){
            if(instruction->operand1[i] == mode){
                found_mode = true;
                break;
            }
        }
        if(!found_mode)
            return false;
    }
    if(op2!=NULL){
         mode = getOperandsAddressingMode(op2);
        /* i less than 3 because the addressing modes array has 3 elements */
        for(i = 0; i < 3; i++){
            if(instruction->operand2[i] == mode){
                found_mode = true;
                break;
            }
        }
        if(!found_mode)
            return false;
    }
    return found_mode;
}


/* returns true if errors were detected */
bool first_pass(NamedFile* code_file, SymbolTable* symbol_table, DataSegment* dataseg, InstructionGroup* instructions){
    int ic, dc;
    int size;
    int line_number = 0;
    bool symbolDefined;
    bool errors_found = false;
    char* line;
    char* label;
    char* name;
    char* op1;
    char* op2;
    bool isValid;
    Symbol* curr_label;
    ic=100;
    dc=0;
    while( strcmp((line = read_line(code_file->file)), EOF_REACHED) != 0 ){
        line_number++;
        size = 0;



        if(isEmptyLine(line) || line[0] == ';') /* comment */
            continue;        
          
        symbolDefined = isLabeled(line);

        if(symbolDefined){
            if(isSymbolRedefined(line, symbol_table)){ /* symbol already exists */
                error(SYMBOL_REDEFINITION, code_file->name, line_number);
                errors_found = true;
                continue;
            }
            label = getLabel(line);
            if(!isValidLabel(label)){
                error(ILLEGAL_LABEL, code_file->name, line_number);
                errors_found = true;
                continue;
            }
        }
       
 
        if(isDataStatement(line) || isStringStatement(line)){
            if(symbolDefined){
                label = getLabel(line);
                /* not an instruction, not external */
                curr_label = createSymbol(label, dc, false, false, false);                 
                addSymbol(curr_label, symbol_table);
                free(label);
            }
            
            if(isDataStatement(line)){
                addDataToDataSeg(line, dataseg, &dc);
            }
           
            if(isStringStatement(line)){
                addStringToDataSeg(line, dataseg, &dc);
            }
            continue;
        }

        else if(isExternStatement(line)){
            label = getExternEntryLabel(line);
            /* external symbol */
            curr_label = createSymbol(label, 0, false, true, false);
            addSymbol(curr_label, symbol_table);
            free(label);
            continue;
        }

        else if(isEntryStatement(line))
            continue;

        else {
            if(symbolDefined){
                label = getLabel(line);
                /* not an instruction, not external */
                curr_label = createSymbol(label, ic, true, false, false); 
                addSymbol(curr_label, symbol_table);
                free(label);
            }
        
            if(!isValidInstruction(line, instructions)){
                error(UNKNOWN_INSTRUCTION, code_file->name, line_number);
                errors_found = true;
                continue;
            }

            name = getInstructionName(line);
            if(getAmountOfOperands(name, instructions) != countOperandsGiven(line)){
                error(WRONG_NO_OF_OPERANDS, code_file->name, line_number);
                errors_found = true;
                continue;
            }
    
            op1 = getOperand(1, line);
            op2 = getOperand(2, line);
            if(op1 && op2 == NULL)   
                isValid = validateOperandsAddressingMode(name, instructions, NULL, op1);
            if(op1 && op2)
                isValid = validateOperandsAddressingMode(name, instructions, op1, op2); 
            if(op1 == NULL && op2 == NULL)
                isValid = true;
            if(!isValid){
                error(ILLEGAL_ADDRESSING_MODE, code_file->name, line_number);
                errors_found = true;
                continue;
            }
            
            size = calculateInstructionSize(line, instructions);
            ic += size;        
        }
    }
    updateSymbolTable(symbol_table, ic);
    return errors_found; 
}
