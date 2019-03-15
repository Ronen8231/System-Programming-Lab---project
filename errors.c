#include "errors.h"
#include <stdio.h>


/* prints an error message to stderr */
void error(char* error, char* file_name, int line_number){
    if(line_number > 0)
        fprintf(stderr, "%s: line %d: %s%s\n", file_name, line_number, ERRORS_HEADER, error); 
    else
        fprintf(stderr, "%s: %s%s\n", file_name, ERRORS_HEADER, error); 
}
