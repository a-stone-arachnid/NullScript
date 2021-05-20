#include "nscmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

NS_CMD code[4096];
int16_t mem[512], p, ns_errno, nr;

#define ccell mem[p]

int MODE, codelen;
// int labels[4096], lnum;

#define M_INTERACTIVE   1 << 1
#define M_COMPRESSED    1 << 2
#define M_DEBUG         1 << 3

void version() {
    puts("NullScript version 2.2.0");
}

void usage(char* name) {
    // TODO: Make operation actually match usage
    // TODO: Eval mode - like `ns -e "]]];."`
    printf("usage: %s [-cdi] codefile inputfile", name);
    puts("");
    puts(" -?\tDisplay this help message");
    puts(" -c\tOpen file in compressed mode");
    puts(" -d\tEnable debug mode");
    puts(" -i\tOpen NullScript in REPL mode");
    puts(" -v\tDisplay version number");
}

void welcome()
{
    version();
}

void loadfile(FILE *file) {
    int i;
    for(i = 0; i < 4096; code[i++] = 0);
    for(i = 0; i < 4096; i++) {
        if((code[i] = fgetc(file)) < 0) break;
    }
    codelen = i;
    if(ferror(file) != 0) {
        fputs("Unknown error occured", stderr);
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

int parseCode(int, NS_CMD, int*);

void debug_out(NS_CMD i)
{
    printf("\n=== ccell = %d; nr = %d; p = %d; ===",
           ccell, nr, p);
    printf("\nEXECUTING COMMAND: %c\n", i);
    getchar();
}

int nsshell() {
    int i, r;
    do {
        printf("\n>>> ");
        fgets(code, 4095, stdin);
        codelen = strlen(code);
        for(i = 0, r = 1; i < codelen; i++) {
            if(MODE & M_DEBUG) debug_out(code[i]);
            r=parseCode(0, code[i], &i);
            if(r == 0) return 0;
        }
    } while(1);
    return 0;
}

int main(int argc, char** argv) {
    FILE *infile;
    int i, fnp = 0;
    
    nr = 1;
    srand(time(NULL));
    if(argc < 2) {
        welcome();
        MODE |= M_INTERACTIVE;
    }
    for(i = 1; i < argc; i++) {
        // TODO: New argument parser that takes "ns -icd"-style arguments
        if(strcmp(argv[i], "-i") == 0) MODE |= M_INTERACTIVE;
        else if(strcmp(argv[i], "-c") == 0) MODE |= M_COMPRESSED;
        else if(strcmp(argv[i], "-d") == 0) MODE |= M_DEBUG;
        else if(strcmp(argv[i], "-v") == 0) {
            version();
            exit(EXIT_SUCCESS);
        }
        else if(strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        }
        else if(argv[i][0] == '-') {
            fprintf(stderr, "\n%s: invalid option provided: %s\n", argv[0], argv[i]);
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
        else {
            fnp = i;
            break;
        }
    }
    if(MODE & M_INTERACTIVE) {
        return nsshell();
    }
    if(fnp == 0 && !(MODE & M_INTERACTIVE)) {
        fprintf(stderr, "%s: no input file provided\n", argv[0]);
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    
    infile = fopen(argv[fnp], "r");
    if(infile == 0) {
        fprintf(stderr, "\n%s: file `%s' not found\n", argv[0], argv[fnp]);
        exit(EXIT_FAILURE);
    }
    
    loadfile(infile);
    fclose(infile);
    
    for(i = 0; i < codelen; i++) {
        if(MODE & M_DEBUG) debug_out(code[i]);
        if(parseCode(MODE & M_COMPRESSED, code[i], &i) == 0) return 0;
    }
    return 0;
}

void moveLeft() {
    if(p <= 0) p = 4095;
    else p--;
}
void moveRight() {
    if(p >= 4095) p = 0;
    else p++;
}

// TODO: Add support for input from any file.
void inputNumber() {
    do printf("# ");
       while(scanf("%hd", &ccell) != 1);
}

void inputChar() {
    printf("% ");
    ccell = getchar();
}

void inputString() {
    for(int x = 1; (x = fread(&ccell, 1, 1, stdin)); moveRight());
    do moveLeft(); while(ccell != 0);
}

NS_CMD parseDigit(int d) {
    if(d == 0 && nr == 1) nr = 0;
    else if(d == 0) nr = 97;
    else if(d == 1 && nr == 1) nr = 32;
    else if(d == 1) nr = 1;
    else if(nr == 1) nr = d;
    else nr = nr * d - 1;
    if(nr < 0) nr = 1;
    return K_NOOP_M;
}

NS_CMD translateCode(NS_CMD c) {
    if(isdigit(c))
        return parseDigit(c - '0');
    switch(c) {
        case K_ZERO: return K_ZERO_M;
        case K_SQRE: return K_SQRE_M;
        case K_DECR: return K_DECR_M;
        case K_INCR: return K_INCR_M;
        case K_PUTD: return K_PUTD_M;
        case K_PUTC: return K_PUTC_M;
        case K_GETD: return K_GETD_M;
        case K_GETC: return K_GETC_M;
        case K_LABL: return K_LABL_M;
        case K_RAND: return K_RAND_M;
        case K_JMPZ: return K_JMPZ_M;
        case K_JPNZ: return K_JPNZ_M;
        case K_MOVL: return K_MOVL_M;
        case K_MOVR: return K_MOVR_M;
        case K_COMP: return K_COMP_M;
        case K_ERRO: return K_ERRO_M;
        default: return K_NOOP_M;
    }
}

int parseMicroCode(NS_CMD c, int* ci) {
    int e;
    switch(c) {
        case K_ZERO_M:
            ccell = 0;
            break;
        case K_SQRE_M:
            if(nr == 1) ccell = ccell * ccell;
            else if(nr == 0) ccell = ccell / 2;
            else ccell = ccell / nr;
            break;
        case K_DECR_M:
            if(nr == 0) nr = 10;
            ccell -= nr;
            break;
        case K_INCR_M:
            if(nr == 0) nr = 10;
            ccell += nr;
            break;
        case K_PUTD_M:
            if(nr == 0) nr = 10;
            for(int i = nr; i --> 0; )
            printf("%d", ccell);
            break;
        case K_PUTC_M:
            if(nr == 0) {
                while(ccell) { putchar(ccell); moveLeft(); }
            }
            else {
                for(int i = nr; i --> 0; )
                    putchar(ccell);
            }
            break;
        case K_GETD_M:
            inputNumber();
            break;
        case K_GETC_M:
            if(nr == 0) inputString();
            else inputChar();
            break;
        case K_LABL_M:
            break;
        case K_RAND_M:
            if(ccell != 0)
                ccell = rand() % ccell;
            else
                ccell = rand() % INT16_MAX;
            break;
        case K_JMPZ_M:
            if(ccell == 0 && nr == 0) *ci = codelen - 1;
            else if(ccell == 0 && nr == 1) {
                for(e=*ci; e ++< codelen && translateCode(code[e]) != K_LABL_M;);
                *ci = e;
            }
            else if(ccell == 0) {
                for(e=*ci; e++ < codelen && nr --> 0; );
                *ci = e;
            }
            break;
        case K_JPNZ_M:
            if(ccell != 0 && nr == 0) *ci = 0;
            else if(ccell != 0 && nr == 1) {
                for(e=*ci; e --> 0 && translateCode(code[e]) != K_LABL_M;);
                *ci = e;
            }
            else if(ccell != 0) {
                for(e=*ci; nr --> 0 && e --> 0;);
                *ci = e;
            }
            break;
        case K_MOVL_M:
            if(nr == 0) while(ccell != 0) moveLeft();
            else while(nr --> 0) moveLeft();
            break;
        case K_MOVR_M:
            if(nr == 0) while(ccell != 0) moveRight();
            else while(nr --> 0) moveRight();
            break;
        case K_COMP_M:
        {
            int i, j;
            i = ccell;
            moveRight();
            j = ccell;
            moveRight();
            if(i == j) ccell = 0;
            else if(i < j) ccell = -1;
            else if(i > j) ccell = 1;
        }
            break;
        case K_ERRO_M:
            ns_errno = ccell;
            return 0;
        default: break;
    }
    if(c != K_NOOP_M) nr = 1;
    return 1;
}

int parseCode(int d, NS_CMD c, int* ci) {
    if(d)
        return parseMicroCode(HI4BIT(c), 0) && parseMicroCode(LO4BIT(c), 0);
    else
        return parseMicroCode(translateCode(c), ci);
}
