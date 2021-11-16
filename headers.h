#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct symbol {
	int	DefinedOnSourceLine;
	int	Address; 
	char	Name[7]; };

typedef struct symbols	SYMBOL;

struct symbol* SymbolTable[ 1024 ];

struct opcodes {
	char	OpCode;	
	char	Name[8];

};

typedef struct opcodes OPCODES;


OPCODES OpcodeTable[ 32 ];

int IsAValidSymbol( char *TestSymbol );
int IsADirective( char *Test );
int IsAnInstruction( char *Test );
int updateLocation(char *dirInst, char* tokThird, int currCount, char fullLine[], int lineNumber);
void addSymbol(struct symbol* Tab[], int addr, int src, char* symName);
int symbolExists(struct symbol* Tab[], char* sName);
char getOpcode(char* nem);

//class code/notes
/*
int index;
index = 0;
struct symbol newSym;
newSym = malloc()sizeof(struct symbol);
newSym.address = addr, newSym.sourceLine =src, strcpy(newSym.name, sName);
while(Tab[index] != NULL){
	index++;
}
Tab[index] = newSym;
}

int //returns zero or non zero// SymbolExists(struct Symbol* tab[], char* sName){
int Result = 0;
int  index = 0;
while(Tab[index] != NULL){
	if(strcmp(sName, Tab[index].name) == 0){
		Result = -1;
		break;
	};
	index++;
}

return result;
}
*/
