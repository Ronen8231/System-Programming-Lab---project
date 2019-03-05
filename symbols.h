/**************************************************************************

    File:           symbols.h

    Written By:     Ronen Hanukayev
    ID:             322881327
    Date:           04.03.2019

    Description:    header file defining the symbol and the symbol table
                    structs and declaring basic functions to handle those
                    structs. 

**************************************************************************/

#define INITIAL_SIZE 10 /* initial size of the symbol table */

/* defining a symbol */
typedef struct{
    char* name; /* the name of the symbol */
    int value; /* value of the symbol */
    bool is_instruction; /* boolean to determine if the symbol represents an instruction statement */
    bool is_external; /* boolean to determine whether the symbol is external or not */
} Symbol;

/* defining the symbol table struct */
typedef struct{
    symbol* table; /* dynamic array of symbols */
    int current_size; /* current size of the symbol table (amount of symbols) */
    int size; /* amount of symbols the table can contain */
} SymbolTable;


SymbolTable* createTable();

Symbol* createSymbol(char* name, int value, bool instruction, bool external);

void addSymbol(Symbol* s, SymbolTable* t);




