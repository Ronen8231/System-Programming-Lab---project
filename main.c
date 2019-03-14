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
    DataSegment* dataseg = (DataSegment*)malloc(sizeof(DataSegment));
    NamedFile* curr_file = (NamedFile*)malloc(sizeof(NamedFile));
    InstructionGroup* instructions = allInstructions(); 
    dataseg->first = NULL;
    if(argc < 2){
        error(NO_INPUT_FILES, argv[0], -1); 
    }

    for(i = 1; i < argc; i++){
        symbol_table = createTable();
        curr_file->name = (char*)malloc(strlen(argv[i]) + 1);
        strcpy(curr_file->name, argv[i]);
        curr_file->file = fopen(strcat(argv[i], ".as"), "r");
        printf("name = %s\n", argv[i]);
        first_pass(curr_file, symbol_table, dataseg, instructions);
        fseek(curr_file->file, 0, SEEK_SET);
        second_pass(curr_file, symbol_table, instructions, dataseg);
        printf("DONE\n");
    }
    return 0;
}
