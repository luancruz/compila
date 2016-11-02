%{
#include <stdio.h>
#include "estrutura_arvore.h"
#include "myscanner.tab.h" //generated via bison -d

int CONT_LINE = 1;
%}

%option noyywrap

NUMERO [0-9]
LETRAS [a-zA-Z_]
NOME {LETRAS}([0-9]*{LETRAS}*)+

%%
"+" 			return T_PLUS; 
"-"			return T_MINUS;
"*" 			return T_TIMES; 
"/" 			return T_DIV; 
"," 			return T_COMMA; 
"(" 			return T_OPENPAR; 
")" 			return T_CLOSEPAR; 
";" 			return T_SEMICOL; 
"=" 			return T_ASSIGN;
"==" 			return T_EQ;
"~=" 			return T_NEQ;
"<=" 			return T_LTEQ;
">=" 			return T_GTEQ;
"<" 			return T_LT;
">" 			return T_GT;
and 			return T_AND; 
not 			return T_NOT;
or 			return T_OR;
elseif 			return T_ELSEIF;
while 			return T_WHILE;
do 			return T_DO;
function 		return T_FUNCTION;
for 			return T_FOR;
end 			return T_END;
else 			return T_ELSE;
if  			return T_IF;
then  			return T_THEN;
return 			return T_RETURN;
local  			return T_LOCAL;
nil   			return T_NIL;
{NUMERO}+ 		{yylval.valor = atoi(yytext); return T_NUMBER;}
{NOME}+  		{yylval.variavel = strdup(yytext); return T_NAME;}

[\n]			{CONT_LINE++;}
[--]+			;
[ \t\f\v]		;
.			{printf("CARACTER INVALIDO: %s NA LINHA %d\n",yytext,CONT_LINE); return caracterInvalido;}

%% 


