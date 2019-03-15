#include <string.h>
#include "symbols.h"
#include "dataseg.h"
#include <stdlib.h>
#include "first_pass.h"
#include "instructions.h"
#include "errors.h"
#include "second_pass.h"

int main(int argc, char** argv){
    int i;
    SymbolTable* symbol_table;
    DataSegment* dataseg;
    NamedFile* curr_file = (NamedFile*)malloc(sizeof(NamedFile));
    bool errors_found = false;
    char* name;
    InstructionGroup* instructions = allInstructions(); 
    if(argc < 2){
        error(NO_INPUT_FILES, argv[0], -1); 
    }

    for(i = 1; i < argc; i++){
        dataseg =  (DataSegment*)malloc(sizeof(DataSegment));
        dataseg->first = NULL;
        symbol_table = createTable();
        curr_file->name = (char*)malloc(strlen(argv[i]) + 1);
        strcpy(curr_file->name, argv[i]);
        name = (char*)malloc(strlen(argv[i]) + 4);
        strcpy(name, argv[i]);
        curr_file->file = fopen(strcat(name, ".as"), "r");
        free(name);
        errors_found = first_pass(curr_file, symbol_table, dataseg, instructions);
        fseek(curr_file->file, 0, SEEK_SET);
        if(!errors_found)
            second_pass(curr_file, symbol_table, instructions, dataseg);
        free(dataseg);
    }
    return 0;
}
