#include "support.h"
#include "dataseg.h"
#include <string.h>
#include <stdlib.h>
#include "symbols.h"
#include "parsing.h"
#include "instructions.h"
#include "errors.h"

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
        appendData(dataseg, *dc, atoi(curr_data));
        (*dc)++;
        curr_data = strtok(NULL, ",");
    }
    
    free(code_line);
}


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
        appendData(dataseg, *dc, string[i]);
        (*dc)++;
        i++;
    }
    appendData(dataseg, *dc, 0); /* null terminator */
    (*dc)++;
    free(code_line);
}

int calculateInstructionSize(char* line, InstructionGroup* instructions){
    char* instruction_name;
    char* op1;
    char* op2;
    int amount_of_operands = 0;
    Instruction* instruction;
   
    instruction_name = getInstructionName(line);
    instruction = getInstruction(instruction_name, instructions);
    if(instruction->operand1[0] != None) amount_of_operands++; 
    if(instruction->operand2[0] != None) amount_of_operands++; 
    if(amount_of_operands == 0)
        return 1; /* only one word of data needed */
    if(amount_of_operands == 1)
        return 2; /* two words of data needed */ 
    op1 = getOperand(1, line);
    op2 = getOperand(2, line);
    if(amount_of_operands == 2){
        if(isRegister(op1) && isRegister(op2))
            return 2;
        return 3;
    }
    return -1; /* impossible in valid code */
}

void updateDataSeg(DataSegment* dataseg, int start_address){
    DataSegmentEntry* current = dataseg->first;
    while(current != NULL){
        current->address += start_address;
        current = current->next;
    }
}

void first_pass(FILE* code_file, SymbolTable* symbol_table, DataSegment* dataseg, InstructionGroup* instructions){
    int ic, dc;
    int size;
    bool symbolDefined;
   /* bool errorsFound; */
    char* line;
    char* label;
    Symbol* curr_label;
    ic=100;
    dc=0;
    while( strcmp((line = read_line(code_file)), EOF_REACHED) != 0 ){
        size = 0;
        if(isLabeled(line))
            symbolDefined = true;
        else
            symbolDefined = false;
        
        if(isDataStatement(line) || isStringStatement(line)){
            if(symbolDefined){
                label = getLabel(line);
                
                /* not an instruction, not external */
                curr_label = createSymbol(label, dc, false, false); 
                
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
            curr_label = createSymbol(label, 0, false, true);
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
                curr_label = createSymbol(label, ic, true, false); 
                addSymbol(curr_label, symbol_table);
                free(label);
            }
        
            if(!isValidInstruction(line, instructions)){
                fprintf(stderr, "");
            }
        
            size = calculateInstructionSize(line, instructions);
            ic += size;        
        }
    }
    updateDataSeg(dataseg, ic);
}
