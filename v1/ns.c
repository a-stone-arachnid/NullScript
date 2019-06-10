// NullScript version 1.2.0
#include <stdio.h>  // i/o capabilities (printf, scanf)
#include <string.h> // string functions (strlen)
#include <time.h>   // random header file
#include <stdlib.h> // also random header file

#define tcell cells[cellPtr]
#define tcmd  cmds[cpos]

typedef struct queue {
    int items[8];
    int itemCount;
    int front;
    int rear;
    int maxItems;
} queue;

// Stores the cells of data, and where we are.
int cells[256];
int cellPtr;

// Parameter queue
queue pq;

// Stores the commands and the command that we're currently on.
char cmds[256];
size_t cpos;

// strmem
char strmem[128];

// Initializer
void ns_init() {
    for(int i=0;i<256;i++) cells[i]=0;
    cellPtr = 0;
    pq.front = 0;
    pq.rear = -1;
    pq.itemCount = 0;
    pq.maxItems = 8;
    srand(time(NULL));
}

// Helper function for { loops }
void lLoop(){
    if(tcell != 0) return;
    for(;cpos < strlen(cmds);cpos++)
        if(tcmd == '}') return;
}
void rLoop(){
    for(;cpos > 0;cpos--)
        if(tcmd == '{') 
        {
            cpos--;
            return;
        }
}

int pq_get()
{
    int data;
    if(pq.itemCount == 0){
        printf("Input (int): ");
        scanf("%d", &data);
    } else {
        data = pq.items[pq.front++];
        if(pq.front == pq.maxItems)
            pq.front = 0;
        pq.itemCount--;
    }
    return data;  
}

void pq_set(int value)
{
    if(pq.itemCount != pq.maxItems)
    {
        if(pq.rear == pq.maxItems-1) pq.rear = -1;
        pq.items[++pq.rear] = value;
        pq.itemCount++;
    }
}

// return a random number between 0 and TCELL inclusive.
// adapted from https://stackoverflow.com/a/2999130/8828658
int random() {
    int r, divisor = RAND_MAX / (tcell + 1);
    do { r = rand() / divisor; } while (r > tcell);
    return r;
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
        case '*':
            pq_set(tcell);
            break;
        case '\'':
            tcell = pq_get();
            break;
        case '+':
            tcell = pq_get() + pq_get();
            break;
        case '-':
            tcell = pq_get() - pq_get();
            break;
        case 'x':
            tcell = pq_get() * pq_get();
            break;
        case '/':
            tcell = pq_get() / pq_get();
            break;
        case '"':
            tcell = pq_get() % pq_get();
            break;
        case '!':
            fprintf(stderr, "FATAL ERROR!!!");
            return 0;
        case '?':
            tcell = random();
            break;
        case '$':
            printf("Input (string): ");
            fgets(strmem, sizeof(strmem), stdin);
            break;
        case '#':
            printf("%s", strmem);
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
    printf("NullScript version 1.3.0\n");
    return 0;
}
// Interactive mode
int interactive()
{
    version();
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
    return 0;
}
// Help mode
int help()
{
    version();
    printf("Read the source code!\n");
    return 0;
}

// Main
int main(int argc, char** argv)
{
    // constants for versioning to make strcmp happy
    const char *ver = "--version";
    const char *v = "-v";
    // cool stuff
    if(argc == 1) return interactive();
    else if(argc == 2 && !strcmp(argv[1], ver)) return version();
    else if(argc == 2 && !strcmp(argv[1], v)) return version();
    else if(argc == 2) return evaluate(argv[1]);
    else return help();
}
