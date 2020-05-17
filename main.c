#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Instructions
{
    char *instructionName;
    int address;
    char type;
    int sh;
    int func;
} instructions[] = {
    {"and", 0x0, 'R', 0x0, 0x24},
    {"add", 0x0, 'R', 0x0, 0x20},
    {"lw", 0x23, 'I', 0x0, 0x0},
    {"addi", 0x8, 'I', 0x0, 0x0},
    {"j", 0x2, 'J', 0x0, 0x0}
};

struct Labels
{
    char labelName[32];
    int address;
} labels[256];

// Find the label and return its address
int findLabelAddress(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(token, labels[i].labelName) == 0)
        {
            return labels[i].address;
        }
    }
    return -1;
}

// Find the instruction and return its Shamt
int findShamt(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].sh;
        }
    }
    return -1;
}

// Find the instruction and return its Funtion value
int findFunc(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].func;
        }
    }
    return -1;
}

// Find the instruction and return its type
char findInstructionType(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].type;
        }
    }
    return "NOP";
}

// Find the instruction and return its address
char *findInstruction(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].address;
        }
    }
    return "";
}

// Parse the instructions and convert it to corresponding hex address
void parseInstructions(char *sourceFile, char *outputFile)
{
    FILE *file_ptr = fopen("code.S", "r");
    FILE *file_ptr_output = fopen("output.txt", "w");
    char fileLine[256];
    char *token, type, fileWrite[32];
    int new_address;
    uint32_t instr_address;

    while (fgets(fileLine, sizeof fileLine, file_ptr) != NULL)
    {
        instr_address = 0x00000000;
        new_address = 0x0;
        // Extract first token on each line
        token = strtok(fileLine, " \t\v\r\n\f,()");

        // If line is blank skip ahead
        if (token != NULL)
        {
            printf("Token parsed: %s\n", token);

            // If the token is a label find its address
            if (token[0] == ':')
            {
                // printf("Label %s address: %d\n", token, findLabelAddress(token));
                continue;
            }

            //Check if it is instruction
            // printf("Ttype: %s  ", token);
            type = findInstructionType(token);
            // printf("t: %c\n", type);
            uint8_t opcode = findInstruction(token);
            opcode = opcode;
            if (type == 'R')
            {
                char *dest = strtok(NULL, " ,$");
                char *source1 = strtok(NULL, " ,$");
                char *source2 = strtok(NULL, " ,$");
                // printf("opcode: %d OP1: %s OP2: %s Dest: %s", opcode, source1, source2, dest);
                instr_address |= ((opcode & 0x3f) << 26);
                instr_address |= ((atoi(source1) & 0x1f) << 21);
                instr_address |= ((atoi(source2) & 0x1f) << 16);
                instr_address |= ((atoi(dest) & 0x1f) << 11);
                instr_address |= ((findShamt(token) & 0x1f) << 6);
                instr_address |= ((findFunc(token) & 0x3F));
            }
            else if (type == 'I')
            {
                char *dest = strtok(NULL, " ,$");
                char *source1 = strtok(NULL, " ,$");
                char *immediate = strtok(NULL, " ,");
                immediate = strtok(immediate, "#()");
                // printf("opcode: %d OP1: %s imm: %s Dest: %s", opcode, source1, immediate, dest);
                instr_address |= ((opcode & 0x3f) << 26);
                instr_address |= ((atoi(source1) & 0x1f) << 21);
                instr_address |= ((atoi(dest) & 0x1f) << 16);
                instr_address |= (atoi(immediate) & 0xffff);
                // 100000 00000 00001 000000000001100
            }
            else
            {
                char *jump_address = strtok(NULL, " ");
                jump_address = strtok(jump_address, "\n");

                // If it is an immediate value
                if (jump_address[0] == '#')
                {
                    jump_address = strtok(jump_address, "#");
                    new_address = atoi(jump_address);
                }

                // If it is a label
                else
                {
                    new_address = findLabelAddress(jump_address) * 4;
                }
                instr_address |= ((opcode & 0xff) << 28);
                instr_address |= new_address & 0x3FFFFFF;
            }
            // printf("\n%0x", instr_address);
            sprintf(fileWrite, "0x%0x\n", instr_address);
            fputs(fileWrite, file_ptr_output);
        }
        // printf("\n\n");
    }
    fclose(file_ptr_output);
}

// Parse Labels and add them to a structure
void parseLabels(char *sourceFile, char *outputFile)
{
    FILE *file_ptr = fopen("code.S", "r");

    int address = 0, labelcounter = 0;
    char fileLine[256];
    char *token, *label;
    while (fgets(fileLine, sizeof fileLine, file_ptr) != NULL)
    {
        token = strtok(fileLine, " \t\v\r\f,()");
        if (token[0] == ':')
        {
            token = strtok(token, ":");
            token = strtok(token, "\n");
            strcpy(labels[labelcounter].labelName, token);
            labels[labelcounter++].address = address;
        }
        address++;
    }
    printf("Labels found: %d\n", labelcounter);
    fclose(file_ptr);
}

int main(int argc, char const *argv[])
{
    if (argc<3){
        printf("Error: Enter source and output file name\n");
        exit(0);
    }

    // Find Labels in the assembely if any.
    printf("Parsing Labels...\n");
    parseLabels( argv[1] , argv[2]);
    // for (int i = 0; i < 3; i++)
    //     printf("Label %s -> address: %d\n", labels[i].labelName, labels[i].address);
    // Parse the Instructions
    printf("Building instructions...\n");
    parseInstructions(argv[1] , argv[2]);
    printf("Finished parsing %s generated.\n", argv[2]);
    return 0;
}
