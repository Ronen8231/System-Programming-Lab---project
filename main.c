#include "symbols.h"
#include "dataseg.h"
#include <stdlib.h>
#include "first_pass.h"
#include "instructions.h"

int main(int argc, char** argv){
    int i, j
    SymbolTable* symbol_table;
    DataSegment* dataseg = (DataSegment*)malloc(sizeof(DataSegment));
    FILE* curr_file;
    DataSegmentEntry* entry;
    Symbol* symbol;
    InstructionGroup* instructions = allInstructions(); 
    dataseg->first = NULL;
    if(argc < 2){
        /* error(NO_INPUT_FILES); */
        printf("compilation terminated.\n");
    }

    for(i = 1; i < argc; i++){
        symbol_table = createTable();
        curr_file = fopen(argv[i], "r");
        
        first_pass(curr_file, symbol_table, dataseg, instructions);
        

    }
    return 0;
}
