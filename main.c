#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Instructions
{
    char *instructionName;
    char *address;
} instructions[] = {
    {"mov", "00001"},
    {"addi", "10000"}};

struct Labels
{
    char labelName[32];
    int address;
} labels[256];

int *findLabelAddress(char *token)
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

char *findInstruction(char *token)
{
    // Lower the token character
    char *c = token;
    for (; *c; ++c)
        *c = (char)tolower(*c);
    // Search the instruction HashMap
    for (int i = 0; i < 2; i++)
    {
        if (strcmp(token, instructions[i].instructionName) == 0)
        {
            return instructions[i].address;
        }
    }
    return NULL;
}
void parseInstructions()
{
    FILE *file_ptr = fopen("code.S", "r");
    FILE *file_ptr_output = fopen("output.txt", "w");
    char fileLine[256];
    char *token;

    while (fgets(fileLine, sizeof fileLine, file_ptr) != NULL)
    {
        // Extract first token on each line
        token = strtok(fileLine, " \t\v\r\n\f,()");

        // If line is blank skip ahead
        if (token != NULL)
        {
            printf("gggg%s\n", token);
            fputs(findInstruction(token), file_ptr_output);

            // If the token is a label find its address
            if (token[0] == ':')
            {
                printf("%s %d", token, findLabelAddress(token));
            }

            token = strtok(NULL, " \t\v\r\n\f,()");
            // Remove comment tokens
            if (strcmp(token, ";") == 0)
            {
                while (token != NULL)
                {
                    token = strtok(NULL, " \t\v\r\n\f,()");
                }
            }
            if (token != NULL)
            {
                printf("%s\n", token);
                fputs(token, file_ptr_output);
            }

            // while (token != NULL)
            // {
            //     token = strtok(NULL, " \t\v\r\n\f,()");
            //     // Remove comment tokens
            //     if (strcmp(token, ";") == 0)
            //     {
            //         while (token != NULL)
            //         {
            //             token = strtok(NULL, " \t\v\r\n\f,()");
            //         }
            //     }
            //     if (token != NULL)
            //     {
            //         printf("%s\n", token);
            //         fputs(token, file_ptr_output);
            //     }
            // }
        }
        printf("\n");
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
