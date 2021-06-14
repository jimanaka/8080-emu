#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int Dissasemble(unsigned char *buff, int pc);

int Disassemble(unsigned char *buff, int pc)
{
    unsigned char opcode = buff[pc];
    switch (opcode)
    {
        case 0x00: printf("NOP\n"); break;
        default: printf("not implemented yet\n");
    }
    return 0;
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
    for(int pc = 0; pc < len; pc++)
    {
        Disassemble(buffer, pc);
    }
    return 0;
}