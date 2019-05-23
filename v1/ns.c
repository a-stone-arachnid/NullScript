// NullScript version 1.0.0
#include <stdio.h>	// i/o capabilities (printf, scanf)
#include <string.h> // string functions (strlen)

#define tcell cells[cellPtr]

// Stores the cells of data, and where we are.
int cells[256];
int cellPtr;

// Stores the commands and the command that we're currently on.
char cmds[256];
size_t cpos;

// Initializer for the globals
void ns_init() {
	for(int i=0;i<256;i++) cells[i]=0;
	cellPtr = 0;
}

// Execution loop - does commands
int exec(char* in)
{
	strcpy(cmds, in);
	for(cpos = 0; cpos < strlen(cmds); cpos++)
	{
		switch(cmds[cpos])
		{
		case ']':
			tcell++;
			break;
		case '[':
			tcell--;
			break;
		case ';':
			tcell *= tcell;
			break;
		case '.':
			printf("%d", tcell);
			break;
		case ',':
			putchar(tcell);
			break;
		case '~':
			tcell = 0;
			break;
		case '<':
			cellPtr == 0 ? cellPtr = 255 : cellPtr--;
			break;
		case '>':
			cellPtr == 255 ? cellPtr = 0 : cellPtr++;
			break;
		case '&':
			printf("Input: ");
			scanf("%i", &tcell);
			break;
		case 'Q':
		case 'q':
			return 0;
		default:
			break;
		}
	}
	return 1;
}

// Main
int main(int argc, char** argv)
{
	printf("NullScript version 1.0.0\n");
	ns_init();
	while(1)
	{
		printf("\nNS> ");
		char si[256];
		fgets(si, sizeof(si), stdin);
		if(!exec(si)) break;
		printf("\n");
	}
}
