#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Instructions
{
    char *instructionName;
    int  address;
    char type;
} instructions[] = {
    {"mov", 00001, 'R'},
    {"addi", 10000, 'R'},
    {"j",000010, 'J'}
    };

struct Labels
{
    char labelName[32];
    int address;
} labels[256];

int findLabelAddress(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 2; i++)
    {
        if (strcmp(token, labels[i].labelName) == 0)
        {
            return labels[i].address;
        }
    }
    return -1;
}

char findInstructionType(char *token){
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].type;
        }
    }
    return 'NOP';
}

char *findInstruction(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].address;
        }
    }
    return "";
}
void parseInstructions()
{
    FILE *file_ptr = fopen("code.S", "r");
    FILE *file_ptr_output = fopen("output.txt", "w");
    char fileLine[256];
    char *token, type;
    int new_address;
    uint32_t instr_address;

    while (fgets(fileLine, sizeof fileLine, file_ptr) != NULL)
    {   
        instr_address = 0x0;
        new_address  = 0x0;
        // Extract first token on each line
        token = strtok(fileLine, " \t\v\r\n\f,()");

        // If line is blank skip ahead
        if (token != NULL)
        {
            printf("Token parsed: %s\n", token);
            // fputs(findInstruction(token), file_ptr_output);

            // If the token is a label find its address
            if (token[0] == ':')
            {
                printf("Label %s address: %d\n", token, findLabelAddress(token));
                continue;
            }

            //Check if it is instruction
            printf("Ttype: %s  ", token);
            type = findInstructionType(token);
            if (type == 'R'){
                printf("%c", type, token);
            }
            else if (type == 'I')
            {
                /* code */
            }
            else
            {
                char *jump_address = strtok(NULL," ");
                jump_address = strtok(jump_address, "\n");

                // If it is an immediate value
                if(jump_address[0] == '#'){
                    jump_address = strtok(jump_address, "#");
                    new_address = atoi(jump_address);
                }

                // If it is a label
                else
                {
                    new_address = findLabelAddress(jump_address) * 4;
                }
                uint8_t opcode = findInstruction(token);
                instr_address |= ((opcode& 0xff) << 28);
                instr_address |= new_address & 0x3FFFFFF;
                printf("\n%0x", instr_address);
            }
            
        }
        printf("\n\n");
    }
    fclose(file_ptr_output);
}

void parseLabels()
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
    fclose(file_ptr);
}

int main(int argc, char const *argv[])
{
    // Find Labels in the assembely if any.

    parseLabels();
    for (int i = 0; i < 3; i++)
        printf("Label %s -> address: %d\n", labels[i].labelName, labels[i].address);
    // Parse the Instructions
    parseInstructions();

    return 0;
}
