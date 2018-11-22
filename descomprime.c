/* 
 * File:   main.c
 * Author: Felipe Luís Pinheiro
 * MAtricula: 2018/0052667
 * Created on 16 de Novembro de 2018, 09:51
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define BITS sizeof(unsigned int) /* numero de bits de cada palavra*/
#define MAX_VALUE (1 << BITS) - 1 /* valor maximo da palabra do tamanho definido */
#define BITS_CODE 32

/**
 * estrutura HUFF -> serve para saber qual o codigo binario para cada caractere do texto.
 */
typedef struct HUFF {
    char code[BITS_CODE];
    int cod;
} HUFF;

/**
 * codigo do livro do deitel
 */
void displayBITS(unsigned int value) {
    unsigned int c; // counter
    unsigned int displayMask = 1 << BITS_CODE - 1;
    for (c = 1; c <= BITS_CODE - 1; ++c) {
        value <<= 1; //shift value left by 1
    }// end for
    putchar(value & displayMask ? '1' : '0');
}

/**
 * codigo do professor - 
 * @param input
 * @return 
 */
unsigned int ler_binario(FILE *input) {
    unsigned int return_value;
    static int input_bit_count = 0;
    static unsigned long input_bit_buffer = 0L;

    while (input_bit_count <= BITS_CODE) {
        input_bit_buffer |=
                (unsigned long) getc(input) << (32 - input_bit_count);
        input_bit_count += 1;
    }
    return_value = input_bit_buffer;
    input_bit_buffer <<= BITS;
    input_bit_count -= BITS;
    return (return_value);
}

/**
 * verifica o tamanho de um string
 * @param str
 * @return 
 */
int stringSIZE(char * str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

/**
 * imprime o buffer total
 * @param str
 */
void printBUFFER(unsigned char * str) {
    int i = 0;
    for (i = 0; i < BITS_CODE; i++) {
        displayBITS(str[i]);
    }
    printf("\n");
}

/**
 * procura uma letra no buffer que  corresponda ao codigo huff.cod, está função ainda precisa ser melhorada. 
 * @param ptr
 * @param huff
 * @param size
 * @return 
 */
char pegaLetra(FILE * ptr, HUFF * huff, int size) {

    static int i = 0; // percorre o buffer
    static unsigned char buffer[BITS_CODE];
    int j, k; // aumento o tamanho da palavra
    char string[BITS_CODE] = "";
    for (j = 0; j < BITS_CODE; j++) {
        if (i == 0) {
            fread(buffer, 1, (unsigned int) BITS_CODE, ptr);
        }
        string[j] = buffer[i];
        string[j+1] = '\0';
        i++;
        if (i == BITS_CODE) {
            i = 0;
        }
        for(k = 0; k < size; k++){
            if(strcmp(string, huff[k].code)==0){
                return huff[k].cod;
            }
        }
    }
    return 0;
}

/*
 * função main
 */
int main(int argc, char** argv) {
    FILE * fileCODE = fopen(argv[1], "r");
    int size;
    int i, out[32];
    fscanf(fileCODE, "%d", &size);
    HUFF * huff;
    huff = (HUFF*) calloc(sizeof (HUFF), size);
    for (i = 0; i < size; i++) {
        fscanf(fileCODE, "%s %d", huff[i].code, &huff[i].cod);
    }
    fclose(fileCODE);
    FILE * fileIN = fopen(argv[2], "rb");
    FILE * fileOUT = fopen("saida.txt", "w");
    char c;
    c = pegaLetra(fileIN, huff, size);
    while (c != -1) {
        fprintf(fileOUT, "%c", c);
//        printf( "%c", c);
        c = pegaLetra(fileIN, huff, size);
    }
    fprintf(fileOUT, "%c", EOF);
    fclose(fileOUT);
    fclose(fileIN);
    return 0;
}

