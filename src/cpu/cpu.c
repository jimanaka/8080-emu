#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct Flags
{
    uint8_t z : 1;  //set 1 when res=0
    uint8_t s : 1;  //set w/MSB set
    uint8_t p : 1;  //parity
    uint8_t cy : 1; //carry
    uint8_t ac : 1; //accumulator
} Flags;

typedef struct State
{
    uint8_t a; //registers
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t sp;
    uint16_t pc;
    uint8_t *memory;    //memory
    struct Flags flags; //flags struct
    uint8_t int_enable;
} State;

int parity(int x, int size)
{
    int i, count = 0, b = 1;
    for (i = 0; i < size; i++)
    {
        if (x & (b << i))
        {
            count++;
        }
    }
    if (count % 2)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void UnimplementedIns(State *state)
{
    printf("Instruction not implemented yet\n");
    exit(1);
}

void LoadBinary(State *state, char *filename, uint32_t offset)
{
    char path[11 + strlen(filename)];
    strcpy(path, "../../rom/");
    strcat(path, filename);
    printf("%s\n", path);
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        printf("Could not open binary '%s' (full path: %s\n", filename, path);
        exit(1);
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        printf("Could not seek end of file\n");
        exit(1);
    }
    const int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t *buffer = &state->memory[offset];
    fread(buffer, 1, len, fp);
    fclose(fp);
}

