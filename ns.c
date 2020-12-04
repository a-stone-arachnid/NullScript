#include "nscmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

NS_CMD code[4096];
int16_t mem[512], p, ns_errno;

#define ccell mem[p]

int MODE, codelen;
// int labels[4096], lnum;

#define M_INTERACTIVE   1 << 1
#define M_COMPRESSED    1 << 2

void usage() {
    puts("usage: ns6 [-i | -c] filename");
}

void loadfile(FILE *file) {
    int i;
    for(i = 0; i < 4096; code[i++] = 0);
    for(i = 0; i < 4096; i++) {
        code[i] = fgetc(file);
        if(feof(file) != 0) break;
    }
    codelen = i;
    if(ferror(file) != 0) {
        fputs("Unknown error occured", stderr);
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

int parseCode(int, NS_CMD, int*);

int nsshell() {
    int i,r;
    
    printf("\n>>> ");
    fgets(code, 4095, stdin);
    while(1) {
        for(i = 0, r = 1; i < strlen(code); i++) {
            r=parseCode(0, i[code], &i);
            if(r == 0) return 0;
        }
        printf("\n>>> ");
        fgets(code, 4095, stdin);
    }
    return 0;
}

int main(int argc, char** argv) {
    FILE *infile;
    int i, fnp = 0;
    if(argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    }
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0) MODE |= M_INTERACTIVE;
        else if(strcmp(argv[i], "-c") == 0) MODE |= M_COMPRESSED;
        else if(argv[i][0] == '-') {
            usage();
            fprintf(stderr, "\nInvalid option provided: %s\n", argv[i]);
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
        fputs("No input file provided\n", stderr);
        usage();
        exit(EXIT_FAILURE);
    }
    
    infile = fopen(argv[fnp], "r");
    if(infile == 0) {
        fprintf(stderr, "\nFile `%s' not found\n", argv[fnp]);
        exit(EXIT_FAILURE);
    }
    
    loadfile(infile);
    fclose(infile);
    
    for(i = 0; i < codelen; i++) {
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

NS_CMD translateCode(NS_CMD c) {
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
        case K_JUMP: return K_JUMP_M;
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
            ccell = ccell * ccell;
            break;
        case K_DECR_M:
            ccell--;
            break;
        case K_INCR_M:
            ccell++;
            break;
        case K_PUTD_M:
            printf("%d ", ccell);
            break;
        case K_PUTC_M:
            putchar(ccell);
            break;
        case K_GETD_M:
            do {
                printf("Enter a number: ");
                e = scanf("%hd", &ccell);
            } while(e == EOF);
            break;
        case K_GETC_M:
            printf("Enter a character: ");
            ccell = getchar();
            break;
        case K_LABL_M:
            break;
        case K_JUMP_M:
            for(e=*ci; e --> 0 && translateCode(code[e]) != K_LABL_M;);
            *ci = e;
            break;
        case K_JMPZ_M:
            if(ccell == 0) {
                for(e=*ci; e --> 0 && translateCode(code[e]) != K_LABL_M;);
                *ci = e;
            }
            break;
        case K_JPNZ_M:
            if(ccell != 0) {
                for(e=*ci; e --> 0 && translateCode(code[e]) != K_LABL_M;);
                *ci = e;
            }
            break;
        case K_MOVL_M:
            moveLeft();
            break;
        case K_MOVR_M:
            moveRight();
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
    return 1;
}

int parseCode(int d, NS_CMD c, int* ci) {
    if(d)
        return parseMicroCode(HI4BIT(c), 0) && parseMicroCode(LO4BIT(c), 0);
    else
        return parseMicroCode(translateCode(c), ci);
}
