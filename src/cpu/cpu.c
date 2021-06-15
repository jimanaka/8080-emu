#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>

typedef struct Flags
{
    uint8_t z:1; //set 1 when res=0
    uint8_t s:1; //set w/MSB set
    uint8_t p:1; //parity
    uint8_t cy:1; //carry
    uint8_t ac:1; //accumulator
} Flags;

typedef struct State
{
    uint8_t     a; //registers
    uint8_t     b;
    uint8_t     c;
    uint8_t     d;
    uint8_t     e;
    uint8_t     h;
    uint8_t     l;
    uint16_t    sp;
    uint16_t    pc;
    uint8_t     *memory; //memory
    struct      Flags flags; //flags struct
    uint8_t     int_enable;
} State;

void UnimplementedIns(State *state)
{
    printf("Instruction not yet implemented");
    exit(1);
}

int EmulateCPU(State *state)
{
    unsigned char *opcode = &state->memory[state->pc];
    return 0;
}

int main(void)
{
    return 0;
}