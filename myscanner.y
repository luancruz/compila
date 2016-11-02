%{
#include <stdio.h>
#include <stdlib.h>
#include "estrutura_arvore.h"
#include <stdarg.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <math.h>
#include <string.h>
//#include "myscanner.tab.h"

#define N 30

FILE *yyin;
FILE *yyout;

//extern int yylex();
//extern FILE *yyin;
extern const char *yytext;
void yyerror(char *msg);

int sym[26];

//inicio
int init(noTipo* ptr);

//------------------------Composicao nos---------------------------------------------------
int i=0;//declaracao de variaveis usados no processo


//no tipo Constante -- definido
noTipo* noConstante(int x){
    	noTipo *ptr;
    	if(!(ptr=malloc(sizeof(noTipo)))){
   		yyerror("Impossivel alocar na heap\n");
		return ptr;
	}
    	ptr->tipo = tipoConst;
    	ptr->noConst.num = x;
    	return ptr;
}

//no tipo identificador -- definido
noTipo* noIdent(char c[100]){
    	noTipo *ptr;
    	if(!(ptr=malloc(sizeof(noTipo)))){
        	yyerror("Impossivel alocar na heap\n");
		return ptr;
	}
    	ptr->tipo = tipoIdent;
	strcpy(ptr->noId.ident,c);
    	//ptr->noId.ident = c;
    	return ptr;
}

//no tipo identificador atribuicao -- definido
noTipo* noIdentAtrib(char c[100]){
    	noTipo *ptr;
    	if(!(ptr=malloc(sizeof(noTipo)))){
        	yyerror("Impossivel alocar na heap\n");
		return ptr;
	}
    	ptr->tipo = tipoIdentAtrib;
	strcpy(ptr->noId.ident,c);
    	//ptr->noId.ident = c;
    	return ptr;
}

noTipo* noIdentFunc(char c[100]){
	noTipo *ptr;
    	if(!(ptr=malloc(sizeof(noTipo)))){
        	yyerror("Impossivel alocar na heap\n");
		return ptr;
	}
    	ptr->tipo = tipoIdentFunc;
	strcpy(ptr->noId.ident,c);
    	//ptr->noId.ident = c;
    	return ptr;
}


//no tipo nao-terminal -- definido
noTipo* noNaoTerm(int naoOper, int nfilho, ...) {
	va_list ap;
	noTipo *ptr;
    	//if(!(ptr=malloc(sizeof(noTipo)))){
        //	yyerror("Impossivel alocar na heap\n");
	//	return ptr;
	//}
    	if(!(ptr = malloc(sizeof(noTipo) + (nfilho-1) * sizeof(noTipo *)))){
		yyerror("Impossivel alocar na heap\n");
		return ptr;
	}
    	ptr->tipo = tipoNaoTerm;
	ptr->noNaoTerminal.terminal = naoOper;
	ptr->noNaoTerminal.numFilhos = nfilho;
	va_start(ap, nfilho);
    	for (i = 0; i < nfilho; i++)
       		ptr->noNaoTerminal.filho[i] = va_arg(ap, noTipo*);
    	va_end(ap);
    	return ptr;
}

//no tipo operador -- definido
noTipo* operad(int oper, int nops, ...) {
    	va_list ap;
    	noTipo *ptr;
    	//if(!(ptr=malloc(sizeof(noTipo)))){
        //	yyerror("Impossivel alocar na heap\n");
	//	return ptr;
	//}
    	if(!(ptr = malloc(sizeof(noTipo) + (nops-1) * sizeof(noTipo *)))){
		yyerror("Impossivel alocar na heap\n");
	}
	ptr->tipo = tipoOperad;
	ptr->noOperad.oper = oper;
	ptr->noOperad.nops = nops;
	va_start(ap, nops);//inicia lista de tamanho nops
	for (i = 0; i < nops; i++){
		ptr->noOperad.op[i] = va_arg(ap, noTipo*);
	}
	va_end(ap);
    	return ptr;
}

//add filhos nao terminais, se houverem -- nao recursao
void addNaoTerminal(noTipo ** pai, noTipo *filho){
	int numFilhos = (*pai)->noNaoTerminal.numFilhos;

	if(((*pai) = (noTipo*) realloc((*pai), sizeof(noTipo) + (numFilhos) * sizeof(noTipo*))) == NULL)
        	yyerror("Impossivel alocar na heap\n");

    	(*pai)->noNaoTerminal.filho[numFilhos] = filho;
    	(*pai)->noNaoTerminal.numFilhos = ++numFilhos;
}


void libera_no_arvore(noTipo *ptr) {
    	int a;
    	if(!ptr)
		return;
    	if (ptr->tipo == tipoOperad){
  		for (i=0; i < ptr->noOperad.nops; i++){
            		libera_no_arvore(ptr->noOperad.op[i]);
		}
		free(ptr->noOperad.op);
    	}
    	free(ptr);
}



%}

