#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *memory;

void init(registers_t *starting_registers)
{
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    // Initialize memory with a decent number of buckets (e.g., 4096)
    memory = ht_init(4096);
}

// TODO: create any necessary helper functions

/**
 * Extracts the register index from a string like "x5" or "x10".
 */
int parse_reg(char *str) {
    if (!str || *str != 'x') return 0; // Basic safety
    return atoi(str + 1); // Skip 'x' and parse integer
}

/**
 * Helper to safely write to a register.
 * Writes to x0 are always ignored.
 */
void write_reg(int reg_idx, int value) {
    if (reg_idx != 0) {
        registers->r[reg_idx] = value;
    }
}

/**
 * Extracts the next token from the string string, splitting by comma, space, or parentheses.
 */
char* get_token(char **str_ptr) {
    return strsep(str_ptr, ", ()");
}

/**
 * Memory Helper: Write a 32-bit word to memory (Little Endian).
 * Splits the 32-bit int into 4 bytes.
 */
void mem_write_word(int addr, int val) {
    ht_add(memory, addr + 0, val & 0xFF);
    ht_add(memory, addr + 1, (val >> 8) & 0xFF);
    ht_add(memory, addr + 2, (val >> 16) & 0xFF);
    ht_add(memory, addr + 3, (val >> 24) & 0xFF);
}

/**
 * Memory Helper: Write a single byte to memory.
 */
void mem_write_byte(int addr, int val) {
    ht_add(memory, addr, val & 0xFF);
}

/**
 * Memory Helper: Read a 32-bit word from memory (Little Endian).
 * Recombines 4 bytes into an int.
 */
int mem_read_word(int addr) {
    int b0 = ht_get(memory, addr + 0);
    int b1 = ht_get(memory, addr + 1);
    int b2 = ht_get(memory, addr + 2);
    int b3 = ht_get(memory, addr + 3);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

/**
 * Memory Helper: Read a single byte from memory and sign-extend it.
 */
int mem_read_byte(int addr) {
    int b = ht_get(memory, addr);
    // Sign extension for 8-bit to 32-bit
    return (int)(int8_t)b;
}

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    // TODO: write logic for evaluating instruction on current interpreter state

    // Pointers for tokens
    char *token1, *token2, *token3;
    int rd, rs1, rs2, imm;

    // Helper macro to skip empty tokens caused by multiple delimiters (like ", ")
    #define NEXT_TOKEN(t) do { t = get_token(&instruction); } while(t && strlen(t) == 0)

    if (op_type == R_TYPE) {
        // Format: add rd, rs1, rs2
        NEXT_TOKEN(token1); // rd
        NEXT_TOKEN(token2); // rs1
        NEXT_TOKEN(token3); // rs2

        rd = parse_reg(token1);
        rs1 = parse_reg(token2);
        rs2 = parse_reg(token3);

        int val1 = registers->r[rs1];
        int val2 = registers->r[rs2];
        int result = 0;

        if (strcmp(op, "add") == 0) result = val1 + val2;
        else if (strcmp(op, "sub") == 0) result = val1 - val2;
        else if (strcmp(op, "and") == 0) result = val1 & val2;
        else if (strcmp(op, "or") == 0)  result = val1 | val2;
        else if (strcmp(op, "xor") == 0) result = val1 ^ val2;
        else if (strcmp(op, "nor") == 0) result = ~(val1 | val2);
        else if (strcmp(op, "slt") == 0) result = (val1 < val2) ? 1 : 0;
        else if (strcmp(op, "sll") == 0) result = val1 << val2;
        else if (strcmp(op, "sra") == 0) result = val1 >> val2; // Arithmetic shift (usually default for signed int in C)

        write_reg(rd, result);

    } else if (op_type == I_TYPE) {
        // Format: addi rd, rs1, imm
        NEXT_TOKEN(token1); // rd
        NEXT_TOKEN(token2); // rs1
        NEXT_TOKEN(token3); // imm

        rd = parse_reg(token1);
        rs1 = parse_reg(token2);
        imm = (int)strtol(token3, NULL, 0); // Handles hex (0x) and decimal

        int val1 = registers->r[rs1];
        int result = 0;

        if (strcmp(op, "addi") == 0) result = val1 + imm;
        else if (strcmp(op, "andi") == 0) result = val1 & imm;
        else if (strcmp(op, "ori") == 0)  result = val1 | imm;
        else if (strcmp(op, "xori") == 0) result = val1 ^ imm;
        else if (strcmp(op, "slti") == 0) result = (val1 < imm) ? 1 : 0;

        write_reg(rd, result);

    } else if (op_type == MEM_TYPE) {
        // Format: lw rd, offset(rs1)  OR  sw rs2, offset(rs1)
        // Token parsing is tricky: "offset(rs1)"
        // We can parse: token1 (rd/rs2), token2 (offset), then parse register from inside instruction manually or next token
        
        NEXT_TOKEN(token1); // rd (for load) or rs2 (for store)
        NEXT_TOKEN(token2); // offset
        NEXT_TOKEN(token3); // rs1 (extracted because we split by parens "()")

        // If the string was "8(x5)", strsep split at '(', so token2 is "8", token3 is "x5"
        
        int reg_op = parse_reg(token1); // rd or rs2
        int offset = (int)strtol(token2, NULL, 0);
        int base_reg = parse_reg(token3);
        int addr = registers->r[base_reg] + offset;

        if (strcmp(op, "lw") == 0) {
            int val = mem_read_word(addr);
            write_reg(reg_op, val);
        } else if (strcmp(op, "lb") == 0) {
            int val = mem_read_byte(addr);
            write_reg(reg_op, val);
        } else if (strcmp(op, "sw") == 0) {
            mem_write_word(addr, registers->r[reg_op]);
        } else if (strcmp(op, "sb") == 0) {
            mem_write_byte(addr, registers->r[reg_op]);
        }

    } else if (op_type == U_TYPE) {
        // Format: lui rd, imm
        NEXT_TOKEN(token1); // rd
        NEXT_TOKEN(token2); // imm

        rd = parse_reg(token1);
        imm = (int)strtol(token2, NULL, 0);

        // LUI places immediate in top 20 bits
        int result = imm << 12;
        write_reg(rd, result);
    }
}
