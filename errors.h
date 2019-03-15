#ifndef ERRORS_H_
#define ERRORS_H_




/* defining all the error messages of the assembler */



#define ERRORS_HEADER "error: "
#define LINE_LENGTH_EXCEEDED "line length exceeded"
#define UNKNOWN_INSTRUCTION "Unknown Instruction"
#define NO_INPUT_FILES "No input files\nCompilation terminated."
#define UNKNOWN_SYMBOL "Unknown Symbol"
#define SYMBOL_REDEFINITION "Redefinition of Symbol"
#define WRONG_NO_OF_OPERANDS "Wrong number of operands given"
#define ILLEGAL_ADDRESSING_MODE "Illegal addressing mode"
#define ILLEGAL_LABEL "Illegal label"

void error(char* error, char* file_name, int line_number);

#endif