%union {
	int valor;
    	char* variavel;
	noTipo *PointerTipoNo;
}
%start entrada
%token <valor> T_NUMBER
%token <variavel> T_NAME
%token caracterInvalido
%token T_AND T_NOT T_OR T_ELSEIF T_WHILE T_DO T_FUNCTION T_END T_FOR T_ELSE T_IF T_THEN T_RETURN T_LOCAL T_NIL T_PLUS 
%token T_MINUS T_TIMES T_DIV T_COMMA T_OPENPAR T_CLOSEPAR T_SEMICOL T_ASSIGN T_EQ T_NEQ T_LTEQ T_GTEQ T_LT T_GT T_PRINT

%nonassoc FOR
%nonassoc FUNCTION
%nonassoc LOCAL
%nonassoc MENOS
%nonassoc CALL
//%nonassoc LIST

%left T_OR
%left T_AND
%left T_EQ T_NEQ T_LTEQ T_GTEQ T_LT T_GT
%left T_PLUS T_MINUS
%left T_TIMES T_DIV
%left T_NOT
%right T_ASSIGN

%type <PointerTipoNo> programa bloco Z comando elseif elseaux listadenomesAtribuicao nameAtribuicao
%type <PointerTipoNo> comandoret chamadadefuncao listadenomes name listaexp expraux exp 
//%type <PointerTipoNo> opbin opunaria expbin

%%

entrada	: 
	programa { init($1); libera_no_arvore($1); }
	;

programa	: 						
		bloco { $$ = noNaoTerm(programa, 1, $1); }
		;					
  
bloco : 
	Z {if($1 == NULL){$$ = noNaoTerm(bloco,0);}}
      	| Z comandoret { if($1 == NULL){ $$ = noNaoTerm(bloco,1,$2); } else{ $$ = $1;addNaoTerminal(&$$, $2); } }
	;

Z	: 							
	{ $$ = NULL; }
	| comando Z { if($1 != NULL){ $$ = noNaoTerm(bloco,1,$1); } if($2 != NULL){ int w = 0; for (w; w < $2->noNaoTerminal.numFilhos; ++w){addNaoTerminal(&$$, $2->noNaoTerminal.filho[w]);}} }
	;


comando : 
	T_SEMICOL 								{$$ = noNaoTerm(comando,1,operad(T_SEMICOL, 1, NULL)); }
	| listadenomesAtribuicao T_ASSIGN listaexp 				{$$ = noNaoTerm(comando,1,operad(T_ASSIGN,2,$1,$3)); }
	| chamadadefuncao 							{$$ = noNaoTerm(comando,1,$1); }
	| T_DO bloco T_END					{$$ = noNaoTerm(comando,3,operad(T_DO, 1, NULL),$2,operad(T_END, 1, NULL)); }
	| T_WHILE exp T_DO bloco T_END {$$ = noNaoTerm(comando,5,operad(T_WHILE, 1, NULL),$2,operad(T_DO, 1, NULL),$4,operad(T_END, 1, NULL));}
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_DO bloco T_END %prec FOR	{$$ = noNaoTerm(comando,9,operad(T_FOR, 1, NULL),noIdent($2),operad(T_ASSIGN, 1, NULL),$4,operad(T_COMMA, 1, NULL),$6, operad(T_DO, 1, NULL),$8,operad(T_END, 1, NULL));}
	| T_FOR T_NAME T_ASSIGN exp T_COMMA exp T_COMMA exp T_DO bloco T_END	{$$ = noNaoTerm(comando,11,operad(T_FOR, 1, NULL),noIdent($2),operad(T_ASSIGN, 1, NULL),$4,operad(T_COMMA, 1, NULL),$6,operad(T_COMMA, 1, NULL),$8,operad(T_DO, 1, NULL),$10,operad(T_END, 1, NULL));}
	
	| T_IF exp T_THEN bloco elseif elseaux T_END 				{$$ = noNaoTerm(comando,7,operad(T_IF, 1, NULL),$2,operad(T_THEN, 1, NULL), $4, $5, $6, operad(T_END, 1, NULL));} 

	| T_FUNCTION T_NAME T_OPENPAR T_CLOSEPAR bloco T_END %prec FUNCTION 
{$$ = noNaoTerm(comando,1,operad(T_FUNCTION, 2, noIdentFunc($2),$5));}

	| T_FUNCTION T_NAME T_OPENPAR listadenomes T_CLOSEPAR bloco T_END 
{$$ = noNaoTerm(comando,1,operad(T_FUNCTION, 3, noIdentFunc($2),$4,$6));}


	| T_LOCAL listadenomes %prec LOCAL {$$ = noNaoTerm(comando,1,operad(T_LOCAL, 1, $2));}


	| T_LOCAL listadenomesAtribuicao T_ASSIGN listaexp {$$ = noNaoTerm(comando,2,operad(T_LOCAL, 1, NULL), operad(T_ASSIGN, 2, $2,$4));}
	;

