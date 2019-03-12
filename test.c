/*************************************************************************

This file purpose is checking and testing functions

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
int end = 1;


char* read_line(FILE* file){
    char* line = (char*)malloc(80);
    char c;
    int i = 0;
    memset(line, 0, 80);
    while(1){
        c = fgetc(file);
        if(c == '\n')
            return line;
        if(c == EOF){
            end=0;
            return line;
        }
        line[i++] = c;
    }
    return line;
}


int main(int argc, char** argv){
    char* str = (char*) malloc(80);
    char* code;
    char* f;
    int i;
    strcpy(str, "Hello: .string \"abcdef\"");
    code = strstr(str, ".string");
    f = strtok(code, ".string");
    for(i = 0; i < 17; i++){
        printf("%c\n", f[i]); 
    }   


    return 0;
}
