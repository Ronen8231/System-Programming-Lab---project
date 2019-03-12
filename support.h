/**************************************************************************

    File:           support.h

    Written By:     Ronen Hanukayev
    ID:             322881327

    Description:    This header file defines useful data types
                    such as bool, and declares supporting functions
                    such as the read_line function. It also defines 
                    useful constants.

**************************************************************************/

#include <stdio.h>

#ifndef SUPPORT_H_
#define SUPPORT_H_

#define MAX_LINE_LENGTH 80 /* defining the maximum length of a single line in the input */
#define EOF_REACHED "eof\n" /* a string that is used as a return value when EOF reached */



typedef enum{false, true} bool; /* defining the boolean data type */



/**************************************************************************
    
    This function reads a line from a file(until the '\n' character)
    and returns the line as a string.
    NOTE: THE STRING SHOULD BE FREED USING free() AFTER IT WAS USED!

    Parameters: 
        file -  The file to read from.

    Returns:    
        A string that contains the line that was read.
        Returns LINE_LENGTH_EXCEEDED(defined in errors.h) if 
        the line was longer than MAX_LINE_LENGTH. 
        Returns EOF_REACHED if the end of file was reached.
        In the last to cases, there is no need to free() the string.  

**************************************************************************/
char* read_line(FILE* file);

/**************************************************************************

    Replaces every occurence of a tab character with a space character 
    in a given string

    Parameters:
	    str - the string to be altered

    Returns: Nothing

**************************************************************************/
void replace_tabs_with_spaces(char *str);

#endif
