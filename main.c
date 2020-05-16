#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Instructions{
    char* instructionName;
    char* address;
} instructions[] = {
    {"mov", "00001"},
    {"addi", "10000"}
};

struct Labels{
    char* labelName;
    int address;
};

char* findInstruction(char *token){
    // Lower the token character
    char *c = token;
    for(; *c; ++c) *c = (char)tolower(*c);
    // Search the instruction HashMap
    for(int i = 0; i < 2; i++){
        if (strcmp(token, instructions[i].instructionName) == 0){
            return instructions[i].address;
        }
    }
    return "a";
}
void parseLabels(){
    FILE *file_ptr = fopen("code.S", "r");
    FILE *file_ptr_output = fopen("output.txt","w");
    char fileLine[256];
    char *token;
    

    while (fgets(fileLine, sizeof fileLine, file_ptr) != NULL)
    {  
        token = strtok(fileLine, " \t\v\r\n\f,()");
        if (token != NULL){
            printf("%s\n", token);
	    fputs(findInstruction(token),file_ptr_output);
	    
	    
        }
        int label_Flag = 0;
        while (token != NULL)
        {
            token = strtok(NULL, " \t\v\r\n\f,()");
            if (strcmp(token, ";")==0){
                while (token != NULL)
                {
                    token = strtok(NULL, " \t\v\r\n\f,()");
                }
                
            }
            if(token != NULL){
                printf("%s\n", token);
		fputs(token,file_ptr_output);
		
	    }
        }
        printf("\n");
    }
    fclose(file_ptr_output);
}

void parseInstructions()
{
    	FILE *file_ptr = fopen("code.S", "r");
	
    	// char *fileLine = (char)malloc(256* sizeof(char));
    	char fileLine[256];
    	char *token;
    	while (fgets(fileLine, sizeof fileLine, file_ptr) != NULL)
    	{
        	token = strtok(fileLine, " \t\v\r\n\f,():");
		
        	if (!token){
            	continue;
        	}
        	printf("\nMain Token>%s\n", token);
        	if(findInstruction(token))
			
           		printf("Token Found\n");
        	while (token != NULL)
        	{   
            		token = strtok(NULL, "\t\v\r\n\f,()");
			
        	}	
        
        	printf("\n\n");
    	}
    	if (feof(file_ptr)){
        	printf("\n----- End of File -----\n");
    	}
    	fclose(file_ptr);
}

	
	

int main(int argc, char const *argv[])
{
    	// Find Labels in the assembely if any.

    	parseLabels();
	
    	// Parse the Instructions
    	// parseInstructions();
    
    	return 0;
}
