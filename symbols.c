/**************************************************************************

    File:           symbols.c

    Written By:     Ronen Hanukayev
    ID:             322881327
    Date:           05.03.2019

    Description:   Implementation of the symbols header file.

 
**************************************************************************/

#include <string.h>
#include "symbols.h"

#define INITIAL_SIZE 10 /* initial size of the symbol table */

/* defining a symbol */
typedef struct{
    char* name; /* the name of the symbol */
    int value; /* value of the symbol */
    bool isInstruction; /* boolean to determine if the symbol represents an instruction statement */
    bool isExternal; /* boolean to determine whether the symbol is external or not */
} symbol;

/* defining the symbol table struct */
typedef struct{
    symbol* table; /* dynamic array of symbols */
    int size; /* current size of the symbol table (amount of symbols) */
} symbolTable;



/**************************************************************************
    
    This function creates an empty symbol table in size of INITIAL_SIZE
    and returns a pointer to that table.

**************************************************************************/
SymbolTable* createTable(){
    SymbolTable* symbol_table = (SymbolTable*)malloc(sizeof(SymbolTable));
    symbol_table->table = (Symbol*)malloc(sizeof(Symbol) * INITIAL_SIZE);
    symbol_table->current_size = 0;
    symbol_table->size = INITIAL_SIZE;
    return symbol_table;
}


symbol* createSymbol(char* name, int value, bool instruction, bool external){
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = (char*)malloc(strlen(name) + 1);
    strcpy(symbol->name, name);
    symbol->value = value;
    symbol->is_instruction = instruction;
    symbol->is_external = external;
    return symbol;
}



void addSymbol(Symbol* symbol, SymbolTable* symbol_table){
    if(symbol_table->current_size == symbol_table->size){
        
    }

}