int EmulateCPU(State *state)
{
    unsigned char *opcode = &state->memory[state->pc];
    int bytesUsed = 1;
    state->pc++;
    printf("%04x ", state->pc);
    switch (*opcode)
    {
    case 0x00:
        break; //NOP
    case 0x01:
    {
        state->c = opcode[2];
        state->b = opcode[3];
        state->pc += 2;
    }
    break;
    case 0x02:
        UnimplementedIns(state);
        break;
    case 0x03:
        UnimplementedIns(state);
        break;
    case 0x04:
        UnimplementedIns(state);
        break;
    case 0x05:
    {
        uint8_t temp = state->b - 1;
        state->flags.z = (temp == 0);
        state->flags.s = (0x80 == (temp & 0x80)); //check if sig bit is set
        state->flags.p = parity(temp, 8);
        state->b = temp;
    }
    break;
    case 0x06:
    {
        state->b = opcode[1];
        state->pc++;
    }
    break;
    case 0x07:
        UnimplementedIns(state);
        break;
    case 0x08:
        UnimplementedIns(state);
        break;
    case 0x09: //HL = HL + BC
    {
        //32 bits to check for carrying
        uint32_t hl = (state->h << 8) | state->l;
        uint32_t bc = (state->b << 8) | state->c;
        uint32_t temp = hl + bc;
        state->h = temp & (0xff00) >> 8; //
        state->l = temp & 0xff;
        state->flags.cy = ((temp & 0xffff0000) > 0); //if there are any 1's in higher 16, then set carry flag
    }
    break;
    case 0x0a:
        UnimplementedIns(state);
        break;
    case 0x0b:
        UnimplementedIns(state);
        break;
    case 0x0c:
        UnimplementedIns(state);
        break;
    case 0x0d:
        {
            //DCR C
            uint8_t temp = state->c - 1;
            state->flags.z = (temp == 0);
            state->flags.s = (0x80 == (temp & 0x80));
            state->flags.p = parity(temp, 8);
            state->b = temp;
        }
        break;
    case 0x0e:
        {
            // Move Could
            state->c = opcode[1];
            state->pc++;
        }
        break;
    case 0x0f:
        {
            //Rotate A right
            uint8_t temp = state->a;
            state->a = (temp & 0x1) << 7;
            state->a = state->a | (temp >> 1);
            state->flags.cy = (1 == (temp & 0x1));
        }
        break;
    case 0x10:
        UnimplementedIns(state);
        break;
    case 0x11:
        {
            //Load immediate register LXI
            state->e = opcode[1];
            state->d = opcode[2];
            state->pc += 2;
        }
        break;
    case 0x12:
        UnimplementedIns(state);
        break;
    case 0x13:
        {
            //Increment d and e registers
            uint16_t temp = ((state->d << 8) | (state->e));
            temp++;
            state->d = ((temp & 0xff00) >> 8);
            state->e = ((temp & 0xff));
        }
        break;
    case 0x14:
        UnimplementedIns(state);
        break;
    case 0x15:
        UnimplementedIns(state);
        break;
    case 0x16:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0x17:
        UnimplementedIns(state);
        break;
    case 0x18:
        UnimplementedIns(state);
        break;
    case 0x19:
        {
            uint32_t hl = ((state->h) << 8) | (state->h);
            uint32_t de = ((state->d) << 8) | (state->e);
            uint32_t temp = hl + de;
            state->h = (temp & 0xff00) >> 8;
            state->l = (temp & 0xff);
            state->flags.cy = ((temp & 0xffff0000) > 0);
        }
        break;
    case 0x1a:
        {
            //Load A indirect
            uint16_t temp = ((state->d) << 8) | (state->e);
            state->a = state->memory[temp];
        }
        break;
    case 0x1b:
        UnimplementedIns(state);
        break;
    case 0x1c:
        UnimplementedIns(state);
        break;
    case 0x1d:
        UnimplementedIns(state);
        break;
    case 0x1e:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0x1f:
        UnimplementedIns(state);
        break;
    case 0x20:
        UnimplementedIns(state);
        break;
    case 0x21:
        {
            //Load register pair HL
            state->l = opcode[1];
            state->h = opcode[2];
            state->pc+=2;
        }
        break;
    case 0x22:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0x23:
        {
            //increment H and L
            uint16_t temp = (state->h << 8) | (state->l);
            temp++;
            state->h = (temp & 0xff00) >> 8;
            state->l = (temp&0xff);
        }
        break;
    case 0x24:
        UnimplementedIns(state);
        break;
    case 0x25:
        UnimplementedIns(state);
        break;
    case 0x26:
        {
            //Move Immediate H
            state->h = opcode[1];
            state->pc++;
        }
        bytesUsed = 2;
        break;
    case 0x27:
        UnimplementedIns(state);
        break;
    case 0x28:
        UnimplementedIns(state);
        break;
    case 0x29:
        {
            //Add HL to HL
            uint32_t hl = (state->h << 8) | (state->l);
            uint32_t temp = hl + hl;
            state->h = (temp & 0xff00) >> 8;
            state->l = (temp & 0xff);
            state->flags.cy = ((temp & 0xffff0000) > 0);
        }
        break;
    case 0x2a:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0x2b:
        UnimplementedIns(state);
        break;
    case 0x2c:
        UnimplementedIns(state);
        break;
    case 0x2d:
        UnimplementedIns(state);
        break;
    case 0x2e:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0x2f:
        UnimplementedIns(state);
        break;
    case 0x30:
        UnimplementedIns(state);
        break;
    case 0x31:
        {
            //Load immediate stack pionter
            state->sp = (opcode[2] << 8) | (opcode[1]);
            state->pc += 2;
        }
        break;
    case 0x32:
        {
            //store A direct
            uint16_t temp = (opcode[2]<<8) | (opcode[1]);
            state->memory[temp] = state->a;
            state->pc += 2;
        }
        break;
    case 0x33:
        UnimplementedIns(state);
        break;
    case 0x34:
        UnimplementedIns(state);
        break;
    case 0x35:
        UnimplementedIns(state);
        break;
    case 0x36:
        {
            uint16_t temp = (state->h << 8) | (state->l);
            state->memory[temp] = opcode[1];
            state->pc++;
        }
        break;
    case 0x37:
        UnimplementedIns(state);
        break;
    case 0x38:
        UnimplementedIns(state);
    case 0x39:
        UnimplementedIns(state);
        break;
    case 0x3a:
        {
            //load A direct
            uint16_t temp = (opcode[2] << 8) | (opcode[1]);
            state->a = state->memory[temp];
            state->pc += 2;
        }
        break;
    case 0x3b:
        UnimplementedIns(state);
        break;
    case 0x3c:
        UnimplementedIns(state);
        break;
    case 0x3d:
        UnimplementedIns(state);
        break;
    case 0x3e:
        {
            //Move immediate register A
            state->a = opcode[1];
            state->pc++;
        }
        break;
    case 0x3f:
        UnimplementedIns(state);
        break;
    case 0x40:
        UnimplementedIns(state);
        break;
    case 0x41:
        UnimplementedIns(state);
        break;
    case 0x42:
        UnimplementedIns(state);
        break;
    case 0x43:
        UnimplementedIns(state);
        break;
    case 0x44:
        UnimplementedIns(state);
        break;
    case 0x45:
        UnimplementedIns(state);
        break;
    case 0x46:
        UnimplementedIns(state);
        break;
    case 0x47:
        UnimplementedIns(state);
        break;
    case 0x48:
        UnimplementedIns(state);
        break;
    case 0x49:
        UnimplementedIns(state);
        break;
    case 0x4a:
        UnimplementedIns(state);
        break;
    case 0x4b:
        UnimplementedIns(state);
        break;
    case 0x4c:
        UnimplementedIns(state);
        break;
    case 0x4d:
        UnimplementedIns(state);
        break;
    case 0x4e:
        UnimplementedIns(state);
        break;
    case 0x4f:
        UnimplementedIns(state);
        break;
    case 0x50:
        UnimplementedIns(state);
        break;
    case 0x51:
        UnimplementedIns(state);
        break;
    case 0x52:
        UnimplementedIns(state);
        break;
    case 0x53:
        UnimplementedIns(state);
        break;
    case 0x54:
        UnimplementedIns(state);
        break;
    case 0x55:
        UnimplementedIns(state);
        break;
    case 0x56:
        {
            //move memory (HL) to register D
            uint16_t temp = (state->h << 8) | (state->l);
            state->d = state->memory[temp];
        }
        break;
    case 0x57:
        UnimplementedIns(state);
        break;
    case 0x58:
        UnimplementedIns(state);
        break;
    case 0x59:
        UnimplementedIns(state);
        break;
    case 0x5a:
        UnimplementedIns(state);
        break;
    case 0x5b:
        UnimplementedIns(state);
        break;
    case 0x5c:
        UnimplementedIns(state);
        break;
    case 0x5d:
        UnimplementedIns(state);
        break;
    case 0x5e:
        {
            //move memory (HL) to register E MOVE E,M
            uint16_t temp = (state->h << 8) | (state->l);
            state->e = state->memory[temp];
        }
        break;
    case 0x5f:
        UnimplementedIns(state);
        break;
    case 0x60:
        UnimplementedIns(state);
        break;
    case 0x61:
        UnimplementedIns(state);
        break;
    case 0x62:
        UnimplementedIns(state);
        break;
    case 0x63:
        UnimplementedIns(state);
        break;
    case 0x64:
        UnimplementedIns(state);
        break;
    case 0x65:
        UnimplementedIns(state);
        break;
    case 0x66:
        UnimplementedIns(state);
        break;
    case 0x67:
        UnimplementedIns(state);
        break;
    case 0x68:
        UnimplementedIns(state);
        break;
    case 0x69:
        UnimplementedIns(state);
        break;
    case 0x6a:
        UnimplementedIns(state);
        break;
    case 0x6b:
        UnimplementedIns(state);
        break;
    case 0x6c:
        UnimplementedIns(state);
        break;
    case 0x6d:
        UnimplementedIns(state);
        break;
    case 0x6e:
        UnimplementedIns(state);
        break;
    case 0x6f:
        {
            //move reg. A to L MOV L,A
            state->l = state->a;
        }
        break;
    case 0x70:
        UnimplementedIns(state);
        break;
    case 0x71:
        UnimplementedIns(state);
        break;
    case 0x72:
        UnimplementedIns(state);
        break;
    case 0x73:
        UnimplementedIns(state);
        break;
    case 0x74:
        UnimplementedIns(state);
        break;
    case 0x75:
        UnimplementedIns(state);
        break;
    case 0x76:
        UnimplementedIns(state);
        break;
    case 0x77:
        {
            //Move reg. A to memory (HL) MOV M,A
            uint16_t temp = (state->h << 8) | (state->l);
            state->memory[temp] = state->a;
        }
        break;
    case 0x78:
        UnimplementedIns(state);
        break;
    case 0x79:
        UnimplementedIns(state);
        break;
    case 0x7a:
        UnimplementedIns(state);
        break;
    case 0x7b:
        UnimplementedIns(state);
        break;
    case 0x7c:
        UnimplementedIns(state);
        break;
    case 0x7d:
        UnimplementedIns(state);
        break;
    case 0x7e:
        {
            //move mem. (HL) to reg. A MOV A,M
            uint16_t temp = (state->h << 8) | (state->l);
            state->a = state->memory[temp];
        }
        break;
    case 0x7f:
        UnimplementedIns(state);
        break;
    case 0x80:
        UnimplementedIns(state);
        break;
    case 0x81:
        UnimplementedIns(state);
        break;
    case 0x82:
        UnimplementedIns(state);
        break;
    case 0x83:
        UnimplementedIns(state);
        break;
    case 0x84:
        UnimplementedIns(state);
        break;
    case 0x85:
        UnimplementedIns(state);
        break;
    case 0x86:
        UnimplementedIns(state);
        break;
    case 0x87:
        UnimplementedIns(state);
        break;
    case 0x88:
        UnimplementedIns(state);
        break;
    case 0x89:
        UnimplementedIns(state);
        break;
    case 0x8a:
        UnimplementedIns(state);
        break;
    case 0x8b:
        UnimplementedIns(state);
        break;
    case 0x8c:
        UnimplementedIns(state);
        break;
    case 0x8d:
        UnimplementedIns(state);
        break;
    case 0x8e:
        UnimplementedIns(state);
        break;
    case 0x8f:
        UnimplementedIns(state);
        break;
    case 0x90:
        UnimplementedIns(state);
        break;
    case 0x91:
        UnimplementedIns(state);
        break;
    case 0x92:
        UnimplementedIns(state);
        break;
    case 0x93:
        UnimplementedIns(state);
        break;
    case 0x94:
        UnimplementedIns(state);
        break;
    case 0x95:
        UnimplementedIns(state);
        break;
    case 0x96:
        UnimplementedIns(state);
        break;
    case 0x97:
        UnimplementedIns(state);
        break;
    case 0x98:
        UnimplementedIns(state);
        break;
    case 0x99:
        UnimplementedIns(state);
        break;
    case 0x9a:
        UnimplementedIns(state);
        break;
    case 0x9b:
        UnimplementedIns(state);
        break;
    case 0x9c:
        UnimplementedIns(state);
        break;
    case 0x9d:
        UnimplementedIns(state);
        break;
    case 0x9e:
        UnimplementedIns(state);
        break;
    case 0x9f:
        UnimplementedIns(state);
        break;
    case 0xa0:
        UnimplementedIns(state);
        break;
    case 0xa1:
        UnimplementedIns(state);
        break;
    case 0xa2:
        UnimplementedIns(state);
        break;
    case 0xa3:
        UnimplementedIns(state);
        break;
    case 0xa4:
        UnimplementedIns(state);
        break;
    case 0xa5:
        UnimplementedIns(state);
        break;
    case 0xa6:
        UnimplementedIns(state);
        break;
    case 0xa7:
        {
            //and with reg. A ANA A
            state->a = state->a & state->a;
            state->flags.cy = 0;
            state->flags.ac = 0;
            state->flags.z = (state->a == 0);
            state->flags.p = parity(state->a, 8);
            state->flags.s = (0x80 == (state->a & 0x80));
        }
        break;
    case 0xa8:
        UnimplementedIns(state);
        break;
    case 0xa9:
        UnimplementedIns(state);
        break;
    case 0xaa:
        UnimplementedIns(state);
        break;
    case 0xab:
        UnimplementedIns(state);
        break;
    case 0xac:
        UnimplementedIns(state);
        break;
    case 0xad:
        UnimplementedIns(state);
        break;
    case 0xae:
        UnimplementedIns(state);
        break;
    case 0xaf:
        {
            //exclusiv or with A XRA A
            state->a = state->a ^ state->a;
            state->flags.z = (state->a == 0);
            state->flags.s = (0x80 == (state->a & 0x80));
            state->flags.p = parity(state->a, 8);
            state->flags.cy = state->flags.ac = 0;
        }
        break;
    case 0xb0:
        UnimplementedIns(state);
        break;
    case 0xb1:
        UnimplementedIns(state);
        break;
    case 0xb2:
        UnimplementedIns(state);
        break;
    case 0xb3:
        UnimplementedIns(state);
        break;
    case 0xb4:
        UnimplementedIns(state);
        break;
    case 0xb5:
        UnimplementedIns(state);
        break;
    case 0xb6:
        UnimplementedIns(state);
        break;
    case 0xb7:
        UnimplementedIns(state);
        break;
    case 0xb8:
        UnimplementedIns(state);
        break;
    case 0xb9:
        UnimplementedIns(state);
        break;
    case 0xba:
        UnimplementedIns(state);
        break;
    case 0xbb:
        UnimplementedIns(state);
        break;
    case 0xbc:
        UnimplementedIns(state);
        break;
    case 0xbd:
        UnimplementedIns(state);
        break;
    case 0xbe:
        UnimplementedIns(state);
        break;
    case 0xbf:
        UnimplementedIns(state);
        break;
    case 0xc0:
        UnimplementedIns(state);
        break;
    case 0xc1:
        {
            //POP B
            state->c = state->memory[state->sp];
            state->b = state->memory[state->sp + 1];
            state->sp += 2;
        }
        break;
    case 0xc2:
        {
            //jump on no zero JNZ adr
            if (state->flags.z == 0)
            {
                state->pc = (opcode[2] << 8) | (opcode[1]);
                
            } else {
                state->pc += 2;
            }
        }
        break;
    case 0xc3:
        {
            //Jump addr JMP adr
            state->pc = (opcode[2] << 8) | (opcode[1]);
        }
        break;
    case 0xc4:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xc5:
        {
            //PUSH B
            state->memory[state->sp - 2] = state->c;
            state->memory[state->sp - 1] = state->b;
            state->sp = state->sp - 2;
        }
        break;
    case 0xc6:
        {
            //add A D8 ADI D8
            uint16_t temp = (uint16_t) state->a + (uint16_t) opcode[1];
            state->flags.z = ((temp & 0xff) == 0);
            state->flags.s = ((temp & 0x80) == 0x80);
            state->flags.p = parity((temp & 0xff), 8);
            state->flags.cy = ((temp & 0xff00) > 0);
            //state->flags.ac = state->flags.cy;
            state->a = (uint8_t) temp;
            state->pc++;
        }
        break;
    case 0xc7:
        UnimplementedIns(state);
        break;
    case 0xc8:
        UnimplementedIns(state);
        break;
    case 0xc9:
        {
            //RET
            state->pc = (state->memory[state->sp + 1] << 8) | (state->memory[state->sp]);
            state->sp += 2;
        }
        break;
    case 0xca:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xcb:
        UnimplementedIns(state);
        break;
    case 0xcc:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xcd:
        {
            //CALL adr (may need to do pc+2???)
            state->pc += 2; // pc+=2 b/c push instruction after the og call
            state->memory[state->sp -1] = ((state->pc >> 8) & 0xff);
            state->memory[state->sp - 2] = (state->pc & 0xff);
            state->sp = state->sp - 2;
            state->pc = (opcode[2] << 8) | (opcode[1]);

        }
        break;
    case 0xce:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0xcf:
        UnimplementedIns(state);
        break;
    case 0xd0:
        UnimplementedIns(state);
        break;
    case 0xd1:
        {
            //POP D
            state->e = state->memory[state->sp];
            state->d = state->memory[state->sp + 1];
            state->sp += 2;
        }
        break;
    case 0xd2:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xd3:
        {
            //OUT speical.  Do nothing? Looked at github code but just found an increment for pc.  look at data sheet to fix
            state->pc++;
        }
        break;
    case 0xd4:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xd5:
        {
            //push D
            state->memory[state->sp-2] = state->e;
            state->memory[state->sp-1] = state->d;
            state->sp -= 2;
        }
        break;
    case 0xd6:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0xd7:
        UnimplementedIns(state);
        break;
    case 0xd8:
        UnimplementedIns(state);
        break;
    case 0xd9:
        UnimplementedIns(state);
        break;
    case 0xda:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xdb:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0xdc:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xdd:
        UnimplementedIns(state);
        break;
    case 0xde:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0xdf:
        UnimplementedIns(state);
        break;
    case 0xe0:
        UnimplementedIns(state);
        break;
    case 0xe1:
        {
            //POP H
            state->l = state->memory[state->sp];
            state->h = state->memory[state->sp + 1];
            state->sp += 2;
        }
        break;
    case 0xe2:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xe3:
        UnimplementedIns(state);
        break;
    case 0xe4:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xe5:
        {
            //PUSH H
            state->memory[state->sp - 2] = state->l;
            state->memory[state->sp -1] = state->h;
            state->sp -= 2;
        }
        break;
    case 0xe6:
        {
            //AND immediate with A ANI D8
            state-> a = state->a & opcode[1];
            state->flags.z = (state->a == 0);
            state->flags.s = ((state->a & 0x80) == 0x80);
            state->flags.p = parity(state->a, 8);
            state->flags.cy = state->flags.ac = 0;
            state->pc++;
        }
        break;
    case 0xe7:
        UnimplementedIns(state);
        break;
    case 0xe8:
        UnimplementedIns(state);
        break;
    case 0xe9:
        UnimplementedIns(state);
        break;
    case 0xea:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xeb:
        {
            //exchange DE and HL reg.
            uint8_t temp = state->h;
            state->h = state->d;
            state->d = temp;
            temp = state->l;
            state->l = state->e;
            state->e = temp;
        }
        break;
    case 0xec:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xed:
        UnimplementedIns(state);
        break;
    case 0xee:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0xef:
        UnimplementedIns(state);
        break;
    case 0xf0:
        UnimplementedIns(state);
        break;
    case 0xf1:
        {
            //POP PSW
            state->a = state->memory[state->sp + 1];
            uint8_t psw = state->memory[state->sp];
            state->flags.z = ((psw & 0x01) & 0x01);
            state->flags.s = ((psw & 0x02) & 0x02);
            state->flags.p = ((psw & 0x03) & 0x03);
            state->flags.cy = ((psw & 0x04) & 0x04);
            state->flags.ac = ((psw & 0x05) & 0x05);
            state->sp += 2;

        }
    case 0xf2:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xf3:
        UnimplementedIns(state);
        break;
    case 0xf4:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xf5:
        {
            //PUSH PSW
            state->memory[state->sp - 1] = state->a;
            uint8_t psw = (state->flags.z | state->flags.s << 1 | state->flags.p << 2 | state->flags.cy << 3 | state->flags.ac << 4);
            state->memory[state->sp-2] = psw;
            state->sp -= 2;
        }
        break;
    case 0xf6:
        UnimplementedIns(state);
        bytesUsed = 2;
        break;
    case 0xf7:
        UnimplementedIns(state);
        break;
    case 0xf8:
        UnimplementedIns(state);
        break;
    case 0xf9:
        UnimplementedIns(state);
        break;
    case 0xfa:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xfb:
        UnimplementedIns(state);
        break;
    case 0xfc:
        UnimplementedIns(state);
        bytesUsed = 3;
        break;
    case 0xfd:
        UnimplementedIns(state);
        break;
    case 0xfe:
        {
            //compare immediate with A
            uint8_t temp = state->a - opcode[1];
            state->flags.z = (temp == 0);
            state->flags.s = ((temp & 0x80) == 0x80);
            state->flags.p = parity(temp, 8);
            state->flags.cy = (state->a < opcode[1]);
            //state->flags.ac = ??
            state->pc++;
        }
        break;
    case 0xff:
        UnimplementedIns(state);
        break;

    default:
        UnimplementedIns(state);
    }

    printf("%02x, state{\n"
            "\t\tpc: %d\n"
           "\t\tFlags: z: %d, s: %d, p: %d, cy: %d, ac: %d\n"
           "\t\tregisters: a: %02x, b: %02x, c: %02x, d: %02x, e: %02x, h: %02x, l: %02x sp: %02x\n"
           "\t}\n",
           opcode[0], state->pc, state->flags.z, state->flags.s, state->flags.p, state->flags.cy, state->flags.ac, state->a, state->b, state->c, state->d, state->e, state->h, state->l, state->sp);

    return bytesUsed;
}

int main(void)
{

    State *state = calloc(1, sizeof(State));
    state->memory = malloc(0x10000);
    if (!state->memory)
    {
        printf("could not allocate vRAM\n");
        exit(1);
    }
    LoadBinary(state, "invaders.h", 0);
    LoadBinary(state, "invaders.g", 0x800);
    LoadBinary(state, "invaders.f", 0x1000);
    LoadBinary(state, "invaders.e", 0x1800);
    while (1)
    {
        EmulateCPU(state);
    }
    return 0;
}
