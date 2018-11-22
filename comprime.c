/* 
 * File:   main.c
 * Author: Felipe Luís Pinheiro
 * Matricula: 2018/0052667
 * Created on 6 de Novembro de 2018, 11:13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <locale.h>
#define BITS sizeof(unsigned int) /* numero de bits de cada palavra*/
#define MAX_VALUE (1 << BITS) - 1 /* valor maximo da palabra do tamanho definido */
#define ASCII_CODE 260

/**
 * Estrtura ASCII - no momento se tornou desnecessária, foi usada na primeira 
 * versão do código, mas acabou ficando para não modificar muito a estrutura 
 * por falta de tempo.
 */
typedef struct ASCII {
    char cod;
    int quant;
} ASCII;

/**
 * Estrutura TREE - é uma arvore binária que serve para montar a arvore de huffman
 */
typedef struct TREE {
    int quant;
    int cod;
    struct TREE * left;
    struct TREE * right;
} TREE;

/**
 * estrutura nodeLISTA - estrutura do nó da lista para criação da arvore de huffman
 */
typedef struct nodeLISTA {
    struct nodeLISTA * next;
    TREE * tree;
} nodeLISTA;

/**
 * estrutura LISTA - serve para armazenar as sub arvores de huffman enquanto 
 * que o sistema nào termina de rodar
 */
typedef struct LISTA {
    nodeLISTA * inicio;
    int tamanho;
} LISTA;

/**
 * funcao criaFOLHA cria uma folha da arvore de huffman
 * @param cod
 * @param quant
 * @return ponteiro de estrutura TREE
 */
