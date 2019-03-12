#include "support.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instructions.h"

/*
bool isLegal(char* line){
    if(strlen(line) > 80)
        return false;
    if(isNumber(line[0])
        return false;
}*/
bool isRegister(char* op){
    return op[0] == '@';
}
bool isLabeled(char* line){
    int i;
    for(i = 0; i < strlen(line); i++){
        if(line[i] == '"') /* we reached a declaration of a string, which might contain a colon */
            return false;
        if(line[i] == ':')
            return true;
    }
    return false;
}

char* getLabel(char* line){
    char* label;
    char* code_line = (char*)malloc(strlen(line) + 1);
    strcpy(code_line, line);
    label = strtok(code_line, ":");
    return label;
}

bool isDataStatement(char* line){
    if(strstr(line, ".data"))
        return true;
    return false;
}

bool isStringStatement(char* line){
    if(strstr(line, ".string"))
        return true;
    return false;
}

bool isExternStatement(char* line){
    if(strstr(line, ".extern"))
        return true;
    return false;
}

bool isEntryStatement(char* line){
    if(strstr(line, ".entry"))
        return true;
    return false;
}

char* getExternEntryLabel(char* line){
    char* label;
    char* argument;
    char* code;
    char* code_line = (char*)malloc(strlen(line) + 1);
    strcpy(code_line, line);
    if(isEntryStatement(line)){
        code = strstr(code_line, ".entry");
        argument = strtok(code, ".entry");
    }
    else{
        code = strstr(code_line, ".extern");
        argument = strtok(code, ".extern");
    }
    
    replace_tabs_with_spaces(argument); 
    argument = strtok(argument, " ");
    label = (char*) malloc (strlen(argument) + 1);
    strcpy(label, argument);

    free(code_line);
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
    char* token = NULL;
    char* final_operand;
    int size = 0;
    int i;
    char* full_line = (char*)malloc(strlen(line) + 1);
    strcpy(full_line, line);
    
    replace_tabs_with_spaces(full_line);
    
    if(isLabeled(line)){
        token = strtok(full_line, " "); /* label */
        token = strtok(NULL, " "); /* instruction */
    }
    else
        token = strtok(full_line, " "); /* instruction */

    token = strtok(NULL, " "); 
    if(operand_num < 1 || operand_num > 2) /* impossible */
        return NULL;
    
    for(i = 0; i < strlen(token) && token[i] != ','; i++, size++);
        
    if(operand_num == 1){
        final_operand = (char*)malloc(size + 1);
        memset(final_operand, 0, size + 1);
        strncpy(final_operand, token, size);
    }

    if(operand_num == 2){
        if(i == strlen(token)){ /* comma isn't part of the token */
            token = strtok(NULL, " ");
            if(strlen(token) == 1) /* comma plus null terminator */
                token = strtok(NULL, " ");
            size = strlen(token);
            if(token[0] == ',') size--;
            final_operand = (char*)malloc(size + 1);
            if(token[0] == ',') strcpy(final_operand, &(token[1]));
            else strcpy(final_operand, token);
        }
        else{
            i += 1;
            if(i == strlen(token)){
                token = strtok(NULL, " ");
                size = strlen(token);
                final_operand = (char*)malloc(size + 1);
                strcpy(final_operand, token);
            }
            else{
                size = strlen(token) - i;
                final_operand = (char*)malloc(size + 1);
                memset(final_operand, 0, size + 1);
                strncpy(final_operand, &(token[i]), size);
            }

        }
    }
 
    free(full_line);
    return final_operand;
}
