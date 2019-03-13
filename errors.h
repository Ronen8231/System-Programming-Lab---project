#ifndef ERRORS_H_
#define ERRORS_H_

#define ERRORS_HEADER "error: "
#define LINE_LENGTH_EXCEEDED "line length exceeded"
#define UNKNOWN_INSTRUCTION "Unknown Instruction"
#define NO_INPUT_FILES "No input files\nCompilation terminated."
#define UNKNOWN_SYMBOL "Unknown Symbol"

void error(char* error, char* file_name, int line_number);

#endif