TREE * criaFOLHA(char cod, int quant) {
    TREE * tree = (TREE*) malloc(sizeof (TREE));
    if (tree == NULL) {
        fprintf(stderr, "ERRO: falta de memoria\n");
        return NULL;
    }
    tree->cod = cod;
    tree->quant = quant;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

/**
 * funçao criaTREE usando as folhas criadas na função criaFOLHA essa funcoa 
 * cria as subarvores
 * @param left
 * @param right
 * @return ponteiro de TREE
 */
TREE * criaTREE(TREE * left, TREE * right) {
    TREE * tree = (TREE*) malloc(sizeof (TREE));
    if (tree == NULL) {
        fprintf(stderr, "ERRO: falta de memoria\n");
        return NULL;
    }
    tree->quant = 0;
    tree->cod = 0;
    if (right != NULL) {
        tree->quant += right->quant;
    }
    if (left != NULL) {
        tree->quant += left->quant;
    }
    tree->left = left;
    tree->right = right;
    return tree;
}

/**
 * inseri - adiciona um elemento na lista, que na verdade funciona quase como  uma fila. 
 * @param lista
 * @param cod
 * @param quant
 * @return 
 */
int inseri(LISTA * lista, char cod, int quant) {
    nodeLISTA * novo;
    novo = (nodeLISTA*) malloc(sizeof (nodeLISTA));
    if (novo == NULL) {
        fprintf(stderr, "ERRO: falta memoria");
        return 0;
    }
    novo->tree = criaFOLHA(cod, quant);
    novo->next = NULL;
    lista->tamanho++;
    if (lista->inicio == NULL) {
        lista->inicio = novo;
        lista->inicio->next = NULL;
        return 1;
    }
    nodeLISTA * aux;
    aux = lista->inicio;
    while (aux->next != NULL) {
        aux = aux->next;
    }
    aux->next = novo;
    return 1;
}

/**
 * cria a lista vazia
 * @param lista
 * @param ascii
 */
void criaLISTA(LISTA * lista, ASCII * ascii) {
    lista->inicio = NULL;
    lista->tamanho = 0;
    int i;
    for (i = 0; i < ASCII_CODE; i++) {
        if (ascii[i].quant != 0) {
            inseri(lista, ascii[i].cod, ascii[i].quant);
        }
    }
}

/**
 * ordena a estrutura ASCII - perdeu o uso com a implementação
 * @param ascii
 * @return 
 */
int ordenar(ASCII * ascii) {
    int i;
    int j;
    ASCII temp;
    for (i = 0; i < ASCII_CODE; i++) {
        for (j = 0; j < ASCII_CODE; j++) {
            if (ascii[i].quant < ascii[j].quant) {
                temp = ascii[i];
                ascii[i] = ascii[j];
                ascii[j] = temp;
            }
        }
    }
    return 1;
}

/**
 * imprime ASCII - perdeu o uso com a implementação
 * @param ascii
 */
void printASCII(ASCII * ascii) {
    int cont;
    printf("contagem de letras do texto\n");
    for (cont = 0; cont < ASCII_CODE; cont++) {
        if (ascii[cont].quant > 0) {
            if (ascii[cont].cod == '\n') {
                printf("CR %d\n", ascii[cont].quant);
            } else if (ascii[cont].cod == ' ') {
                printf("espaco %d\n", ascii[cont].quant);
            } else {
                printf("%c %d\n", ascii[cont].cod, ascii[cont].quant);
            }
        }
    }
}

/**
 * vaziaTREE verifica se a arvore está vazia
 * @param tree
 * @return 
 */
int vaziaTREE(TREE * tree) {
    return tree == NULL;
}

/**
 * LIbera todo o conteudo da arvore
 * @param tree
 * @return 
 */
TREE * liberaTREE(TREE * tree) {
    if (!vaziaTREE(tree)) {
        vaziaTREE(tree->left);
        vaziaTREE(tree->right);
        free(tree);
    }
    return NULL;
}

/**
 * Verifica a altura da arvore
 * @param tree
 * @return 
 */
int alturaTREE(TREE * tree) {
    if (tree == NULL) {
        return -1;
    }
    int dir;
    dir = alturaTREE(tree->right);
    int esq;
    esq = alturaTREE(tree->left);
    if (dir > esq) {
        return dir + 1;
    } else {
        return esq + 1;
    }
    return 0;
}

/**
 * imprime a arvore e sua altura
 * @param tree
 */
void printTREE(TREE * tree) {
    if (vaziaTREE(tree)) {
        return;
    }
    if (tree->cod == 0) {
        printf("(NULL %d %d)", tree->quant, alturaTREE(tree));
    } else if (tree->cod == '\n') {
        printf("(CR %d %d)", tree->quant, alturaTREE(tree));
    } else if (tree->cod == ' ') {
        printf("(ESPACO %d %d)", tree->quant, alturaTREE(tree));
    } else {
        printf("(%c %d %d)", tree->cod, tree->quant, alturaTREE(tree));
    }
    printTREE(tree->left);
    printTREE(tree->right);
}

/**
 * imprime a lista e toda as suas subarvores
 * @param lista
 */
void printLISTA(LISTA * lista) {
    nodeLISTA * node = lista->inicio;
    while (node != NULL) {
        printTREE(node->tree);
        printf("\n\n");
        node = node->next;
    }
    return;
}

/**
 * retira o segundo elemento da lista.
 * @param lista
 * @return 
 */
int retiraLISTA(LISTA * lista) {
    nodeLISTA * aux = lista->inicio;
    if (lista->tamanho == 1) {
        return 1;
    }
    while (aux->next->tree != NULL) {
        aux = aux->next;
    }
    nodeLISTA * temp = aux->next;
    aux->next = temp->next;
    free(temp);
    lista->tamanho--;
    return 1;
}

/**
 * ordena a lista em funcao da frequencia dos elemntos e da altura das subarvores
 * @param lista
 */
void ordenaLISTA(LISTA * lista) {
    nodeLISTA * node = lista->inicio;
    nodeLISTA * node2 = lista->inicio;
    ;
    TREE * aux;

    while (node != NULL) {
        node2 = node;
        while (node2 != NULL) {
            if (node->tree->quant > node2->tree->quant) {
                aux = node->tree;
                node->tree = node2->tree;
                node2->tree = aux;
            } else if (node->tree->quant == node2->tree->quant) {
                if (alturaTREE(node->tree) > alturaTREE(node2->tree)) {
                    aux = node->tree;
                    node->tree = node2->tree;
                    node2->tree = aux;
                }
            }
            node2 = node2->next;
        }
        node = node->next;
    }
}

/**
 * cria a arvore de huffman
 * @param lista
 * @return 
 */
TREE * huffman(LISTA * lista) {
    TREE * tree;
    while (lista->tamanho > 1) {
        ordenaLISTA(lista);
        lista->inicio->tree = criaTREE(lista->inicio->tree, lista->inicio->next->tree);
        lista->inicio->next->tree = NULL;
        retiraLISTA(lista);
    }
    tree = lista->inicio->tree;
    return tree;
}

/**
 * conta a quantidade de folhas na arvore
 * @param tree
 * @return 
 */
int contarFOLHAS(TREE * tree) {
    if (vaziaTREE(tree)) {
        return 0;
    }
    if (vaziaTREE(tree->left) && vaziaTREE(tree->right)) {
        return 1;
    }
    return contarFOLHAS(tree->left) + contarFOLHAS(tree->right);
}

/**
 * funcao preORDEM - percorre a arvore em pre-ordem procurando as folhas
 * @param file
 * @param tree
 * @param str
 * @param pos
 */
void preORDEM(FILE * file, TREE * tree, unsigned int * str, int pos) {
    if (!vaziaTREE(tree)) {
        if ((int) tree->cod != 0) {
            int i;
            fprintf(file, "\n");
            for (i = 0; i < pos; i++) {
                fprintf(file, "%u", str[i]);
            }
            fprintf(file, " %d", tree->cod);
        }
        str[pos] = 1;
        preORDEM(file, tree->left, str, pos + 1);
        str[pos] = 0;
        preORDEM(file, tree->right, str, pos + 1);
    }
}

/**
 * funcao codeHUFFMAN - basicamente só chama a função preORDEM com os 
 * argumentos certos para gerar o arquivo "codificacao.txt"
 * @param tree
 */
void codeHUFFMAN(TREE * tree) {
    FILE * file = fopen("codificacao.txt", "w");
    fprintf(file, "%d", contarFOLHAS(tree));
    unsigned int * str = (unsigned int *) calloc(sizeof (unsigned int), alturaTREE(tree)* 2);
    preORDEM(file, tree, str, 0);
    free(str);
    fclose(file);
    return;
}

/**
 * estrutura HUFF -> serve para saber qual o codigo binario para cada caractere do texto.
 */
typedef struct HUFF {
    char code[32];
    int cod;
} HUFF;

/**
 * codigo do professor
 * @param output
 * @param code
 */
void escrever_binario(FILE *output, unsigned int code) {
    static int output_bit_count = 0;
    static unsigned long output_bit_buffer = 0L;
    // quando recebe 2 imprime todo o resto do buffer.
    if (code == 2) {
        putc(output_bit_buffer >> CHAR_BIT * sizeof (unsigned int) -BITS, output);
        return;
    }
    output_bit_buffer |= (unsigned long) code << (CHAR_BIT * sizeof (unsigned int) -BITS - output_bit_count);
    output_bit_count += BITS;
    while (output_bit_count >= CHAR_BIT) {
        putc(output_bit_buffer >> CHAR_BIT * sizeof (unsigned int) -BITS, output);
        output_bit_buffer <<= CHAR_BIT;
        output_bit_count -= CHAR_BIT;
    }
}

/**
 * codigo do livro do deitel
 */
void displayBITS(unsigned int value, FILE * ptr) {
    unsigned int c; // counter
    unsigned int displayMask = 1 << 31;
    for (c = 1; c <= 31; ++c) {
        value <<= 1; //shift value left by 1
    }// end for
    //        putchar(value & displayMask ? '1' : '0');
    putc(value & displayMask ? '1' : '0', ptr);
}

/**
 * ler o texto e reescreve em forma de texto binário. 
 * @param entrada
 */
void codificando(char * entrada) {
    FILE * fileCODE = fopen("codificacao.txt", "r");
    FILE * fileIN = fopen(entrada, "r");
    FILE * fileOUT = fopen("huffman.bin", "w+b");
    int size;
    fscanf(fileCODE, "%d", &size);
    HUFF * huff;
    huff = (HUFF*) calloc(sizeof (HUFF), size);
    int i, j;
    for (i = 0; i < size; i++) {
        fscanf(fileCODE, "%s %d", huff[i].code, &huff[i].cod);
    }
    fclose(fileCODE);
    char c;
    c = fgetc(fileIN);
    while (c != EOF) {
        i = 0;
        if (c >= 0) {
            while ((int) c != huff[i].cod) {
                i++;
            }
            if (i > size) {
                printf("ERRO: CARACTERE NÃO ENCONTRADO\n");
                exit(1);
            }
            j = 0;
            while (huff[i].code[j] != '\0') {
                displayBITS((unsigned int) (huff[i].code[j] - '0'), fileOUT);
                j++;
            }
        }
        c = fgetc(fileIN);
    }
    //acrescenta o EOF no final do arquivo binário.
    i = 0;
    while ((int) c != huff[i].cod) {
        i++;
    }
    j = 0;
    while (huff[i].code[j] != '\0') {
        displayBITS((unsigned int) (huff[i].code[j] - '0'), fileOUT);
        j++;
    }
    fclose(fileOUT);
    fclose(fileIN);
    free(huff);
}

/**
 * main
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    setlocale(LC_ALL, "Portuguese_Brasil");

    FILE* ptr_file;
    ASCII* ascii = (ASCII*) calloc(sizeof (ASCII), ASCII_CODE); //table de hash
    char c; // minha letra a ser lida
    ptr_file = fopen(argv[1], "r+");
    c = fgetc(ptr_file);
    int total = 0;
    while (c != EOF) {
        if (c >= 0) {
            ascii[c].cod = c;
            ++ascii[c].quant;
            total++;
        }
        c = fgetc(ptr_file);
    }
    total++;
    ascii[ASCII_CODE - 1].cod = EOF;
    ++ascii[ASCII_CODE - 1].quant;
    printf("Este texto possui %d letras\n", total);
    fclose(ptr_file);
    LISTA lista;
    criaLISTA(&lista, ascii);
    free(ascii);
    TREE * tree = huffman(&lista);
    free(lista.inicio);
    codeHUFFMAN(tree);
    liberaTREE(tree);
    codificando(argv[1]);
    return 0;
}
