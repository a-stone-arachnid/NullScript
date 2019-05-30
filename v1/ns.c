// NullScript version 1.0.0
#include <stdio.h>	// i/o capabilities (printf, scanf)
#include <string.h> // string functions (strlen)

#define tcell cells[cellPtr]
#define tcmd  cmds[cpos]

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

// Helper function for { loops }
void lLoop(){
	if(tcell != 0) return;
	for(;cpos < strlen(cmds);cpos++)
	{
		if(tcmd == '}') return;
	}
}
void rLoop(){
	for(;cpos > 0;cpos--)
	{
		if(tcmd == '{') 
		{
			cpos--;
			return;
		}
	}
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
			printf("Input <int>: ");
			scanf("%i", &tcell);
			getchar();
			break;
		case '^':
			printf("Input <char>: ");
			tcell = getchar();
			getchar();
			break;
		case '{':
			lLoop();
			break;
		case '}':
			rLoop();
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

// Version mode
int version()
{
    printf("NullScript version 1.0.0\nCopyright (c) 2019 Ethan Nord.");
    return 0;
}
// Interactive mode
int interactive()
{
    printf("NullScript version 1.0.0\n");
	ns_init();
	while(1)
	{
		printf("\nNS> ");
		char si[256];
		fgets(si, sizeof(si), stdin);
		if(!exec(si)) return 0;
		printf("\n");
	}
    return 0;
}
// Evil mode
int evaluate(char*si)
{
    ns_init();
    exec(si);
    printf("\n");
    return 0;
}
// Help mode
int help()
{
	version();
	printf("\nUsage:\nNS\nNS code\nNS -v");
	return 0;
}

// Main
int main(int argc, char** argv)
{
    char mode = 0;
    // constants for versioning to make strcmp happy
    const char *ver = "--version";
    const char *v = "-v";
    
    if(argc == 1) return interactive();
    else if(argc == 2 && !strcmp(argv[1], ver)) return version();
    else if(argc == 2 && !strcmp(argv[1], v)) return version();
    else if(argc == 2) return evaluate(argv[1]);
	else return help();
}
