#include <stdio.h>
#include <stdlib.h>
FILE *yyin;
FILE *yyout;
FILE *out;

typedef enum {tipoConst, tipoIdent, tipoOperad, tipoNaoTerm, tipoIdentAtrib, tipoIdentFunc} tipoNo; //tipo de no
//tipo de nos nao terminais
typedef enum {bloco, programa, comandoret, comando, exp, chamadadefuncao, listadenomes, listaexp, opbin, opunaria } tipoNaoTerminal; 

//no nome -- identificador -- variavel
typedef struct{			//Identificadores
    char ident[100];
}noId;

//no numero -- constante
typedef struct{			//constantes
    int num;			//valor da constante
}noConst;

//no operador
typedef struct{			//Operadores
    int oper, nops;		//operador | número de operandos
    struct noTrack *op[1];		//operandos
}noOperad;

/* nao terminais */
typedef struct {
    tipoNaoTerminal terminal;
    int numFilhos;                   
    struct noTrack *filho[1];  /* numero de operads*/
}noNaoTerminal;

//estrutura do no
typedef struct noTrack{		//marcação
    tipoNo tipo;		//tipo de nó
    union {
        noConst noConst;			//constantes
        noId noId;			//identificadores
        noOperad noOperad;		//operadores
	noNaoTerminal noNaoTerminal; 	//nao terminal
    };
}noTipo;


extern int sym[26];


//-------------------------------------------------------------
//estrutura tambela de simbolos
typedef struct{
    char nome[100];
    int valor;
    struct sym_table *next;
}sym_table;


