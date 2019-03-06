/**************************************************************************

    File:           symbols.c

    Written By:     Ronen Hanukayev
    ID:             322881327
    Date:           05.03.2019

    Description:   Implementation of the symbols header file.

 
**************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "symbols.h"


SymbolTable* createTable(){
    SymbolTable* symbol_table = (SymbolTable*)malloc(sizeof(SymbolTable));
    symbol_table->table = (Symbol**)malloc(sizeof(Symbol*) * INITIAL_SIZE);
    symbol_table->current_size = 0;
    symbol_table->size = INITIAL_SIZE;
    return symbol_table;
}


Symbol* createSymbol(char* name, int value, bool instruction, bool external){
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
        symbol_table->table = (Symbol**)realloc(symbol_table->table, 
                                            symbol_table->size * ENLARGEMENT_FACTOR * sizeof(Symbol*)); 
        symbol_table->size = symbol_table->size * ENLARGEMENT_FACTOR;
    }
    symbol_table->table[symbol_table->current_size] = symbol;
    symbol_table->current_size++;

}

int getValue(char* symbol_name, SymbolTable* symbol_table){
    int i;
    Symbol* current;
    for(i = 0; i < symbol_table->current_size; i++){
        current = symbol_table->table[i];
        if(strcmp(current->name, symbol_name) == 0) /* symbol found */
            return current->value;
    }
    return NO_SUCH_SYMBOL;
}