elseif	: 						
		{ $$ = NULL; }
		| T_ELSEIF exp T_THEN bloco elseif 	{$$ = operad(T_ELSEIF, 4, $2, operad(T_THEN, 1, NULL), $4,$5); }
		;

elseaux	:
		{ $$ = NULL; }
		| T_ELSE bloco { $$ = operad(T_ELSE, 1, $2); }
		;
	
comandoret : 
		T_RETURN				{$$ = noNaoTerm(comandoret,1,operad(T_RETURN, 1, NULL));}
		| T_RETURN listaexp			{$$ = noNaoTerm(comandoret,2,operad(T_RETURN, 1, NULL),$2);}
		| T_RETURN T_SEMICOL			{$$ = noNaoTerm(comandoret,2,operad(T_RETURN, 1, NULL),operad(T_SEMICOL, 1, NULL));}
		| T_RETURN listaexp T_SEMICOL		{$$ = noNaoTerm(comandoret,3,operad(T_RETURN, 1, NULL),$2,operad(T_SEMICOL, 1, NULL));}
		;

chamadadefuncao : 
		T_NAME T_OPENPAR T_CLOSEPAR %prec CALL 	{$$ = noNaoTerm(chamadadefuncao,3,noIdent($1),operad(T_OPENPAR, 1,NULL),operad(T_CLOSEPAR, 1,NULL));}	
		| T_NAME T_OPENPAR listaexp T_CLOSEPAR 
{if(strcmp($1, "print") == 0){$$ = noNaoTerm(chamadadefuncao,1,operad(T_PRINT, 1,$3));} else {$$ = noNaoTerm(chamadadefuncao,4,noIdent($1),operad(T_OPENPAR, 1,NULL), $3,operad(T_CLOSEPAR, 1,NULL)); } }
		;
listadenomesAtribuicao : 
		T_NAME nameAtribuicao				{$$ = noNaoTerm(listadenomes,2,noIdentAtrib($1),$2);}
		;

nameAtribuicao : 		
		{ $$ = NULL; }
		| T_COMMA T_NAME name			{$$ = operad(T_COMMA,2,noIdentAtrib($2),$3);}
	   	;

listadenomes : 
		T_NAME name				{$$ = noNaoTerm(listadenomes,2,noIdent($1),$2);}
		;

name : 		
		{ $$ = NULL; }
		| T_COMMA T_NAME name			{$$ = operad(T_COMMA,2,noIdent($2),$3);}
	   	;
	
listaexp : 						
		exp expraux				{$$ = noNaoTerm(listaexp,2,$1,$2); }
		;

expraux : 	
		{ $$ = NULL; }
		| T_COMMA exp expraux			{ $$ = operad(T_COMMA, 2, $2,$3); }
		;

exp : exp T_PLUS exp 			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_PLUS,2,$1,$3)));}
	|exp T_MINUS exp 		{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_MINUS,2,$1,$3)));}
	|exp T_TIMES exp		{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_TIMES,2,$1,$3)));}
	|exp T_DIV exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_DIV,2,$1,$3)));}
	|exp T_LT exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_LT,2,$1,$3)));}
	|exp T_GT exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_GT,2,$1,$3)));}
	|exp T_EQ exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_EQ,2,$1,$3)));}
	|exp T_NEQ exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_NEQ,2,$1,$3)));}
	|exp T_LTEQ exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_LTEQ,2,$1,$3)));}
	|exp T_GTEQ exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_GTEQ,2,$1,$3)));}
	|exp T_AND exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_AND,2,$1,$3)));}
	|exp T_OR exp			{$$ = noNaoTerm(exp,1, noNaoTerm(opbin,1,operad(T_OR,2,$1,$3)));}
	| T_MINUS exp			{$$ = noNaoTerm(exp,1,noNaoTerm(opunaria,1,operad(T_MINUS,1,$2)));}
	| T_NOT exp			{$$ = noNaoTerm(exp,1,noNaoTerm(opunaria,1,operad(T_NOT,1,$2)));}
	| T_NUMBER				{$$ = noNaoTerm(exp,1,noConstante($1)); }
	| T_NAME				{$$ = noNaoTerm(exp,1,noIdent($1)); }
	| T_NIL					{$$ = noNaoTerm(exp,1,operad(T_NIL,1,NULL)); }
	| chamadadefuncao			{$$ = noNaoTerm(exp,1,$1); }
	| T_OPENPAR exp T_CLOSEPAR		{$$ = noNaoTerm(exp,3,operad(T_OPENPAR,1,NULL),$2,operad(T_CLOSEPAR,1,NULL)); }	
	//| term				{$$ = $1;}
	; 

