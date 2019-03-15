#include "support.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#include "parsing.h"
#include <ctype.h>


/* count actual amount of operands given */
int countOperandsGiven(char* line){
    int no_of_operands = 0;
    char* full_line = (char*)malloc(strlen(line) + 1);
    bool labeled = isLabeled(line);
    char* token;
    strcpy(full_line, line);
    replace_tabs_with_spaces(full_line);
    token = strtok(full_line, " ,");
    if(labeled)
        token = strtok(NULL, " ,");
    
    token = strtok(NULL, " ,");
    while(token){
        no_of_operands++;
        token = strtok(NULL, " ,");
    }
    free(full_line);
    return no_of_operands;
}


/* returns true if the line is empty */
bool isEmptyLine(char* line){
    int i;
    if(strlen(line) == 0)
        return true;
    for( i = 0; i < strlen(line); i++){
        if(line[i] != ' ' && line[i] != '\t')
            return false;
    }
    return true;
}


/* assuming valid register */
int getRegisterNumber(char* reg){
    return atoi(&reg[2]);
}

bool isValidLabel(char* label){
    int i;
    int len = strlen(label);
    if(len < 1 || len > MAX_LABEL_LENGTH) return false;
    for(i = 0; i < len; i++){
        if(i==0){
            if(!isalpha(label[i]))
                return false;
        }
        else{
            if(!isalnum(label[i]))
                return false;
        }
    }
    return true;
}


/* returns none if not valid */
AddressingMode getOperandsAddressingMode(char* operand){
    if(isValidRegister(operand))
        return Register;
    if(isValidLabel(operand))
        return Direct;
    if(atoi(operand) || strcmp(operand, "0") == 0 || strcmp(operand, "-0") == 0)
        return Immediate;
    return None;
}


bool isRegister(char* op){
    return op[0] == '@';
}

bool isValidRegister(char* op){
    if(strlen(op) == 3 && op[0] == '@' && op[1] == 'r' && atoi(&(op[2])) >= 0 && atoi(&(op[2])) <= 7 )
        return true;
    return false;   
}

bool isLabeled(char* line){
    bool ret;
    char* full_line;
    char* token;
    full_line = (char*)malloc(strlen(line) + 1);
    strcpy(full_line, line);
    replace_tabs_with_spaces(full_line);
    token = strtok(full_line, " ");
    if(token[strlen(token) - 1] == ':')
        ret = true;
    else ret = false;
    free(full_line);
    return ret;
}

/* assuming label exists in the line, label should be freed after use */
char* getLabel(char* line){
    char* label;
    char* token;
    char* code_line = (char*)malloc(strlen(line) + 1);
    strcpy(code_line, line);
    token = strtok(code_line, ":");
    label = (char*)malloc(strlen(token) + 1);
    strcpy(label, token);
    free(code_line);
    return label;
}

/* gets the first field of the code line; either
    a statement(data, string etc) or an instruction */
char* getStatementInstruction(char* line){
    char* full_line;
    char* token;
    char* state_inst;
    bool labeled = isLabeled(line);
    full_line = (char*)malloc(strlen(line) + 1);
    strcpy(full_line, line);
    replace_tabs_with_spaces(full_line);
    token = strtok(full_line, " ");
    if(labeled)    
        token = strtok(NULL, " ");
    state_inst = (char*)malloc(strlen(token) + 1);
    strcpy(state_inst, token);
    free(full_line);
    return state_inst;
}


bool isDataStatement(char* line){
    bool ret;
    char* statement = getStatementInstruction(line);
    if(strcmp(statement, ".data") == 0)
        ret = true;
    else ret = false;
    free(statement);
    return ret;
}

bool isStringStatement(char* line){
    bool ret;
    char* statement = getStatementInstruction(line);
    if(strcmp(statement, ".string") == 0)
        ret = true;
    else ret = false;
    free(statement);
    return ret;
}

bool isExternStatement(char* line){
    bool ret;
    char* statement = getStatementInstruction(line);
    if(strcmp(statement, ".extern") == 0)
        ret = true;
    else ret = false;
    free(statement);
    return ret;
}

bool isEntryStatement(char* line){
    bool ret;
    char* statement = getStatementInstruction(line);
    if(strcmp(statement, ".entry") == 0)
        ret = true;
    else ret = false;
    free(statement);
    return ret;
}


/* get the label that comed after the extern/entry statement */
char* getExternEntryLabel(char* line){
    char* full_line;
    char* token;
    char* label;

    full_line = (char*)malloc(strlen(line) + 1);
    strcpy(full_line, line);
    replace_tabs_with_spaces(full_line);

    token = strtok(full_line, " ");
    token = strtok(NULL, " ");
    if(isLabeled(line))
        token = strtok(full_line, " ");
    label = (char*)malloc(strlen(token) + 1);
    strcpy(label, token);
    free(full_line);
    return label;
}


/* instruction should be freed after use */
char* getInstructionName(char* line){
    char* instruction;
    char* final_instruction;
    char* code_line;
    char* full_line = (char*)malloc(strlen(line) + 1);
    strcpy(full_line, line);
    if(isLabeled(line)){
        code_line = strtok(full_line, ":");
        code_line = strtok(NULL, ":");
    }
    else
        code_line = full_line;
    
    replace_tabs_with_spaces(code_line);
    instruction = strtok(code_line, " ");
    final_instruction = (char*)malloc(strlen(instruction) + 1);
    strcpy(final_instruction, instruction);
    free(full_line);
    return final_instruction;
}

bool isValidInstruction(char* line, InstructionGroup* instructions){
    char* instruction;
    bool ret = false;
    instruction = getInstructionName(line);
    if(instructionExists(instruction, instructions))
        ret = true;
    free(instruction);
    return ret;
}

char* getOperand(int operand_num, char* line){
    char* full_line = (char*)malloc(strlen(line) + 1);
    char* token;
    int i;
    bool labeled = isLabeled(line);
    char* operand;
    strcpy(full_line, line);
    replace_tabs_with_spaces(full_line);
    token = strtok(full_line, " ,");
    if(labeled)
        token = strtok(NULL, " ,");

    for(i = 0; i < operand_num; i++){
        if(token == NULL){
            free(full_line);
            return NULL;
        }
        token = strtok(NULL, " ,");
    }
    if(token == NULL){
        free(full_line);
        return NULL;
    }
    operand = (char*)malloc(strlen(token) + 1);
    strcpy(operand, token);
    free(full_line);
    return operand;
}
