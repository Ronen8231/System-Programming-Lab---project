/**************************************************************************

    File:           support.c

    Written By:     Ronen Hanukayev
    ID:             322881327

    Description:    This is the implementation of the support.h header. 

**************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "support.h"
#include "errors.h"

char* read_line(FILE* file){
    char* line = (char*)malloc(MAX_LINE_LENGTH + 1);
    char c;
    int i = 0;

    memset(line, 0, MAX_LINE_LENGTH + 1);

    c = fgetc(file);
    if(c == EOF){
        free(line);
        return EOF_REACHED;
    }

    line[i++] = c;

    while(i < MAX_LINE_LENGTH){
        c = fgetc(file);
        if(c == '\n')
            return line;
        if(c == EOF){
            return line;
        }
        line[i++] = c;
    }
    
    free(line);    
    return LINE_LENGTH_EXCEEDED;
} 

void replace_tabs_with_spaces(char *str){
	int pos = 0;
	while(str[pos] != '\0'){
		if(str[pos] == '\t')
			str[pos] = ' ';
		pos++;
	}
}
