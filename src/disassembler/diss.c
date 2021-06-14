#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int Disasemble(unsigned char *buff, int pc);

int Disassemble(unsigned char *buff, int pc)
{
    unsigned char *opcode = &buff[pc]; /* pointer used for multiple bytes */
    int bytesUsed = 1;
    switch (*opcode)
    {
        case 0x00: printf("NOP"); break;
        case 0x01: printf("LXI\tB,#$%02x%02x", opcode[2], opcode[1]); bytesUsed=3; break;
        case 0x02: printf("STAX\tB"); break;
        case 0x03: printf("INX\tB"); break;
        case 0x04: printf("INR\tB"); break;
        case 0x05: printf("DCR\tB"); break;
        case 0x06: printf("MVI\tB,#$%02x", opcode[1]); bytesUsed=2; break;
        case 0x07: printf("RLC"); break;
        case 0x08: printf("NOP"); break;
        case 0x09: printf("DAD\tB"); break;
        case 0x0a: printf("LDAX\tB"); break;
        case 0x0b: printf("DCX\tB"); break;
        case 0x0c: printf("INR\tC"); break;
        case 0x0d: printf("DCR\t C"); break;
        case 0x0e: printf("MVI\tC,#$%02x", opcode[1]); bytesUsed=2; break;
        case 0x0f: printf("RRC"); break;
        case 0x10: printf("NOP"); break;
        case 0x11: printf("LXI\tD,#$%02x%02x", opcode[2], opcode[1]); bytesUsed=3; break;
        case 0x12: printf("STAX\tD"); break;
        case 0x13: printf("INX\tD"); break;
        case 0x14: printf("INR\tD"); break;
        case 0x15: printf("DCR\tD"); break;
        case 0x16: printf("MVI\tD,#$%02x", opcode[1]); bytesUsed=2; break;
        case 0x17: printf("RAL"); break;
        case 0x18: printf("NOP"); break;
        case 0x19: printf("DAD\tD"); break;
        case 0x1a: printf("LDAX\tD"); break;
        case 0x1b: printf("DCX\tD"); break;
        case 0x1c: printf("INR\tE"); break;
        case 0x1d: printf("DCR\tE"); break;
        case 0x1e: printf("MVI\tE,#$%02x",opcode[1]); bytesUsed=2; break;
        case 0x1f: printf("RAR"); break;
        case 0x20: printf("RIM"); break;
        case 0x21: printf("LXI\tH,#$%02x%02x",opcode[2],opcode[1]); bytesUsed=3; break;
        case 0x22: printf("SHLD\t$%02x%02x",opcode[2],opcode[1]); bytesUsed=3; break;
        case 0x23: printf("INX\tH"); break;
        case 0x24: printf("INR\tH"); break;
        case 0x25: printf("DCR\tH"); break;
        case 0x26: printf("MVI\tH,#$%02x",opcode[1]);bytesUsed=2;break;
        case 0x27: printf("DAA"); break;
        case 0x28: printf("NOP"); break;
        case 0x29: printf("DAD\tH"); break;
        case 0x2a: printf("LHLD\t%02x%02x",opcode[2],opcode[1]);bytesUsed=3;break;
        
        default: printf("not implemented yet");
        printf("\n");
    }
    return bytesUsed;
}

int main(void)
{
    FILE *fp = fopen("../../rom/invaders.h", "r");
    if(!fp)
    {
        printf("Could not open binary file\n");
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        printf("Could not seek end of file\n");
        return -1;
    }

    const int len = ftell(fp);
    printf("length: %d\n", len);
    fseek(fp, 0, SEEK_SET);
    unsigned char buffer[len];
    for (int i = 0; i < len; i++)
    {
        buffer[i] = 0x00;
    }
    fread(buffer, 1, len, fp);
    fclose(fp);
    int pc = 0;
    while(pc < len)
    {
        pc += Disassemble(buffer, pc);
    }
    return 0;
}