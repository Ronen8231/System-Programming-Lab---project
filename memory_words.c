#include "memory_words.h"
#include "instructions.h"
#include <stdlib.h>

/* creating an instruction word */
InstructionWord* createInstructionWord(AddressingMode first, int opcode, AddressingMode second, ARE are){
    InstructionWord* word = (InstructionWord*) malloc(sizeof(InstructionWord));
    word->first_addressing_mode = first;
    word->opcode = opcode;
    word->second_addressing_mode = second;
    word->ARE = are;
    return word;
}




/* calculating the integer value of an instruction word/ a register word */


int instructionWordToInt(InstructionWord* instruction_word){
    int final_value = 0;
    int current;
    current = instruction_word->first_addressing_mode;
    current <<= 9; /* first addressing modes is on bytes 9 to 11 */
    final_value += current;
    current = instruction_word->opcode;
    current <<= 5; /* opcode is on bytes 5 to 8 */
    final_value += current;
    current = instruction_word->second_addressing_mode;
    current <<= 2; /* first addressing modes is on bytes 2 to 4 */
    final_value += current;
    final_value += instruction_word->ARE; /* A,R,E field is on bytes 0 to 1, no need to shift left */
    return final_value;
}

int registerWordToInt(RegisterAddressingWord* word){
    int final_value = 0;
    int current = word->src_reg;
    current <<= 7; /* source register is on bytes 7 to 11 */
    final_value += current;
    current = word-> dest_reg;
    current <<= 2; /* dest register is on bytes 2 to 6 */
    final_value += current;
    final_value += word->ARE; /* A,R,E field is on bytes 0 to 1, no need to shift left */
    return final_value;
} 