/*
exp : 
		expbin term				{$$ = noNaoTerm(exp,2,$1,$2); }
		| opunaria exp				{$$ = noNaoTerm(exp,2,$1,$2); }
		| T_OPENPAR exp T_CLOSEPAR		{$$ = noNaoTerm(exp,3,operad(T_OPENPAR,1,NULL),$2,operad(T_CLOSEPAR,1,NULL)); }
		;


expbin : 	{ $$ = NULL; }
		| expbin term opbin			{$$ = noNaoTerm(exp,3,$1,$2,$3); }
	 	;


term : 	
		T_NUMBER				{$$ = noNaoTerm(exp,1,noConstante($1)); }
		| T_NAME				{$$ = noNaoTerm(exp,1,noIdent($1)); }
		| T_NIL					{$$ = noNaoTerm(exp,1,operad(T_NIL,1,NULL)); }
		| chamadadefuncao			{$$ = noNaoTerm(exp,1,$1); }
		| T_OPENPAR exp T_CLOSEPAR		{$$ = noNaoTerm(exp,3,operad(T_OPENPAR,1,NULL),$2,operad(T_CLOSEPAR,1,NULL)); }
		;


opbin : 
		T_PLUS				{$$ = noNaoTerm(opbin,1,operad(T_PLUS,1,NULL)); }
		| T_MINUS			{$$ = noNaoTerm(opbin,1,operad(T_MINUS,1,NULL)); }
		| T_TIMES			{$$ = noNaoTerm(opbin,1,operad(T_TIMES,1,NULL)); }
		| T_DIV				{$$ = noNaoTerm(opbin,1,operad(T_DIV,1,NULL)); }
		| T_LT				{$$ = noNaoTerm(opbin,1,operad(T_LT,1,NULL)); }
		| T_GT				{$$ = noNaoTerm(opbin,1,operad(T_GT,1,NULL)); }
		| T_LTEQ			{$$ = noNaoTerm(opbin,1,operad(T_LTEQ,1,NULL)); }
		| T_GTEQ			{$$ = noNaoTerm(opbin,1,operad(T_GTEQ,1,NULL)); }
		| T_EQ				{$$ = noNaoTerm(opbin,1,operad(T_EQ,1,NULL)); }
		| T_NEQ				{$$ = noNaoTerm(opbin,1,operad(T_NEQ,1,NULL)); }
		| T_AND				{$$ = noNaoTerm(opbin,1,operad(T_AND,1,NULL)); }
		| T_OR 				{$$ = noNaoTerm(opbin,1,operad(T_OR,1,NULL)); }
		;

opunaria : 	
		T_MINUS	%prec MENOS		{$$ = noNaoTerm(opunaria,1,operad(T_MINUS,1,NULL)); }
		| T_NOT				{$$ = noNaoTerm(opunaria,1,operad(T_NOT,1,NULL)); }
		;

*/


%%

void yyerror(char *msg){
	fprintf(stderr, "%s\n",msg);
	//exit(1);
}

int main(int argc, char **argv)
{
	++argv, --argc; /* pular o nome do programa */
	if(argc == 0)
	{
		yyin = stdin;
		yyout = stdout;
	}
	else if(argc == 1)
	{
		yyin = fopen(argv[0], "r" );
		yyout = stdout;
	}
	else if(argc == 2){
		yyin = fopen(argv[0], "r" );
	    yyout = fopen(argv[1], "w");		
	}	
	yyparse();
	//printf("valor %d \n",yyparse());
//	printTree(arvore);
	fclose(yyin);
	fclose(yyout);
	return 0;	
}
