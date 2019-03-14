/**************************************************************************

    File:           symbols.h

    Written By:     Ronen Hanukayev
    ID:             322881327
    Date:           04.03.2019

    Description:    header file defining the symbol and the symbol table
                    structs and declaring basic functions to handle those
                    structs. 

**************************************************************************/


#ifndef SYMBOLS_H_
#define SYMBOLS_H_


#include "support.h"

#define INITIAL_SIZE 10 /* initial size of the symbol table */
#define ENLARGEMENT_FACTOR 1.5 /* the factor that will be used to enlarge the table in case it is full */
#define NO_SUCH_SYMBOL -1 /* return code that says that a symbol does not exist */

/* defining a symbol */
typedef struct{
    char* name; /* the name of the symbol */
    int value; /* value of the symbol */
    bool is_instruction; /* boolean to determine if the symbol represents an instruction statement */
    bool is_external; /* boolean to determine whether the symbol is external or not */
    bool is_entry;
} Symbol;

/* defining the symbol table struct */
typedef struct{
    Symbol** table; /* dynamic array of symbols */
    int current_size; /* current size of the symbol table (amount of symbols) */
    int size; /* amount of symbols the table can contain */
} SymbolTable;


/**************************************************************************
    
    This function creates an empty symbol table in size of INITIAL_SIZE
    and returns a pointer to that table.

    Parameters:     None

    Returns:        A pointer to the symbol table that was created.

**************************************************************************/
SymbolTable* createTable();


/**************************************************************************
    
    This function creates a symbol and returns a pointer to that symbol.

    Parameters: name -          the name of the symbol
                value -         the value of the symbol
                instruction -   boolean to determine whether the symbol
                                represents an instruction statement
                external -      boolean to determine whether the symbol
                                is external or not.
                entry -         boolean to determine whether the symbol
                                was declared in an entry statement or not.

    Returns:    A pointer to the symbol that was created.

**************************************************************************/
Symbol* createSymbol(char* name, int value, bool instruction, bool external, bool entry);


/**************************************************************************

    A function that adds a symbol to a given symbol table. If the symbol
    table is full, enlarge it by ENLARGEMENT_FACTOR

    Parameters:     symbol -        a pointer to the symbol which will be
                                    added to the table
                    symbol_table -  the symbol table which will contain
                                    the new symbol
    Returns: Nothing.

**************************************************************************/
void addSymbol(Symbol* symnol, SymbolTable* symbol_table);


/*************************************************************************

    Returns the value of a symbol.

    Parameters: symbol_name -   the name of the symbol
                symbol_table -  the symbol table

    Returns:    The value of the symbol, or NO_SUCH_SYMBOL if the symbol
                doesn't exist.

*************************************************************************/
int getValue(char* symbol_name, SymbolTable* symbol_table);

bool isExternal(char*, SymbolTable*);

#endif

